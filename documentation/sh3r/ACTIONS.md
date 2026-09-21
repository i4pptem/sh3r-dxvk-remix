# GitHub Actions builds

The **SH3R Runtime Build** workflow builds the Release x64 Runtime and the matching
x86 game-facing bridge on a GitHub-hosted Windows runner. It also runs the SH3
rasterized-UI and injection-state tests in a separate Meson build directory.

## Running a build

- Push to `main`, `release/*` or `release-*`, or open a pull request targeting `main`.
- To build manually, open the repository's **Actions** tab, select
  **SH3R Runtime Build**, then choose **Run workflow** and the desired branch.
- If GitHub has disabled Actions in a newly created fork, enable them in its
  **Actions** tab before the first run.

No private NVIDIA test captures or Slack credentials are required. Dependencies
are fetched through the upstream submodules and Packman manifests. A fresh build
requires those public dependency servers to be available.

## Artifacts

A successful run provides:

- `sh3r-dxvk-remix-<commit>-release-x86-game`: Runtime and bridge files arranged
  for a 32-bit game such as Silent Hill 3. The root `d3d9.dll` is the x86 bridge;
  `.trex/d3d9.dll` is the x64 Runtime. Keep the `.trex` folder intact.
- `sh3r-dxvk-remix-<commit>-symbols`: debugging symbols, separate from the player
  package.
- `sh3r-build-logs-<run>`: available configure/build and unit-test logs, also
  uploaded when a build fails.

These artifacts contain the Runtime components, **not** the complete Silent Hill 3
mod, PC Fix, compatibility plugin or USD assets. Use a matching SH3R-Comp version,
close the game and back up the installed Runtime before testing an artifact.

The workflow does not publish a GitHub Release or replace any installed files.
A successful build and unit tests do not replace an in-game validation pass.
