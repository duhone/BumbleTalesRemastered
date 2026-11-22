#!/usr/bin/env python3
"""Dumps image entries from the legacy Bumble Tales graphics project files (.hgp).

Usage (from the repo root):
	python .github/skills/refresh-assets/read_hgp.py pieceShatter Selector
	python .github/skills/refresh-assets/read_hgp.py --all

Filters are case-insensitive substrings matched against the png file stem and the description.
Standard library only.
"""
import argparse
import struct
import sys
from pathlib import Path

DEFAULT_LEGACY_ROOT = Path(__file__).resolve().parents[3].parent / "BumbleTales-1"
HGP_FILES = ["BumbleTales/Resources/bumbletalesHD.hgp", "BumbleTales/Resources/bumbletales.hgp"]

# Remastered sprites.fbs FrameRate enum values (FPS0 = never auto animates).
FPS_ENUM = [10, 12, 15, 20, 30, 60]


class Reader:
	def __init__(self, data):
		self.data = data
		self.pos = 0

	def i32(self):
		v = struct.unpack_from("<i", self.data, self.pos)[0]
		self.pos += 4
		return v

	def f32(self):
		v = struct.unpack_from("<f", self.data, self.pos)[0]
		self.pos += 4
		return v

	def boolean(self):
		v = self.data[self.pos] != 0
		self.pos += 1
		return v

	def string(self):
		n = self.i32()
		v = self.data[self.pos:self.pos + n].decode("latin-1")
		self.pos += n
		return v


def read_hgp(path):
	"""Yields (hgf_path, image dict) for every image in the hgf section of an hgp file."""
	r = Reader(path.read_bytes())
	for _ in range(r.i32()):
		hgf_path = r.string()
		for _ in range(r.i32()):
			img = {"path": r.string(), "desc": r.string()}
			img["width"] = r.i32()
			img["height"] = r.i32()
			img["color_key"] = (r.i32(), r.i32(), r.i32())
			img["frame_rate"] = r.f32()
			img["auto_animate"] = r.boolean()
			img["frames_per_set"] = [r.i32() for _ in range(r.i32())]
			yield hgf_path, img


def suggest_fps(rate):
	if rate <= 0:
		return "FPS0"
	return "FPS" + str(min(FPS_ENUM, key=lambda v: abs(v - rate)))


def main():
	ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
	ap.add_argument("filters", nargs="*", help="substrings of png stem or description")
	ap.add_argument("--all", action="store_true", help="dump every image")
	ap.add_argument("--legacy-root", type=Path, default=DEFAULT_LEGACY_ROOT, help="legacy BumbleTales-1 folder")
	args = ap.parse_args()
	if not args.filters and not args.all:
		ap.error("give at least one filter or --all")

	filters = [f.lower() for f in args.filters]
	found = False
	for rel in HGP_FILES:
		hgp = args.legacy_root / rel
		if not hgp.exists():
			print(f"missing: {hgp}", file=sys.stderr)
			continue
		print(f"== {rel}")
		for _, img in read_hgp(hgp):
			stem = Path(img["path"].replace("\\", "/")).stem
			if not args.all and not any(f in stem.lower() or f in img["desc"].lower() for f in filters):
				continue
			found = True
			const = img["desc"].replace(" ", "_")
			print(f"  {stem:28} AssetList::{const:30} {img['width']}x{img['height']}"
				  f"  rate={img['frame_rate']:g} auto_animate={int(img['auto_animate'])}"
				  f"  frames_per_set={img['frames_per_set']}  (rate -> {suggest_fps(img['frame_rate'])})")
	if not found:
		print("no matching images (not in any hgp, so legacy code could not load it)")
		return 1
	return 0


if __name__ == "__main__":
	sys.exit(main())
