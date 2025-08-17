#!/bin/bash

DeviceIP="$1"
DevicePort="$2"
ProxyURL="$3"
ProxyPort="$4"

REAL_USER=$(logname)

run_as_user() {
    sudo -u "$REAL_USER" DBUS_SESSION_BUS_ADDRESS="unix:path=/run/user/$(id -u $REAL_USER)/bus" "$@"
}

CONNECT_OUTPUT=$(adb connect "${DeviceIP}:${DevicePort}" 2>&1)

if echo "$CONNECT_OUTPUT" | grep -q 'connected'; then

    run_as_user gsettings set org.gnome.system.proxy mode 'manual'
    run_as_user gsettings set org.gnome.system.proxy.socks host "$ProxyURL"
    run_as_user gsettings set org.gnome.system.proxy.socks port "$ProxyPort"
    
    adb forward tcp:"$ProxyPort" tcp:"$ProxyPort"
    
    printf "1"
else
    printf "0"
    run_as_user gsettings set org.gnome.system.proxy mode 'none'
fi
