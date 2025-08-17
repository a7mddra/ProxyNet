#!/bin/bash

if adb shell "netstat -tlnp | grep -qE ':::1081|:::7071'"; then
    printf "1"
else
    printf "0"
fi