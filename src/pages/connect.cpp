#include "project.hpp"

bool App::inRange() {
    Shell shell;
    if (shell.runShell("adb", false, data.DeviceIP, data.DevicePort) == "0") {
        log = "Please connect your phone via ADB first.";
        refreshUI(curr);
        return false;
    }   return true;
}

void App::reset() {
    Shell shell;
    processing.store(true);
    shell.runShell("reset", false, data.DeviceIP, data.DevicePort);
    log = "Reset Success.";
    refreshUI(curr);
    processing.store(false);
}

void App::monitor() {
    Shell shell;
    processing.store(true);
    bool vpn = shell.runShell("vpn", false) == "1";
    bool proxy = shell.runShell("proxy", false) == "1";
    log = string("VPN: ") + (vpn ? "Active" : "Down")
        + " | PROXY: " + (proxy ? "Active" : "Down");
    refreshUI(curr); processing.store(false); 
}

void App::unlock() {
    Shell shell;
    if (!inRange()) {return;} processing.store(true);
    if (shell.runShell("unlock", false, data.DevicePIN) == "0") {
        log = "Failed to unlock your phone.";
    } log = "Device unlocked.";
    refreshUI(curr); processing.store(false); 
}

void App::connectADB() {
    Shell shell;
    shell.runShell("adbd", true, data.DeviceIP, data.DevicePort);
}


void App::startConfig() {
    if (!inRange()) return;

    processing.store(true);

    const vector<string> anim = {
        " 🖳 =    🗂",
        " 🖳 ==   🗂",
        " 🖳  ==  🗂",
        " 🖳   == 🗂",
        " 🖳    ==🗂",
        " 🖳     =🗂"
    };

    thread([this, anim]() mutable {
    #ifdef __linux__
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(1, &cpuset);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    #endif

        size_t idx = 0, dir = 1;
        while (processing.load()) {
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
        {
            shell.runShell("init", false, data.ProxyURL, data.ProxyPort);
            if (shell.runShell("unlock", false, data.DevicePIN) == "0") {
                log = "Failed to unlock your phone.";
                refreshUI(curr);
                processing.store(false);
                return;
            }
        }
        {
            shell.runShell("launch", false, data.ConfigPath);
        }
        {
            int x = (stoi(data.ApkX) * stoi(data.DeviceWidth))  / stoi(data.MDeviceWidth);
            int y = (stoi(data.ApkY) * stoi(data.DeviceHeight)) / stoi(data.MDeviceHeight);
            checkSplash(x, y);
        }
        {
            shell.runShell("setup", false);
        }
        {
            log = "Connected 🖳 =====🗂";
            refreshUI(curr);
        }
        
        processing.store(false);
    }).detach();
}
