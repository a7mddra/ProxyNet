#!/bin/bash

ConfigPath="$1"

adb push "$ConfigPath" /sdcard/

adb shell am start \
  -a android.intent.action.VIEW \
  -t application/octet-stream \
  -d "file:///sdcard/config.hc" \
  -n xyz.easypro.httpcustom/team.dev.epro.apkcustom.activities.ConfigImport
