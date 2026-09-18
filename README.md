# No Cursor Shadow

`NoCursorShadow.asi` is a standalone GTA San Andreas plugin that removes the
offset shadow from the front-end cursor.

The game draws every front-end cursor twice inside `CMenuManager::Draw`: first
a translucent grey copy shifted down and right, then the cursor itself. The
plugin skips only the first draw. It covers both the standard arrow and the
four-way cursor shown on the map, and changes no other shadow in the game.

The patch is version-specific. It verifies the original machine code at both
sites before writing anything; an unknown or already modified executable is
left untouched.

## Features

- Removes the shadow from the normal front-end cursor.
- Removes the shadow from the map drag cursor.
- Works while SA-MP is loaded, because the affected renderer belongs to GTA.
- Verifies the bytes it replaces before writing and refuses to patch any
  other executable.

## Requirements

- GTA San Andreas 1.0 US (Compact or Hoodlum executable).
- An ASI loader, such as Silent's ASI Loader or Ultimate ASI Loader.

Other executable versions are unsupported. When the expected five-byte call
is absent at either site the plugin does nothing.

## Installation

1. Extract `NoCursorShadow.asi` into the GTA San Andreas directory or its
   `scripts` directory.
2. Start the game.

Remove the file to uninstall the fix.

## Building

Visual Studio 2022 (v143), `Release|Win32`. Open `NoCursorShadow.sln` or run:

```powershell
msbuild NoCursorShadow.sln /t:Rebuild /p:Configuration=Release /p:Platform=Win32
```

The plugin is written to `build\NoCursorShadow.asi`.

## Repository Layout

```text
NoCursorShadow.sln
README.md
CHANGELOG.md
LICENSE
.github\workflows\release.yml   Tagged release build, checksum and attestation
src\
  NoCursorShadow.cpp            DllMain and the patch thread
  NoCursorShadow.rc             Version resource
  NoCursorShadow.vcxproj
  addresses.h                   Patch sites and expected bytes
  patch.cpp / patch.h           Readable-memory check and protected write
  resource.h
  version.h
```

## How It Works

In GTA San Andreas 1.0 US both front-end cursor branches call
`CSprite2d::Draw` with colour `(100, 100, 100, 50)` and a rectangle offset by
`(6, 3)` before drawing the opaque cursor. The plugin verifies the relative
calls at `0x57C0BC` (map crosshair) and `0x57C1B2` (standard cursor), replaces
each with `add esp, 8; nop; nop` to discard the two stack arguments the
`__thiscall` callee would have popped, and leaves the opaque cursor draws that
follow unchanged.

## Release Integrity

Tagged releases are built by GitHub Actions from the tagged commit. Each
release carries `NoCursorShadow-vX.Y.Z.zip`, its SHA-256 in
`NoCursorShadow-vX.Y.Z.zip.sha256` and a signed build-provenance attestation,
which proves that the archive was produced by this repository's workflow
from that revision. It does not prove the code is bug-free.

```text
gh attestation verify NoCursorShadow-vX.Y.Z.zip -R sonochiwa/sa-no-cursor-shadow
```

## License

MIT. See [LICENSE](LICENSE).
