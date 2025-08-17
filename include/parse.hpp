#pragma once
#include "headers.hpp"

struct Data {
    string DevicePIN;
    string DeviceIP;
    string DevicePort;
    string ProxyURL;
    string ProxyPort;
    string ConfigPath;
};

Data loaddata(const string& filepath);
Data editdata(const Data& data, const string& filepath);
Data resetdata(const string& filepath);
