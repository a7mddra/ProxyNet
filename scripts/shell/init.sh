#!/bin/bash

ProxyURL="$1"
ProxyPort="$2"

REAL_USER=$(logname)

run_as_user() {
    sudo -u "$REAL_USER" DBUS_SESSION_BUS_ADDRESS="unix:path=/run/user/$(id -u $REAL_USER)/bus" "$@"
}

run_as_user gsettings set org.gnome.system.proxy mode 'manual'
run_as_user gsettings set org.gnome.system.proxy.socks host "$ProxyURL"
run_as_user gsettings set org.gnome.system.proxy.socks port "$ProxyPort"

adb forward tcp:"$ProxyPort" tcp:"$ProxyPort"
