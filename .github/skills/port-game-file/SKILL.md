---
name: port-game-file
description: Ports one legacy Bumble Tales .h/.cpp class into a C++23 module under Game/source, adapting coordinates, assets, input, retained-mode rendering, namespaces, and engine APIs. Use when the user says "port <ClassName>".
---

# Port one legacy game file

Port exactly one legacy class at a time. The normal input is a class name, for example `port SeparatorItem`.

## Repositories and scope

Use repository-relative paths with `/` separators in all instructions and reports:

- Legacy game: `../BumbleTales-1/BumbleTales/`
- Legacy engine: `../BumbleTales-1/Engines/`
- Remastered game: `Game/source/`
- Remastered assets: `Assets/Graphics/`
- Remastered engine: `conjure/`

Read the legacy `.h` and `.cpp` (or `.mm`) and the closest already-ported game classes before editing. The legacy engine is read-only reference material.

The skill may edit any needed files under `Game/source/`, `Game/build/build.cmake`, and remastered asset-registration files. Do not edit anything under `conjure/`. If an engine feature is missing, stop at the game-side boundary and report the missing feature instead of changing the engine.

## Workflow

1. Perform a code-dependency preflight before editing anything:
   - Locate the legacy header and implementation, then inspect all direct base classes, included headers, member types, constructor parameters, return types, callbacks, and code-called helpers.
   - Check whether every required code dependency already has a remastered module in `Game/source/` and inspect its exported API. Always inspect `Constants.ixx` for shared constants, enums, and small dependency-free types before deciding that a new support file is needed.
   - Follow transitive dependencies far enough to detect missing modules and cycles. Missing code dependencies are a hard stop: do not create a partial port; report the missing dependency and cancel the port so it can be ported first. Missing assets are not a hard stop.
   - If a dependency cycle exists, resolve only dependency-free shared constants/types by moving or adding them to `Constants.ixx` (or a narrowly scoped support module) when possible. Do not silently port a second behavior-heavy class; if the cycle cannot be cleanly broken, cancel and report it.
2. Find analogous hand-ported modules and follow their module layout, naming, ownership, warning discipline, and API patterns.
3. Create one `Game/source/<ClassName>.ixx` module. Preserve the useful original header attribution, but do not add comments about the port or coordinate conversion.
4. Convert old types and APIs to the remastered equivalents:
   - `CR::Math::PointF`/`int2` to `glm::vec2`/`glm::ivec2`.
   - `CR::Utility` events/delegates to the existing `Event`, `Event1`, and `Delegate` patterns.
   - Sprite creation/deletion/position/frame/visibility to `CR::Engine::Graphics::Sprites` and valid sprite handles.
   - UIKit touch callbacks and `ITouchable` to persistent `Input::Regions` polling in `Update()`.
   - Legacy sound calls to `Audio::SoundFX` or `Audio::Music` handles.
   - Legacy iOS persistence/platform code to the existing remastered save and lifecycle patterns.
5. Convert layout carefully:
   - The old design is 320x480; the remastered design is 640x960.
   - Double legacy pixel coordinates and sizes unless the remastered asset/API already supplies the HD size.
   - Legacy sprite positions are usually centers; remastered `Sprites::SetPositions` takes top-left positions. Use `topLeft = 2 * oldCenter - newSize / 2` when the old code positioned a sprite by center.
   - Legacy input bounds were top-left rectangles; double them directly.
   - Check whether a class's public position is a center while its child sprites are top-left; preserve the established contract.
6. Convert z-order deliberately. The engines sort in opposite directions: in the legacy engine a smaller z is in front; in Conjure a larger z is in front because sprites are drawn in ascending z order. The remastered range is also smaller. For the usual legacy 0-1000 layers, divide by ten and reverse around 100: `newZ = 100 - oldZ / 10`. Thus old values `1000, 800, 400, 300` become new values `0, 20, 60, 70`, preserving the visual order while fitting the new range. Use the same divide-then-reverse rule for other old ranges, choosing the appropriate scene baseline and documenting the decision in the completion report. Never preserve old numeric z values directly.
7. Treat retained-mode state explicitly:
   - A sprite remains rendered after creation until hidden or deleted; `Render()` should update visibility, position, and frame state rather than relying on not calling a draw method.
   - For controls, distinguish visible and interactive state. `InputButton::SetVisible(false)` hides its sprite; `Disabled(true)` disables input. Pair them when legacy behavior hid a disabled control, and keep them separate when the old control stayed visible.
   - Audit `InputButton` users and preserve the three useful states: visible and working, visible but input-disabled, and invisible and input-disabled.
8. Register the module in alphabetical order in `Game/build/build.cmake` and add imports at each game-side consumer that needs it.
9. Register new graphics/audio assets using the existing asset workflow. Graphics normally need matching entries in `Assets/Graphics/textures.json` and `sprites.json`, plus a matching hash in `Game/source/main.cpp` (`c_textureSetHashes`). Prefer `.webp` assets already in `Assets`; if only legacy art exists, report that it needs to be converted/added rather than editing the engine.
10. Check for related hand-ported z-order, visibility, coordinate, or input assumptions in files touched by the new class. Fix tightly coupled game-side issues; report broader suspected issues separately rather than making unrelated changes.
11. Build the smallest useful target when practical. The port may remain non-compiling if the remastered engine or assets do not yet provide a required feature, but report every blocker explicitly, including file and symbol names.

## Conventions

Use `module;`, the existing engine/GLM global includes, `export module CR.Game.<ClassName>;`, `import CR.Engine;`, required `CR.Game.*` imports, `import std;`, `import std.compat;`, `export namespace CR::Game`, and a private module section with the established namespace aliases. Use `override`, `nullptr`, fixed-width types, brace initialization, and `[[maybe_unused]]` where the repository requires them. Keep existing ownership conventions unless a nearby port already uses a safer equivalent.

Do not edit `conjure/`, do not create a planning markdown file, and do not claim an asset or engine feature exists without verifying it.

## Completion report

Report:

- the created/changed game files;
- assets registered, copied, or still missing;
- coordinate and z-order decisions;
- retained-mode visibility/input decisions;
- build/test result;
- missing engine features or follow-up work, if any.
