# No Cursor Shadow

An ASI plugin that removes the offset shadow from the GTA San Andreas menu
cursor.

The game draws every front-end cursor twice: first as a translucent grey copy
shifted down and right, then as the normal cursor. This plugin skips only the
first draw call. It affects both the standard arrow and the four-way cursor
shown on the map without changing world, vehicle, pedestrian, or text shadows.

The patch is deliberately version-specific. It verifies the original machine
code before writing anything; an unknown or already-modified executable is left
untouched.

## Features

- Removes the shadow from the normal front-end cursor.
- Removes the shadow from the map drag cursor.
- Works while SA-MP is loaded because the affected renderer belongs to GTA.
- Makes no permanent changes to `gta_sa.exe`.

## Requirements

- GTA San Andreas 1.0 US with the expected `CMenuManager::Draw` code.
- A working ASI loader.
- Windows on an x86-compatible system.

Other executable versions are unsupported. The plugin safely does nothing when
the expected five-byte call signature is absent.

## Installation

Copy `NoCursorShadow.asi` to the GTA San Andreas directory containing
`gta_sa.exe`. Remove the file to uninstall the fix.

Release archives are laid out for direct extraction into that directory:

```text
NoCursorShadow.asi
README.txt
```

## Building

Build `NoCursorShadow.sln` with Visual Studio 2022, the v143 C++ toolset,
`Release` configuration, and `Win32` platform. The output is written to:

```text
build\NoCursorShadow.asi
```

## Repository Layout

```text
NoCursorShadow.sln
README.md
CHANGELOG.md
LICENSE
src\
  NoCursorShadow.cpp
  NoCursorShadow.rc
  NoCursorShadow.vcxproj
  resource.h
```

## How It Works

In GTA San Andreas 1.0 US, both front-end cursor branches call
`CSprite2d::Draw` with color `(100, 100, 100, 50)` and a rectangle offset by
`(6, 3)`. The plugin verifies the relative calls at `0x57C0BC` and `0x57C1B2`,
replaces them with stack cleanup, and leaves the following opaque cursor draws
unchanged.

## Release Integrity

Tagged release archives are built from the tagged source by GitHub Actions.
Each release includes a SHA-256 checksum file and a signed build-provenance
attestation. After downloading the archive, verify its provenance and integrity
with GitHub CLI:

```text
gh attestation verify NoCursorShadow-v1.0.0.zip -R sonochiwa/sa-no-cursor-shadow
```

The attestation identifies the repository workflow and source revision that
produced the archive. It is not a guarantee that the source is bug-free or
safe.

## License

This project is available under the MIT License. See `LICENSE`.
