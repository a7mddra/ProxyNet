#pragma once
#include "headers.hpp"
#include "auth.hpp"

struct Eqauv {
    string name;
    string* value;
};

class App {
public:
    App();
    void run();
    bool sudo();
    void reset();
    void render();
    void unlock();
    void monitor();
    void drawHeader();
    void connectADB();
    void startConfig();
    void refreshUI(size_t idx=0);
    void updateLog(size_t curr);
    void drawOptions(size_t curr);
    void checkSplash(int x, int y);
    void drawLog(bool a, int del=75);
    string LOG = "Use ↑ ↓ to move, Enter ↵ to select";
    string log = LOG;
    
    enum MenuState { MAIN, SETTINGS, VIEWDATA, EDITDATA, CONNECT };
    void stateAction();
    void updateState(MenuState st, vector<string>& opt, size_t cur=0);
    
    Data data;
    size_t curr, padd;
    bool running = true;
    atomic<bool> processing{false};
    MenuState state = MAIN;
    string DATA_FILE  = "assets/data.txt";
    string BACKUP_FILE  = "assets/backup.txt";
    vector<string> ASCII={
        "       _    _____              _     _ ____                    ",
        "      / \\  |___  | __ ___   __| | __| |  _ \\ __ _              ",
        "     / _ \\    / / '_ ` _ \\ / _` |/ _` | |_) / _` |  ProxyNet   ",
        " / ___ \\  / /| | | | | | (_| | (_| |  _ < (_| |  ☠️  ☠️  ☠️ ",
        "   /_/   \\_\\/_/ |_| |_| |_|\\__,_|\\__,_|_| \\_\\__,_|             "
    };
    
    vector<string> mainOptions = 
    {"Proxy Net", "Settings", "Reset", "Exit"};
    vector<string> options = mainOptions;
    vector<string> connectOptions = {"Start Config", "Connect ADB", "Monitor", "Unlock", "Back"};
    vector<string> settingsOptions = 
    {"View Data", "Edit Data", "Reset Settings", "Edit Defaults", "Back"};
    vector<string> dataOptions = 
    {"MDeviceHeight", "MDeviceWidth", "DeviceHeight", "DeviceWidth",
        "DevicePort", "DevicePIN", "DeviceIP", "ApkX", "ApkY",
        "ProxyURL", "ProxyPort", "ConfigPath", "Back"};
    unordered_map<string, Eqauv> mp = {
        {"MDeviceHeight", {"MDeviceHeight", &data.MDeviceHeight}},
        {"MDeviceWidth",  {"MDeviceWidth",  &data.MDeviceWidth }},
        {"DeviceHeight",  {"DeviceHeight",  &data.DeviceHeight }},
        {"DeviceWidth",   {"DeviceWidth",   &data.DeviceWidth  }},        
        {"DevicePort",    {"DevicePort",    &data.DevicePort   }},
        {"DevicePIN",     {"DevicePIN",     &data.DevicePIN    }},
        {"DeviceIP",      {"DeviceIP",      &data.DeviceIP     }},
        {"ApkX",          {"ApkX",          &data.ApkX         }},
        {"ApkY",          {"ApkY",          &data.ApkY         }},
        {"ProxyURL",      {"ProxyURL",      &data.ProxyURL     }},
        {"ProxyPort",     {"ProxyPort",     &data.ProxyPort    }},
        {"ConfigPath",    {"ConfigPath",    &data.ConfigPath   }}
    };
    
private:
    void editMDeviceHeight();
    void editMDeviceWidth();
    void editDeviceHeight();
    void editDeviceWidth();
    void editDevicePort();
    void editConfigPath();
    void resetSettings();
    void editDevicePIN();
    void editProxyPort();
    void editDefaults();
    void editDeviceIP();
    void editProxyURL();
    void editApkX();
    void editApkY();
    bool inRange();
    void applyEdit(const string& field, string& target);
};
