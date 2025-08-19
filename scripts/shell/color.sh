#!/bin/bash

NAME="$1"
X="$2"
Y="$3"

if [ -z "$NAME" ] || [ -z "$X" ] || [ -z "$Y" ]; then
  echo "Usage: $0 <name> <x> <y>" >&2
  exit 1
fi

IMG="tmp/${NAME}.png"
if [ ! -f "$IMG" ]; then
  echo "ERR_NO_IMAGE" >&2
  exit 2
fi

convert "$IMG" -format "%[pixel:p{${X},${Y}}]" info:
