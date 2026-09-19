# No Cursor Shadow

`NoCursorShadow.asi` is a GTA San Andreas plugin that removes the offset
shadow from the front-end cursor.

The game draws every menu cursor twice: a translucent grey copy shifted down
and right, then the cursor itself. The plugin skips the shadow for the
standard arrow and the map cursor and changes nothing else.

## Features

- Removes the shadow from the menu cursor and the map drag cursor.
- Works while SA-MP is loaded.
- Verifies the bytes it replaces before writing and refuses to patch any
  other executable.

## Requirements

- GTA San Andreas 1.0 US (Compact or Hoodlum executable).
- An ASI loader, such as Silent's ASI Loader or Ultimate ASI Loader.

Other executable versions are left untouched.

## Installation

1. Extract `NoCursorShadow.asi` into the GTA San Andreas directory or its
   `scripts` directory.
2. Start the game.

There is nothing to configure. Remove the file to uninstall.

## Release Integrity

Releases are built by GitHub Actions from the tagged commit and carry a
SHA-256 file and a build-provenance attestation:

```text
gh attestation verify NoCursorShadow-vX.Y.Z.zip -R sonochiwa/sa-no-cursor-shadow
```

## License

MIT. See [LICENSE](LICENSE).
