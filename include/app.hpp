#pragma once
#include "headers.hpp"
#include "auth.hpp"

struct Eqauv {
    std::string name;
    std::string* value;
};

class App {
public:
    App();
    void run();
    bool sudo();
    void render();
    void drawHeader();
    void startConfig();
    void drawSettings();
    void refreshUI(size_t idx);
    void updateLog(size_t curr);
    void drawOptions(size_t curr);
    void drawLog(bool a, int del=75);
    string LOG = "Use ↑ ↓ to move, Enter to select";
    string log = LOG;

    enum MenuState { MAIN, SETTINGS, VIEWDATA, EDITDATA, CONNECT };
    void stateAction();
    
    Data data;
    size_t curr, padd;
    bool running = true, processing = false;
    MenuState state = MAIN;
    string DATA_FILE  = "assets/data.txt";
    string SUDO_FILE  = "assets/sudo.txt";
    vector<string> ASCII={
        "       _    _____              _     _ ____                    ",
        "      / \\  |___  | __ ___   __| | __| |  _ \\ __ _              ",
        "     / _ \\    / / '_ ` _ \\ / _` |/ _` | |_) / _` |  ProxyNet   ",
        " / ___ \\  / /| | | | | | (_| | (_| |  _ < (_| |  ☠️  ☠️  ☠️ ",
        "   /_/   \\_\\/_/ |_| |_| |_|\\__,_|\\__,_|_| \\_\\__,_|             "
    };
    vector<string> options = 
    {"Start config", "Settings", "Exit"};
    vector<string> mainOptions = 
    {"Start config", "Settings", "Exit"};
    vector<string> settingsOptions = 
    {"View Data", "Edit Data", "Reset Settings", "Edit Defaults", "Back"};
    vector<string> editdataOptions = 
    {"DevicePIN", "DeviceIP", "DevicePort", "ProxyURL", "ProxyPort", "ConfigPath", "Back"};
    
    unordered_map<string, Eqauv> mp = {
        {"DevicePIN",    {"DevicePIN",    &data.DevicePIN}},
        {"DeviceIP",     {"DeviceIP",     &data.DeviceIP}},
        {"DevicePort",   {"DevicePort",   &data.DevicePort}},
        {"ProxyURL",     {"ProxyURL",     &data.ProxyURL}},
        {"ProxyPort",    {"ProxyPort",    &data.ProxyPort}},
        {"ConfigPath",   {"ConfigPath",   &data.ConfigPath}}
    };
    
private:
    void viewData();
    void editData();
    void resetSettings();
    void editDefaults();
    
    void applyEdit(const string& field, string& target);
    void editDevicePIN();
    void editDeviceIP();
    void editDevicePort();
    void editProxyURL();
    void editProxyPort();
    void editConfigPath();
};