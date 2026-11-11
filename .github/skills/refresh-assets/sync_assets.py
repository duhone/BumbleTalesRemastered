#!/usr/bin/env python3
"""Registers new .flac / .webp files from /Assets in music.json, soundfx.json, textures.json and sprites.json,
keeps those json files in sync with their flatbuffers schemas (.fbs), and keeps c_textureSetHashes in
Game/source/main.cpp in sync with textures.json.

Usage: python sync_assets.py [--scope All|Audio|Graphics] [--dry-run]

Never removes entries or changes existing values. Stale entries are only reported.
Standard library only; runs on Windows, Linux and macOS.
"""
import argparse
import json
import re
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[3]
ASSETS_ROOT = REPO_ROOT / "Assets"
MAIN_CPP = REPO_ROOT / "Game" / "source" / "main.cpp"
PACKAGES_ROOT = REPO_ROOT / "conjure" / "Engine" / "Packages"

# Texture sub folders whose name prefix differs from the folder name.
TEXTURE_PREFIX_OVERRIDES = {"InfoHelp": "Help"}

# Values used for NEW entries instead of the schema default (schema default for frame_rate is FPS60).
# Existing entries that are missing a field are backfilled with the real schema default so behavior doesn't change.
NEW_ENTRY_OVERRIDES = {"frame_rate": "FPS0"}

# Json file (relative to Assets) -> schema file name, and the text layout of that file (matches the existing files).
JSON_STYLES = {
	"Audio/music.json": dict(schema="music.fbs", brace="            ", field="                ", sep=", {", colon=" : "),
	"Audio/soundfx.json": dict(schema="soundfx.fbs", brace="            ", field="                ", sep=", {", colon=" : "),
	"Graphics/textures.json": dict(schema="textures.fbs", brace="        ", field="            ", sep=",{", colon=": "),
	"Graphics/sprites.json": dict(schema="sprites.fbs", brace="        ", field="            ", sep=",{", colon=": "),
}

# Audio sub folder -> json file.
AUDIO_MAP = {"Music": "Audio/music.json", "FX": "Audio/soundfx.json"}

SCALAR_TYPES = {"bool", "byte", "ubyte", "short", "ushort", "int", "uint", "long", "ulong", "float", "double",
				"int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "float32", "float64"}

added, changes, warnings, stale = [], [], [], []
dry_run = False


def rel_path(p):
	return p.relative_to(ASSETS_ROOT).as_posix()


def sorted_files(root, suffix):
	if not root.is_dir():
		return []
	files = [p for p in root.rglob("*") if p.is_file() and p.suffix.lower() == suffix]
	return sorted(files, key=lambda p: p.as_posix().lower())


def read_text(path):
	return path.read_bytes().decode("utf-8-sig")


# ---------------------------------------------------------------- schema
class Schema:
	def __init__(self, path, array, fields, enums):
		self.path, self.array, self.fields, self.enums = path, array, fields, enums


_schema_cache = {}


def get_schema(schema_name):
	if schema_name in _schema_cache:
		return _schema_cache[schema_name]
	found = None
	if PACKAGES_ROOT.is_dir():
		for p in sorted(PACKAGES_ROOT.rglob(schema_name)):
			if p.is_file() and "/source/schemas/" in p.as_posix().lower():
				found = p
				break
	if found is None:
		raise SystemExit(f"Schema {schema_name} not found under conjure/Engine/Packages/*/source/schemas")

	text = read_text(found)
	text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
	text = re.sub(r"//[^\r\n]*", "", text)

	enums = {}
	for m in re.finditer(r"enum\s+(\w+)\s*:\s*\w+\s*\{([^}]*)\}", text):
		values = [v.split("=")[0].strip() for v in m.group(2).split(",")]
		enums[m.group(1)] = [v for v in values if v]

	tables = {}
	for m in re.finditer(r"table\s+(\w+)\s*\{([^}]*)\}", text):
		fields = []
		for decl in m.group(2).split(";"):
			fm = re.match(r"\s*(\w+)\s*:\s*(\[?[\w.]+\]?)\s*(?:=\s*([^\s(]+))?", decl)
			if fm:
				fields.append(dict(name=fm.group(1), type=fm.group(2), default=fm.group(3)))
		tables[m.group(1)] = fields

	rm = re.search(r"root_type\s+(\w+)\s*;", text)
	if not rm:
		raise SystemExit(f"No root_type in {found}")
	array_field = next((f for f in tables[rm.group(1)] if re.fullmatch(r"\[\w+\]", f["type"])), None)
	if array_field is None:
		raise SystemExit(f"Root table in {found} has no vector field")
	element_table = array_field["type"].strip("[]")

	schema = Schema(found, array_field["name"], tables[element_table], enums)
	_schema_cache[schema_name] = schema
	return schema


def get_field_literal(field, value):
	"""Json literal for a value of a schema field type."""
	if field["type"] == "bool":
		return value.lower()
	if field["type"] in SCALAR_TYPES:
		return value
	return json.dumps(value, ensure_ascii=False)


def get_default_literal(field, schema):
	"""Json literal of the schema default, or None when the schema has none (strings, tables)."""
	if field["default"] is not None:
		return get_field_literal(field, field["default"].strip('"'))
	if field["type"] == "bool":
		return "false"
	if field["type"] in SCALAR_TYPES:
		return "0"
	if field["type"] in schema.enums:
		return get_field_literal(field, schema.enums[field["type"]][0])
	return None


# ---------------------------------------------------------------- json
def format_json(orig_text, entries, style):
	"""Rebuilds the json text in the existing layout. Everything before '[' and after the last entry is kept as is."""
	nl = "\r\n" if "\r\n" in orig_text else "\n"
	open_idx = orig_text.index("[")
	close_idx = orig_text.rindex("]")
	tail = orig_text[len(orig_text[:close_idx].rstrip()):]

	out = [orig_text[:open_idx + 1]]
	for i, entry in enumerate(entries):
		out.append(nl + style["brace"] + "{" if i == 0 else style["sep"])
		keys = list(entry.keys())
		for k, key in enumerate(keys):
			comma = "," if k < len(keys) - 1 else ""
			out.append(nl + style["field"] + json.dumps(key) + style["colon"] + entry[key] + comma)
		out.append(nl + style["brace"] + "}")
	out.append(tail)
	return "".join(out)


def name_sort_key(name):
    return (name.lower(), name)


def load_entries(rel_file):
	schema = get_schema(JSON_STYLES[rel_file]["schema"])
	return json.loads(read_text(ASSETS_ROOT / rel_file))[schema.array]


def update_json(rel_file, new_items):
	"""Backfills schema fields on existing entries, appends new entries, writes the file if anything changed.
	new_items: dicts with the values known to the script (name, path, texture)."""
	style = JSON_STYLES[rel_file]
	schema = get_schema(style["schema"])
	full = ASSETS_ROOT / rel_file
	text = read_text(full)

	entries = []
	for e in json.loads(text)[schema.array]:
		entries.append({k: json.dumps(v, ensure_ascii=False) for k, v in e.items()})

	backfill = {}
	no_default = []
	for d in entries:
		for f in schema.fields:
			if f["name"] in d:
				continue
			lit = get_default_literal(f, schema)
			if lit is None:
				if f["name"] not in no_default:
					no_default.append(f["name"])
				continue
			d[f["name"]] = lit
			b = backfill.setdefault(f["name"], dict(count=0, value=lit))
			b["count"] += 1
	for n in no_default:
		warnings.append(f"{rel_file} has entries missing '{n}' and the schema has no default for it. Fill in manually.")

	for item in new_items:
		d = {}
		for f in schema.fields:
			name = f["name"]
			if name in item:
				d[name] = get_field_literal(f, item[name])
			elif name in NEW_ENTRY_OVERRIDES:
				d[name] = get_field_literal(f, NEW_ENTRY_OVERRIDES[name])
			else:
				lit = get_default_literal(f, schema)
				if lit is None:
					warnings.append(f"New entry '{item['name']}' in {rel_file} has no value for schema field '{name}'; set to empty.")
					lit = '""'
				d[name] = lit
		entries.append(d)

	for k, b in backfill.items():
		changes.append(f"{rel_file} : backfilled schema field '{k}' = {b['value']} on {b['count']} existing entries")

	# entries are kept sorted by name
	unsorted_order = [d.get("name") for d in entries]
	entries.sort(key=lambda d: name_sort_key(json.loads(d["name"])) if "name" in d else ("", ""))
	reordered = [d.get("name") for d in entries] != unsorted_order
	if reordered:
		changes.append(f"{rel_file} : entries sorted by name")

	if not backfill and not new_items and not reordered:
		return
	if dry_run:
		return

	new_text = format_json(text, entries, style)
	full.write_bytes(new_text.encode("utf-8"))
	try:
		ok = len(json.loads(new_text)[schema.array]) == len(entries)
	except ValueError:
		ok = False
	if not ok:
		full.write_bytes(text.encode("utf-8"))
		raise SystemExit(f"Result for {rel_file} was not valid json (or lost entries), original restored.")


# ---------------------------------------------------------------- audio
def sync_audio():
	audio_root = ASSETS_ROOT / "Audio"
	flacs = sorted_files(audio_root, ".flac")

	for folder_name, rel_file in AUDIO_MAP.items():
		existing = load_entries(rel_file)
		known_paths = {e["path"].lower() for e in existing}
		known_names = {e["name"].lower() for e in existing}

		new = []
		for f in flacs:
			parts = f.relative_to(audio_root).parts
			if len(parts) < 2 or parts[0].lower() != folder_name.lower():
				continue
			rel = rel_path(f)
			if rel.lower() in known_paths:
				continue

			stem = f.stem
			if folder_name == "Music":
				m = re.match(r"BGM_(.+)$", stem, re.I)
				name = "bgm" + (m.group(1) if m else stem)
			else:
				name = stem

			if name.lower() in known_names:
				warnings.append(f"SKIPPED {rel} - name '{name}' already used in {rel_file}. Add manually with a unique name.")
				continue
			known_names.add(name.lower())
			new.append(dict(name=name, path=rel))
			added.append(f"{rel_file} : {name} -> {rel}")

		for e in existing:
			if not (ASSETS_ROOT / e["path"]).exists():
				stale.append(f"{rel_file} : '{e['name']}' points at missing file {e['path']}")
		update_json(rel_file, new)

	audio_folders = {k.lower() for k in AUDIO_MAP}
	for f in flacs:
		parts = f.relative_to(audio_root).parts
		if len(parts) == 1 or parts[0].lower() not in audio_folders:
			warnings.append(f"IGNORED {rel_path(f)} - not under Audio/Music or Audio/FX, so no json applies.")


# ---------------------------------------------------------------- graphics
def sync_graphics():
	tex_root = ASSETS_ROOT / "Graphics" / "Textures"
	tex_entries = load_entries("Graphics/textures.json")
	spr_entries = load_entries("Graphics/sprites.json")

	tex_by_path = {t["path"].lower(): t for t in tex_entries}
	tex_names = {t["name"].lower(): t["name"] for t in tex_entries}
	spr_names = {s["name"].lower() for s in spr_entries}

	new_tex, new_spr = [], []
	for f in sorted_files(tex_root, ".webp"):
		rel = rel_path(f)
		known = tex_by_path.get(rel.lower())
		if known is not None:
			name = known["name"]
		else:
			stem = f.stem
			parts = f.relative_to(tex_root).parts
			prefix = ""
			if len(parts) > 1:
				folder = parts[0]
				prefix = TEXTURE_PREFIX_OVERRIDES.get(folder, folder[:1].upper() + folder[1:])
			name = prefix + stem if prefix and not stem.startswith(prefix) else stem

			if name.lower() in tex_names:
				warnings.append(f"SKIPPED {rel} - texture name '{name}' already used. Add manually with a unique name.")
				continue
			tex_names[name.lower()] = name
			new_tex.append(dict(name=name, path=rel))
			added.append(f"textures.json : {name} -> {rel}")

		if name.lower() not in spr_names:
			spr_names.add(name.lower())
			new_spr.append(dict(name=name, texture=name))
			added.append(f"sprites.json : {name} -> texture {name}")

	for t in tex_entries:
		if not (ASSETS_ROOT / t["path"]).exists():
			stale.append(f"textures.json : '{t['name']}' points at missing file {t['path']}")
	for s in spr_entries:
		if s["texture"].lower() not in tex_names:
			stale.append(f"sprites.json : '{s['name']}' references unknown texture '{s['texture']}'")

	update_json("Graphics/textures.json", new_tex)
	update_json("Graphics/sprites.json", new_spr)

	sync_texture_set_hashes(sorted((t["name"] for t in tex_entries + new_tex), key=name_sort_key), tex_names)


def sync_texture_set_hashes(tex_order, tex_names):
	"""Adds textures missing from c_textureSetHashes in main.cpp and keeps the array sorted by name."""
	if not MAIN_CPP.exists():
		warnings.append(f"main.cpp not found at {MAIN_CPP}; c_textureSetHashes not updated.")
		return

	raw = MAIN_CPP.read_bytes()
	has_bom = raw.startswith(b"\xef\xbb\xbf")
	src = raw[3 if has_bom else 0:].decode("utf-8", errors="surrogateescape")
	nl = "\r\n" if "\r\n" in src else "\n"

	m = re.search(r"c_textureSetHashes\s*\[\s*\]\s*=\s*\{(?P<body>.*?)\}\s*;", src, re.S)
	if not m:
		warnings.append("c_textureSetHashes not found in main.cpp; not updated.")
		return

	body = m.group("body")
	body_start = m.start("body")
	# entries are written as "Name"_hash64; the older C_Hash64("Name") form is read too and converted
	entry_re = re.compile(r'"([^"]+)"\s*_hash64|(?:\w+::)*C_Hash64\(\s*"([^"]+)"\s*\)')
	entries = list(entry_re.finditer(body))
	if not entries:
		warnings.append("c_textureSetHashes in main.cpp has no \"...\"_hash64 entries; not updated.")
		return
	if re.sub(r"[\s,]", "", entry_re.sub("", body)):
		warnings.append("c_textureSetHashes in main.cpp contains something other than \"...\"_hash64 entries; not updated.")
		return
	if not re.search(r"using\s+namespace\s+[\w:]*Literals\s*;", src):
		warnings.append("main.cpp has no 'using namespace ...::Literals;', which the _hash64 literal needs.")

	existing = [e.group(1) or e.group(2) for e in entries]
	legacy = any(e.group(2) for e in entries)
	in_set = {n.lower() for n in existing}
	missing = [n for n in tex_order if n.lower() not in in_set]
	for k in sorted(in_set):
		if k not in tex_names:
			stale.append(f"main.cpp : c_textureSetHashes has '{k}' which is not in textures.json")

	# the whole array is kept sorted by name; the first entry stays on the line of the opening brace
	all_names = sorted(existing + missing, key=name_sort_key)
	if not missing and not legacy and existing == all_names:
		return

	# reuse the indentation of the line holding the last entry
	last_pos = body_start + entries[-1].start()
	line_start = max(src.rfind("\n", 0, last_pos), src.rfind("\r", 0, last_pos)) + 1
	indent = re.match(r"[ \t]*", src[line_start:]).group(0)
	new_body = (body[:entries[0].start()]
		+ ("," + nl + indent).join('"' + n + '"_hash64' for n in all_names)
		+ body[entries[-1].end():])
	new_src = src[:body_start] + new_body + src[body_start + len(body):]
	for n in missing:
		added.append(f"main.cpp : c_textureSetHashes += {n}")
	if existing != sorted(existing, key=name_sort_key):
		changes.append("main.cpp : c_textureSetHashes sorted by name")
	if legacy:
		changes.append("main.cpp : c_textureSetHashes converted from C_Hash64(\"...\") to \"...\"_hash64")
	if not dry_run:
		out = new_src.encode("utf-8", errors="surrogateescape")
		MAIN_CPP.write_bytes((b"\xef\xbb\xbf" if has_bom else b"") + out)


# ---------------------------------------------------------------- main
def main():
	global dry_run
	parser = argparse.ArgumentParser(description="Register new .flac/.webp assets and sync json files with their schemas.")
	parser.add_argument("--scope", default="All", type=str.capitalize, choices=["All", "Audio", "Graphics"],
						help="All (default), Audio or Graphics. Matches the immediate sub folders of /Assets.")
	parser.add_argument("--dry-run", action="store_true", help="Report what would change without modifying any file.")
	args = parser.parse_args()
	dry_run = args.dry_run

	if hasattr(sys.stdout, "reconfigure"):
		sys.stdout.reconfigure(encoding="utf-8", errors="replace")
	if not ASSETS_ROOT.is_dir():
		raise SystemExit(f"Assets folder not found at {ASSETS_ROOT}")

	if args.scope in ("All", "Audio"):
		sync_audio()
	if args.scope in ("All", "Graphics"):
		sync_graphics()

	print(f"Scope: {args.scope} ({'DRY RUN - nothing written' if dry_run else 'written'})")
	if not added and not changes:
		print("No new assets found and nothing to backfill. Everything is up to date.")
	if added:
		print(f"Added {len(added)} entries:")
		for a in added:
			print(f"  + {a}")
	if changes:
		print("Schema sync:")
		for c in changes:
			print(f"  ~ {c}")
	if warnings:
		print("Warnings:")
		for w in warnings:
			print(f"  ! {w}")
	if stale:
		print("Stale entries (not changed):")
		for s in stale:
			print(f"  ? {s}")


if __name__ == "__main__":
	main()
