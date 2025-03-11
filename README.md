# 2048

This is 2048 built with SDL2 with optional features, written as an assignment for submitting to my university.

## Preview

TODO

## Installation

TODO 

## Building
To build this game, you must have Meson, Ninja, CMake, make, g++/clang++, SDL2, SDL2_mixer, SDL2_ttf, SDL2_image along with required headers installed.

```bash
git clone https://github.com/teppyboy/2048
cd 2048
meson setup builddir  # Needed for installing SDL2 libraries to the project.
```

+ Debug
```bash
meson compile -C ./builddir
meson install -C ./builddir
```

+ Release
```bash
meson setup builddir-release --buildtype release --optimization 3 --strip
meson compile -C ./builddir-release
meson install -C ./builddir-release
```

The output will be in `./bin`

## Acknowledgements

### Fonts
+ Rubik: https://fonts.google.com/specimen/Rubik
+ JetBrains Mono: https://www.jetbrains.com/lp/mono/
+ FOT-Yuruka Std (for the logo)
### Assistances
+ Perplexity (with Claude 3.7 Sonnet)

## License

[MIT](./LICENSE)
