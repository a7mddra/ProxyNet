#!/bin/bash

REAL_USER=$(logname)

run_as_user() {
    sudo -u "$REAL_USER" DBUS_SESSION_BUS_ADDRESS="unix:path=/run/user/$(id -u $REAL_USER)/bus" "$@"
}

run_as_user gsettings set org.gnome.system.proxy mode 'none'