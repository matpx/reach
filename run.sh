#!/bin/sh

set -e

meson compile -C build reach
./build/reach
