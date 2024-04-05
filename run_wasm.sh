#!/bin/sh

set -e

meson compile -C build reach
python -m http.server
