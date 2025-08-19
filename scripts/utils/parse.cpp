#include "project.hpp"

Data loaddata(const string& filepath) {
    Data data;
    auto abs = absolute(filepath);

    ifstream file(filepath);
    if (!file.is_open()) {
        throw runtime_error("Could not open data file: " + abs.string());
    }

    unordered_map<string, string*> mp = {
        {"MDeviceHeight", &data.MDeviceHeight},
        {"MDeviceWidth",  &data.MDeviceWidth },        
        {"DeviceHeight",  &data.DeviceHeight },
        {"DeviceWidth",   &data.DeviceWidth  },
        {"DevicePort",    &data.DevicePort   },
        {"DevicePIN",     &data.DevicePIN    },
        {"DeviceIP",      &data.DeviceIP     },
        {"ApkX",          &data.ApkX         },
        {"ApkY",          &data.ApkY         },
        {"ProxyURL",      &data.ProxyURL     },
        {"ProxyPort",     &data.ProxyPort    },
        {"ConfigPath",    &data.ConfigPath   }
    };

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto eqPos = line.find('=');
        if (eqPos == string::npos) continue;
        string key = line.substr(0, eqPos);
        string value = line.substr(eqPos + 1);
        if (!value.empty() && value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.size() - 2);
        }
        if (mp.count(key)) *mp[key] = value;
    }

    return data;
}

Data editdata(const Data& data, const string& filepath) {
    auto abs = absolute(filepath);

    FILE *f = fopen(filepath.c_str(), "w");
    if (!f) {
        throw runtime_error("Could not open data file for writing: " + abs.string() + " errno=" + to_string(errno));
    }

    int res = fprintf(f, "# Device Configuration\n");
    res += fprintf(f, "MDeviceHeight=\"%s\"\n", data.MDeviceHeight.c_str());
    res += fprintf(f, "MDeviceWidth=\"%s\"\n", data.MDeviceWidth.c_str());    
    res += fprintf(f, "DeviceHeight=\"%s\"\n", data.DeviceHeight.c_str());
    res += fprintf(f, "DeviceWidth=\"%s\"\n", data.DeviceWidth.c_str());
    res += fprintf(f, "DevicePort=\"%s\"\n", data.DevicePort.c_str());
    res += fprintf(f, "DevicePIN=\"%s\"\n", data.DevicePIN.c_str());
    res += fprintf(f, "DeviceIP=\"%s\"\n", data.DeviceIP.c_str());
    res += fprintf(f, "ApkX=\"%s\"\n", data.ApkX.c_str());
    res += fprintf(f, "ApkY=\"%s\"\n", data.ApkY.c_str());
    res += fprintf(f, "ProxyURL=\"%s\"\n", data.ProxyURL.c_str());
    res += fprintf(f, "ProxyPort=\"%s\"\n", data.ProxyPort.c_str());
    res += fprintf(f, "ConfigPath=\"%s\"\n", data.ConfigPath.c_str());

    if (fflush(f) != 0) {
        int err = errno;
        fclose(f);
        throw runtime_error("Failed to fflush file: " + abs.string() + " errno=" + to_string(err));
    }

    int fd = fileno(f);
    if (fd >= 0) {
        if (fsync(fd) != 0) {
            int err = errno;
            fclose(f);
            throw runtime_error("Failed to fsync file: " + abs.string() + " errno=" + to_string(err));
        }
    }

    fclose(f);
    return data;
}

Data resetdata(const string& filepath) {
    return editdata(loaddata("assets/backup.txt"), filepath);
}
