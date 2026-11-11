---
name: refresh-assets
description: Registers newly added .flac and .webp files from /Assets in music.json, soundfx.json, textures.json and sprites.json, syncs those json files with their flatbuffers (.fbs) schemas, and updates c_textureSetHashes in main.cpp. Use when the user says "refresh assets", "refresh audio assets", "refresh graphics assets", or mentions adding new audio/graphics files.
allowed-tools: shell
---

# Refresh Assets (Bumble Tales Remastered)

Finds `.flac` / `.webp` files under `/Assets` (repo root) that are not yet registered and appends entries to the matching json files. Also keeps every json entry complete against its flatbuffers schema, and keeps `c_textureSetHashes` in `Game/source/main.cpp` in sync with `textures.json`.

All of this is done by one script, so a run only needs to execute that script and report its output. Do not open, view or hand-edit the json files or `main.cpp` afterwards unless the script reports a problem.

## Scope

The immediate sub folders of `/Assets` are the filter. Map the user's wording to `--scope`:

| User says | Scope |
|---|---|
| "refresh assets" (nothing else) | `All` |
| "refresh audio assets", music, sound fx | `Audio` |
| "refresh graphics assets", textures, sprites, images | `Graphics` |

## Steps

1. Run from the repo root (the folder containing `Assets`, e.g. `C:\source\BumbleTalesRemastered`; it is NOT the CMake build folder `C:\source_build\...`):
   ```
   python .github/skills/refresh-assets/sync_assets.py --scope All
   ```
   Add `--dry-run` first if the user wants a preview. Use `Audio` or `Graphics` for a subset. Use `python3` where `python` is not available (Linux/macOS). The script is Python 3, standard library only, so it is cross-platform.
2. Report the script output: what was added, schema backfills, warnings, stale entries.
3. Do not rebuild or commit unless asked.

The one thing it does to existing entries is keep each json file and the `c_textureSetHashes` array sorted by `name` (see below).

## What gets registered where

All paths in json are relative to `/Assets`, use forward slashes, and keep the case from disk. Existing registration is detected by path (case-insensitive).

| Source | Json | Entry |
|---|---|---|
| `Audio/Music/*.flac` | `Audio/music.json` | every schema field (`name`, `path`) |
| `Audio/FX/*.flac` | `Audio/soundfx.json` | every schema field (`name`, `path`) |
| `Graphics/Textures/**/*.webp` | `Graphics/textures.json` | every schema field (`name`, `path`) |
| same webp | `Graphics/sprites.json` | every schema field (`name`, `texture`, `frame_rate`, `looping`, ...) |
`"<name>"_hash64`, array kept sorted by name |

Textures and sprites are always 1:1 and share the same `name`; the sprite's `texture` is that name.

`c_textureSetHashes` is checked against `textures.json` on every Graphics run, so any texture missing from it is added (not only the ones added in this run). Names in the array that are not in `textures.json` are reported as stale, not removed.

## Schema-driven fields

Every entry always has every field from its flatbuffers schema written explicitly. The schemas are read at run time from `conjure/Engine/Packages/*/source/schemas/` (`music.fbs`, `soundfx.fbs`, `textures.fbs`, `sprites.fbs`), so adding a field to a schema needs no script change:

- **Existing entries missing a field** are backfilled with the schema default (`frame_rate` -> schema default `FPS60`, `looping` -> `true`, bool -> `false`, numbers -> `0`, enum -> first value). The schema default is used so behavior does not change.
- **New entries** get every schema field. The script supplies `name`, `path`, `texture`; everything else uses the schema default except the override below.
Overrides live in `NEW_ENTRY_OVERRIDES` at the top of the script.
- A field with no schema default (for example a new string field) cannot be filled in; the script warns and the field must be set by hand.
- Fields in a json entry that are not in the schema are kept as they are.

Only sprites that should auto-animate need a different `frame_rate` (`FPS10`, `FPS12`, `FPS15`, `FPS20`, `FPS30`, `FPS60`) or `"looping": false` for one-shot animations. The timing stored inside a webp does not reliably indicate this, so tell the user to adjust those entries when they add animated sprites.

## Sorting

If the array contains anything other than `"..."_hash64` entries (comments, other expressions), the script leaves it alone and warns. Entries are written with the `_hash64` literal; any older `C_Hash64("...")` entries are converted to it. The literal needs `using namespace cecore::Literals;` in `main.cpp` (already there); the script warns if it is missing.

## Naming conventions (observed from existing entries)

Names are hashed (`"Name"_hash64`) and used from game code, so they must be unique.

- **Music**: `bgm` + file stem without the `BGM_` prefix. `BGM_Menu.flac` -> `bgmMenu`.
- **Sound FX**: the file stem as is. `shopopen.flac` -> `shopopen`.
- **Textures/Sprites**: `<FolderPrefix><FileStem>`
  - `FolderPrefix` is the first sub folder under `Graphics/Textures`, first letter capitalized (`pieces` -> `Pieces`). Deeper folders are ignored (`Scrapbook/CharacterTales/CharTale01.webp` -> `ScrapbookCharTale01`).
  Add more exceptions to `TEXTURE_PREFIX_OVERRIDES` in the script.
  - The prefix is not repeated if the stem already starts with it (`Shop/ShopStarMeter.webp` -> `ShopStarMeter`, `Menu/MenuBG.webp` -> `MenuBG`).
  - Examples: `Menu/Button_StoryMode.webp` -> `MenuButton_StoryMode`, `StoryMode/Font_StarHUD.webp` -> `StoryModeFont_StarHUD`.
- Name collisions are skipped with a warning (never overwritten).

## File format rules (the script preserves these)

- CRLF line endings, UTF-8 without BOM, no trailing newline (json). `main.cpp` keeps its own encoding, line endings and indentation.
- `textures.json` / `sprites.json`: 8 space indent for braces, 12 for fields, entries joined as `},{`, `"key": value`.
- `music.json` / `soundfx.json`: 12 space indent for braces, 16 for fields, entries joined as `}, {`, `"key" : value` (space before colon).
- The json is parsed after writing; if it is invalid or lost entries the original file is restored.

## Other folders

- `.flac` outside `Audio/Music` and `Audio/FX` is reported and ignored.
- Only `.webp` under `Graphics/Textures` is considered.
- New asset folders or file types need the script and this document updated.

## Permissions

review changes to `sync_assets.py` like any other code.
