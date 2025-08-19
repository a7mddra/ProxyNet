#!/bin/bash

DeviceIP="$1"
DevicePort="$2"

if timeout 0.2 bash -c "cat < /dev/null > /dev/tcp/${DeviceIP}/${DevicePort}" 2>/dev/null; then
    printf "1" 
else
    printf "0"
fi