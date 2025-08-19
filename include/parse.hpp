#pragma once
#include "headers.hpp"

struct Data {
    string MDeviceHeight;
    string MDeviceWidth;
    string DeviceHeight;
    string DeviceWidth;
    string DevicePort;
    string DevicePIN;
    string DeviceIP;
    string ApkX;
    string ApkY;
    string ProxyURL;
    string ProxyPort;
    string ConfigPath;
};

Data loaddata(const string& filepath);
Data editdata(const Data& data, const string& filepath);
Data resetdata(const string& filepath);
