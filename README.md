# 2048

This is 2048 built with SDL2 with optional features.

## Preview
TODO

## Installation

TODO 

## Building
To use this template, you must have Meson, Ninja, CMake, make, g++/clang++, SDL2, SDL2_mixer, SDL2_ttf, SDL2_image along with required headers installed.

```bash
git clone https://github.com/teppyboy/sdl2-template
cd sdl2-template
meson setup builddir  # Needed for installing SDL2 libraries to the project.
```

+ Debug
```bash
meson compile -C ./builddir
cp -r assets ./builddir/
```
The output will be in `./builddir`

+ Release
```bash
meson setup builddir-release --buildtype release --optimization 3 --strip
meson compile -C ./builddir-release
cp -r assets ./builddir-release/
```
The output will be in `./builddir-release`

## Acknowledgements

TODO

## License

[MIT](./LICENSE)
