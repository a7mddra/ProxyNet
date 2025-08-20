#!/bin/bash

adb wait-for-device
sleep 2

adb shell input tap 300 615
sleep 2

adb shell input tap 685 120
sleep 2

adb shell input tap 585 120
sleep 2

adb shell input tap 685 315
sleep 2

adb shell input keyevent 26
