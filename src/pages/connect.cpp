#include "project.hpp"

void App::startConfig() {
    Shell shell;

    auto isreachable_local = [&]() -> bool {
        string res = shell.runShell("adb", false, data.DeviceIP, data.DevicePort);
        if (res == "0") {
            unreachable = true;
            updateState(MAIN, mainOptions);
            return false;
        }
        unreachable = false;
        return true;
    };

    if (!isreachable_local()) return;
    updateState(CONNECT, connectOptions);

    const vector<string> anim = {
        " 🖳 =    🗂",
        " 🖳 ==   🗂",
        " 🖳  ==  🗂",
        " 🖳   == 🗂",
        " 🖳    ==🗂",
        " 🖳     =🗂"
    };

    connecting.store(true);


    thread([this, anim]() mutable {
    #ifdef __linux__
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(1, &cpuset);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    #endif
        size_t idx = 0;
        int dir = 1;
        while (connecting.load()) {
            log = "Connecting " + anim[idx];
            refreshUI(curr);
            if (dir == 1) {
                if (idx + 1 < anim.size()) idx++;
                else { dir = -1; if (idx > 0) idx--; }
            } else {
                if (idx > 0) idx--;
                else { dir = 1; if (anim.size() > 1) idx++; }
            }
            sleep_for(milliseconds(120));
        } refreshUI(curr);

    }).detach();

    
    thread([this]() mutable {
    #ifdef __linux__
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    #endif
        Shell shell;
        shell.runShell("init", false, data.ProxyURL, data.ProxyPort);
        if (shell.runShell("unlock", false, data.DevicePIN) == "0") {
            log = "Failed to unlock your phone.";
            refreshUI(curr);
            connecting.store(false);
            return;
        }

        shell.runShell("launch", false, data.ConfigPath);

        int x = (stoi(data.ApkX) * stoi(data.DeviceWidth))  / stoi(data.MDeviceWidth);
        int y = (stoi(data.ApkY) * stoi(data.DeviceHeight)) / stoi(data.MDeviceHeight);
        checkSplash(x, y);

        log = "done";
        refreshUI(curr);
        
        connecting.store(false);
    }).detach();
}
