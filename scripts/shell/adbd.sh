#!/bin/bash

DeviceIP="$1"
DevicePort="$2"

adb connect "${DeviceIP}:${DevicePort}"
