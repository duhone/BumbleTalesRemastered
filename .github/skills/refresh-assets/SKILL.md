---
name: refresh-assets
description: Registers newly added .flac and .webp files from /Assets in music.json, soundfx.json, textures.json and sprites.json, syncs those json files with their flatbuffers (.fbs) schemas, updates c_textureSetHashes in main.cpp, and sets each new sprite's frame_rate/looping from the legacy game code and .hgp project files. Use when the user says "refresh assets", "refresh audio assets", "refresh graphics assets", or mentions adding new audio/graphics files.
allowed-tools: shell
---

# Refresh Assets (Bumble Tales Remastered)

Finds `.flac` / `.webp` files under `/Assets` (repo root) that are not yet registered and appends entries to the matching json files. Also keeps every json entry complete against its flatbuffers schema, and keeps `c_textureSetHashes` in `Game/source/main.cpp` in sync with `textures.json`.

The registration is done by one script. The only manual follow-up is choosing `frame_rate` and `looping` for each newly added sprite from the legacy game (see [Sprite frame rate and looping](#sprite-frame-rate-and-looping)). Apart from that, do not hand-edit the json files or `main.cpp` unless the script reports a problem.

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
2. For every sprite the script added, work out `frame_rate` and `looping` from the legacy game and edit those two fields in `Graphics/sprites.json` (see [Sprite frame rate and looping](#sprite-frame-rate-and-looping)). Skip this for a dry run.
3. Report the script output (what was added, schema backfills, warnings, stale entries), plus the `frame_rate`/`looping` chosen for each new sprite and the legacy evidence for it.
4. Do not rebuild or commit unless asked.

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
Overrides live in `NEW_ENTRY_OVERRIDES` at the top of the script. New sprites start as `frame_rate: FPS0`, `looping: true` (not animated). That is only a placeholder until step 2 decides the real values.
- A field with no schema default (for example a new string field) cannot be filled in; the script warns and the field must be set by hand.
- Fields in a json entry that are not in the schema are kept as they are.

## Sprite frame rate and looping

The timing stored inside a webp is not reliable, so do not use it. The source of truth is the legacy game:

- Legacy game code: `../BumbleTales-1/BumbleTales/` (`.cpp`/`.h`/`.mm`)
- Legacy engine: `../BumbleTales-1/Engines/Graphics/` (`SpriteInternal.cpp`)
- Legacy graphics projects: `../BumbleTales-1/BumbleTales/Resources/bumbletalesHD.hgp` (HD, matches the remastered art) and `bumbletales.hgp` (SD; should agree)

Always check how the legacy **code** uses the sprite first. The `.hgp` values are only defaults that code can override. A sprite can have a frame rate and `auto_animate=1` in the `.hgp` and still never animate in game.

### How the legacy engine animates

- `Sprite::SetImage(id)` loads the `.hgp` defaults into the sprite: `auto_animate = default_auto_animate`, `frame_rate = default_frame_rate`.
- The frame rate is in frames per second (each frame lasts `1.0f / frame_rate` seconds).
- Auto animation wraps around (loops) forever, unless `AutoStopAnimate()` was called. That turns it into a one-shot that stops on the last frame.
- Code can override the defaults:
  - `SetAutoAnimate(true/false)` turns auto animation on or off. It also clears the auto-stop flag.
  - `AutoStopAnimate()` makes the current animation a one-shot.
  - `SetFrameRate(r)` / `UpdateFrameRate(r)` replace the `.hgp` frame rate. Also check wrappers such as `RepeatedSpriteHelper::SetFrameRate`.
  - `SetFrame(n)` / `SetFrameSet(n)` / `StepFrame()` / `NextFrameSet()` mean the code picks frames by hand.
  - `PauseAnimation(bool)` only pauses; it does not change looping or rate.
- Images have `frames_per_set`: one entry per frame set (row). Auto animation steps through the frames of the current set only.

### The .hgp format (binary, little-endian)

Written by `../BumbleTales-1/Tools/iPhone Tools/iPhone Tools/FileFormat/HGPFileManager.cs` / `HGFFile.cs` / `HPTImage.cs`. Strings are an `int32` length followed by that many single-byte chars.

```
int32 hgf_count
repeat hgf_count:
    string hgf_path                 e.g. .\Graphics\main.hgf
    int32  image_count
    repeat image_count (HPTImage):
        string  png_path            e.g. .\GraphicsHD\Pieces\pieceShatter0.png
        string  desc                e.g. "Piece Shatter 0"
        int32   width, height       size of ONE frame
        int32   color_key_red, color_key_blue, color_key_green   (red 0xF0 = high quality flag)
        float32 default_frame_rate  frames per second
        bool    default_auto_animate (1 byte)
        int32   num_frame_sets
        int32   frames_per_set[num_frame_sets]
(sound / font / level sections follow; not needed here)
```

The legacy code refers to an image by `CR::AssetList::<desc with spaces replaced by _>` (generated into `AssetList.h` / `AssetListFull.h`), e.g. `"Piece Shatter 0"` -> `CR::AssetList::Piece_Shatter_0`.

Use the bundled reader instead of decoding by hand. Filters are case-insensitive substrings of the png stem or the description. It prints the `AssetList::` constant, frame size, rate, auto_animate, frames_per_set and the nearest `FrameRate` enum value:

```
python .github/skills/refresh-assets/read_hgp.py pieceShatter Selector
```

It reports `no matching images` (exit code 1) when the png is not in any `.hgp`. That means the legacy code could not load it at all.

### Procedure for each new sprite

1. Find the legacy image: run `read_hgp.py <webp stem>` to get its `AssetList::` constant.
   - If it is **not in any .hgp**, and a search of the legacy repo for the stem and constant finds nothing, it is a dead asset. Tell the user and suggest removing the webp and its entries. Do not guess values.
2. Find every use of that constant in the legacy game code (`grep` the `AssetList::` name). Follow it through variables, arrays, members and helper classes to the sprite(s) it is set on. Then look at what the code does with those sprites (the calls listed above).
3. Decide whether it auto-animates in game:
   - It auto-animates if `.hgp` `auto_animate=1` and the code never turns it off, or if the code calls `SetAutoAnimate(true)`.
   - It does **not** auto-animate if the code always turns it off (`SetAutoAnimate(false)`), only drives frames by hand (`SetFrame`/`SetFrameSet`/`StepFrame`), or it is a single frame image. Then use `frame_rate: FPS0`, even if the `.hgp` has a non-zero rate or `auto_animate=1`. `looping` stays `true` (it has no effect at FPS0).
4. If it auto-animates:
   - `frame_rate`: use a rate set in code with `SetFrameRate`/`UpdateFrameRate` if there is one; otherwise use the `.hgp` `default_frame_rate`. For example, `ResourceTile.cpp` calls `SetFrameRate(20)` on tiles whose `.hgp` rate is 10. Map it to the nearest `FrameRate` enum (`FPS10`, `FPS12`, `FPS15`, `FPS20`, `FPS30`, `FPS60`). The remastered engine has one fixed rate per sprite entry, so if the code uses several different rates, pick the main one and tell the user.
   - `looping`: `false` only if every animated use is a one-shot (`AutoStopAnimate()`). If it loops anywhere, or is sometimes looping and sometimes one-shot, use `true`.
5. If HD and SD `.hgp` disagree, use HD and mention it. If the sprite has more than one frame set and auto-animates, mention it: legacy animation stayed within one set.

Example: `pieceShatter0`-`6` -> `.hgp` rate 15, `auto_animate=1`, `frames_per_set=[4,4,4,4]`. `CrystalParticle.cpp` only calls `SetImage(Piece_Shatter_N)` and never touches animation, so it loops at 15 fps -> `FPS15`, `looping: true`. `pieceShatter7.png` existed in the legacy art folders but was in no `.hgp` and had no `AssetList` constant, so it was dead and was removed.

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

review changes to `sync_assets.py` and `read_hgp.py` like any other code.
