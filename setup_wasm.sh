#!/bin/sh

set -e

rm -rf build
meson setup build --wipe --cross-file cross/emscripten.cross
