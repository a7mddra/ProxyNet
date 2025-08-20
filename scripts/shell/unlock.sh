#!/bin/bash

is_locked() {
    adb shell dumpsys window 2>/dev/null | grep -q -E "mDreamingLockscreen=true|mShowingLockscreen=true"
}

PIN="$1"

if ! is_locked; then
    printf "1"
    exit 0
fi

adb shell input keyevent 224 >/dev/null 2>&1
adb shell input keyevent 82 >/dev/null 2>&1
adb shell input text "$PIN" >/dev/null 2>&1
adb shell input keyevent 66 >/dev/null 2>&1

if ! is_locked; then
    printf "1"
else
    printf "0"
fi
