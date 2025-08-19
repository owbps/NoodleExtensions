# Noodle Extensions (Quest 1.40.8 Port)

This is a **work-in-progress port** of the Noodle Extensions mod for Beat Saber **1.40.8 (Quest)**. The goal is to bring back the advanced mapping tools and visual capabilities from earlier versions.

## Current Status

* ✅ Freely positioned walls
* ✅ Freely positioned notes
* ⚠️ Many animation features are still missing or unstable
* ⚠️ Bugs and incomplete behavior are expected

## What Doesn’t Work Yet

* Note/track animations
* Fake note handling
* Some Chroma/Mapping Extensions interactions
* Visual effects such as dissolves and transparency

## Roadmap

* Restore full note/track animation support
* Fix visual and playback glitches
* Re-implement missing features from the 1.37 Quest port
* Improve stability and compatibility with Chroma and Mapping Extensions

## Building from Source

Requirements:

* [qpm](https://github.com/QuestPackageManager/QPM.CLI)
* Powershell
* Ninja + CMake in `PATH`
* Latest Android NDK (`ndkpath.txt` should point to your NDK directory)

```sh
qpm restore
pwsh ./build.ps1
pwsh ./createqmod.ps1 noodle_extensions
```

## Contributing

This port is still experimental and under active development. If you’d like to help:

* Report bugs with logs, steps to reproduce, and map examples if possible
* Test maps that rely on Noodle features and share results
* Submit pull requests for fixes or missing functionality

Any feedback or contributions are appreciated to move this port closer to full feature parity.
