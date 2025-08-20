#include "project.hpp"

void App::resetSettings() {
    log = "Do you want to reset to factory defaults? [y]";
    refreshUI(curr);
    int key = -1;
    while (true) {
        key = getKey();
        if (key == -1 || key == 'E') {
            sleep_for(milliseconds(40));
            continue;
        } break;
    } if (key == 'Y') {
        try { data = resetdata(DATA_FILE); log = "Data reset to defaults."; } 
        catch (const exception &e)  { log = "Reset failed"; }
        refreshUI(curr);
    } else {
        log = "Reset canceled.";
        refreshUI(curr);
    }
}

bool App::sudo() {
    log = " ";
    refreshUI(curr);
    bool ok = auth();
    if (ok) return true;
    else {
        log = "Authentication failed";
        refreshUI(curr);
        return false;
    }
}

void App::editDefaults() {
    log = "Do you want to save your current data as factory defaults? [y]";
    refreshUI(curr);
    int key = -1;
    while (true) {
        key = getKey();
        if (key == -1 || key == 'E') {
            sleep_for(milliseconds(40));
            continue;
        } break;
    } if (key == 'Y') {
        if (!sudo()) {
            log = "You need to run this as root to edit defaults.";
            state = SETTINGS;
            options = settingsOptions;
            refreshUI(curr);
            return;
        } try { data = editdata(data, BACKUP_FILE); log = "Factory defaults updated."; } 
        catch (const exception &e)  { log = "Reset failed"; }
        refreshUI(curr);
    } else {
        log = "Reset canceled.";
        refreshUI(curr);
    }
}

void App::applyEdit(const string& field, string& target) {
    log = "Provide new " + field + " (press enter to cancel):";
    refreshUI(curr);
    string prompt = string(padd, ' ') + " >";
    string input = readLine(orange(prompt));
    if (!input.empty()) {
        target = input;
        try {
            editdata(data, DATA_FILE);
            log = field + " updated and saved.";
        } catch (const exception &e) {log = "Failed to save changes.";}
    } else log = "No input provided.";
    refreshUI(curr);
}

void App::editMDeviceHeight() { applyEdit("MDevice Height", data.MDeviceHeight);}
void App::editMDeviceWidth()  { applyEdit("MDevice Width",  data.MDeviceWidth); }
void App::editDeviceHeight()  { applyEdit("Device Height",  data.DeviceHeight); }
void App::editDeviceWidth()   { applyEdit("Device Width",   data.DeviceWidth);  }
void App::editDevicePort()    { applyEdit("Device Port",    data.DevicePort);   }
void App::editDevicePIN()     { applyEdit("Device PIN",     data.DevicePIN);    }
void App::editDeviceIP()      { applyEdit("Device IP",      data.DeviceIP);     }
void App::editApkX()          { applyEdit("ApkX",           data.ApkX);         }
void App::editApkY()          { applyEdit("ApkY",           data.ApkY);         }
void App::editProxyURL()      { applyEdit("Proxy URL",      data.ProxyURL);     }
void App::editProxyPort()     { applyEdit("Proxy Port",     data.ProxyPort);    }
void App::editConfigPath()    { applyEdit("Config Path",    data.ConfigPath);   }
