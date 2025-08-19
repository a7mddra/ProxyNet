#!/bin/bash

NAME="$1"

if [ -z "$NAME" ]; then
    exit 1
fi

adb exec-out screencap -p > "tmp/${NAME}.png" || {
    exit 2
}

printf "%s" "tmp/${NAME}.png"
