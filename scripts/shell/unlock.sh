#!/bin/bash

is_locked() {
    adb shell dumpsys window 2>/dev/null | grep -q -E "mDreamingLockscreen=true|mShowingLockscreen=true"
}

if [ $# -eq 0 ]; then
    echo "Error: Please provide a PIN" >&2
    exit 1
fi

PIN="$1"

if ! [[ "$PIN" =~ ^[0-9]{4,6}$ ]]; then
    echo "Error: Invalid PIN format" >&2
    exit 1
fi

if ! is_locked; then
    echo "1"
    exit 0
fi

adb shell input keyevent 224 >/dev/null 2>&1
adb shell input keyevent 82 >/dev/null 2>&1
adb shell input text "$PIN" >/dev/null 2>&1
adb shell input keyevent 66 >/dev/null 2>&1

sleep 0.5
if ! is_locked; then
    echo "1"
else
    echo "Error: Unlock failed" >&2
    exit 1
fi
