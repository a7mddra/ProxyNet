#!/bin/bash

if adb shell ip addr show 2>/dev/null | grep -q 'tun0'; then
    printf "1"
else
    printf "0"
fi
