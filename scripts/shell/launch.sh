#!/bin/bash

adb push /home/a7md/ProxyNet/config.hc /sdcard/

adb shell am start \
  -a android.intent.action.VIEW \
  -t application/octet-stream \
  -d "file:///sdcard/config.hc" \
  -n xyz.easypro.httpcustom/team.dev.epro.apkcustom.activities.ConfigImport

adb shell input swipe 650 800 100 800 300