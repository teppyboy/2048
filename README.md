# 2048

This is 2048 built with SDL2 with optional features, written as an assignment for submitting to my university.

## Preview

![Preview 1](.github/assets/preview1.png "Preview of the game")
![Preview 2](.github/assets/preview2.png "Preview of the game")

## Installation

Go to [releases](https://github.com/teppyboy/2048/releases/latest) and download the latest version available for your platform,
if somehow a platform is not available then you can build from source using the tutorial below.

To launch the game on Windows run `2048.exe`, for Linux (and possibly macOS) run `start.sh`

To move the tiles use the WASD key or toggle arrow key support in the game settings.

## Building
To build this game, you must have Meson, Ninja, CMake, pkg-config, make, g++/clang++ along with required headers installed, both MSYS2 (MinGW) and MSVC are supported. 

```bash
git clone https://github.com/teppyboy/2048
cd 2048
meson setup builddir  # Needed for installing SDL2 libraries to the project.
```

> By default it'll build and copy files for MSYS2 environment, to build for MSVC append `msvc` to the build directory name (e.g. `builddir-msvc`)

+ Debug
```bash
meson install -C ./builddir
```

+ Release
```bash
# DO NOT USE --strip WHEN BUILDING WITH MSVC, IT DOESN'T WORK.
meson setup builddir-release --buildtype release --optimization 3 --strip
meson install -C ./builddir-release
```

The output will be in `./bin`

## Acknowledgements

### BGM
+ Vexento - Lonely Dance: https://soundcloud.com/vexento/vexento-lonely-dance
### SFX
+ Swipe: https://pixabay.com/vi/sound-effects/search/swipe/
### Fonts
+ Rubik: https://fonts.google.com/specimen/Rubik
+ JetBrains Mono: https://www.jetbrains.com/lp/mono/
+ FOT-Yuruka Std (for the logo and some text)
### Images (all are blurred)
+ Main menu background: https://www.pixiv.net/en/artworks/121665267
+ Game background: https://www.pixiv.net/en/artworks/127189078
+ Saves background: https://www.pixiv.net/en/artworks/124283216
+ Settings background: https://www.pixiv.net/en/artworks/123982957
### Assistance
+ StackOverflow
+ Perplexity (with Claude 3.7 Sonnet)

## License

[MIT](./LICENSE)
