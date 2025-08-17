#include "project.hpp"

void App::drawSettings() {
    state = SETTINGS;
    options = settingsOptions;
    curr = 0;
    updateLog(curr);
    refreshUI(curr);
}

void App::viewData() {
    state = VIEWDATA;
    options = editdataOptions;
    curr = options.size() - 1;
    refreshUI(curr);
}

void App::resetSettings() {
    log = "Do you want to reset to factory defaults? [y]";
    refreshUI(curr);
    if (getKey()=='Y') {
        try { data = resetdata(DATA_FILE); log = "Data reset to defaults."; } 
        catch (const exception &e)  { log = "Reset failed"; }
        refreshUI(curr);
    }
}

void App::editData() {
    state = EDITDATA;
    options = editdataOptions;
    curr = options.size() - 1;
    refreshUI(curr);
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
    if (getKey()=='Y') {
        if (!sudo()) {
            log = "You need to run this as root to edit defaults.";
            state = SETTINGS;
            options = settingsOptions;
            refreshUI(curr);
            return;
        }
        try { data = editdata(data, SUDO_FILE); log = "Factory defaults updated."; } 
        catch (const exception &e)  { log = "Reset failed"; }
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

void App::editDevicePIN()    { applyEdit("Device PIN",    data.DevicePIN);  }
void App::editDeviceIP()     { applyEdit("Device IP",     data.DeviceIP);   }
void App::editDevicePort()   { applyEdit("Device Port",   data.DevicePort); }
void App::editProxyURL()     { applyEdit("Proxy URL",     data.ProxyURL);   }
void App::editProxyPort()    { applyEdit("Proxy Port",    data.ProxyPort);  }
void App::editConfigPath()   { applyEdit("Config Path",   data.ConfigPath); }
