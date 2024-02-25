#!/bin/sh

set -e

meson compile -C build
./build/reach
