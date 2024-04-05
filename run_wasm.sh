#!/bin/sh

set -e

meson compile -C build
python -m http.server
