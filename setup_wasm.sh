#!/bin/sh

set -e

meson setup build --wipe --cross-file cross/emscripten.cross
