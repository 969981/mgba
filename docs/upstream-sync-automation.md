# Official mGBA upstream sync and snapshot release automation

Workflow: [`.github/workflows/upstream-sync-release.yml`](../.github/workflows/upstream-sync-release.yml).

## Repository layout

The personal repository predates this automation and contains historical and customized branches. Nothing is force-pushed or merged into those branches by this workflow.

- `0.3`: existing default branch, hosting the scheduled workflow only.
- `master`: existing independent source branch, deliberately not overwritten.
- `mgba-ldn`: existing custom development branch, deliberately not overwritten.
- `upstream-master`: exact source mirror of the official `mgba-emu/mgba` master branch.

Source of truth: https://github.com/mgba-emu/mgba.git

## Scheduled behavior

- GitHub Actions checks upstream about every 10 minutes, as GitHub scheduling allows.
- On a new upstream master SHA, it fetches and **fast-forwards only** the dedicated `upstream-master` branch. History divergence fails rather than silently destroying work.
- If a GitHub Release tag `mgba-upstream-<first 12 SHA characters>` already exists, no build is started.
- If the release is missing, Windows, Linux, and macOS run separate builds from the mirrored revision.
- A snapshot Release is published only after **all three** build jobs finish successfully, and includes SHA256 checksums. Failed packages do not create incomplete releases. Scheduled checks do not repeatedly rebuild an unchanged upstream SHA after a failure; use **Run workflow** to retry, or the next upstream source change will trigger a build.
- Older Releases such as `revA-20260930` are not deleted or modified.

## Packaging details

- Windows x64: MinGW + Qt 6; upstream CPack ZIP (bundled runtime DLLs, validate before distribution).
- Linux x64: Ubuntu 24.04; CPack TAR.GZ. This is **not** an AppImage and relies on system libraries.
- macOS ARM64: Homebrew Qt 6 + `macdeployqt`; ad-hoc-signed DMG, not Apple-notarized.

## Manual run

GitHub repository → Actions → **Mirror mGBA upstream and publish desktop builds** → Run workflow.

No upstream write permission is required. The built-in `GITHUB_TOKEN` must have Contents write permission for the isolated mirror branch and release assets.

mGBA is available under Mozilla Public License 2.0; preserve the license and third-party notices in redistributions. These automated snapshots are unofficial, and no ROMs/BIOS/firmware are bundled.
