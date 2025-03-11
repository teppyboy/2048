#!/usr/bin/env python3
import os
import shutil
from pathlib import Path

source_path = Path(os.getenv("MESON_SOURCE_ROOT"))
build_path = Path(os.getenv("MESON_BUILD_ROOT"))
install_path = build_path.parent / "bin"
# Copy all dlls to the bin directory
for dll in build_path.glob("**/*.dll"):
    print(f"Copying {dll} to {install_path / 'bin' / dll.name}")
    shutil.copy(dll, install_path / dll.name)
for so in build_path.glob("**/*.so"):
    print(f"Copying {so} to {install_path / 'lib' / so.name}")
    shutil.copy(so, install_path / 'lib' / so.name)
for dylib in build_path.glob("**/*.dylib"):
    print(f"Copying {dylib} to {install_path / 'lib' / dylib.name}")
    shutil.copy(dylib, install_path / 'lib' / dylib.name)
# Copy assets
print(f"Copying assets to {install_path / 'assets'}")
shutil.copytree(source_path / 'assets', install_path / 'assets', dirs_exist_ok=True)
print("Done!")
