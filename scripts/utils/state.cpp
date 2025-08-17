#include "project.hpp"

void App::stateAction() {
    switch (state) {

        case MAIN:
            switch (curr) {
                case 0: startConfig(); break;
                case 1: drawSettings(); break;
                default: running = false;}
            break;

        case SETTINGS:
            switch (curr) {
                case 0: viewData(); break;
                case 1: editData(); break;
                case 2: resetSettings(); break;
                case 3: editDefaults();  break;
                default: 
                    state = MAIN;
                    options = mainOptions;
                    curr = 0;
                    updateLog(curr);
                    refreshUI(curr); break;}
            break;

        case VIEWDATA:
            switch (curr) {
                case 6:
                    state = SETTINGS;
                    options = settingsOptions;
                    curr = 0;
                    updateLog(curr);
                    refreshUI(curr); break;
                default: break;}
            break;

        case EDITDATA:
            switch (curr) {
                case 0: editDevicePIN (); break;
                case 1: editDeviceIP  (); break;
                case 2: editDevicePort(); break;
                case 3: editProxyURL  (); break;
                case 4: editProxyPort (); break;
                case 5: editConfigPath(); break;
                default:
                    state = SETTINGS;
                    options = settingsOptions;
                    curr = 0;
                    updateLog(curr);
                    refreshUI(curr); break;}
            break;
        
        case CONNECT:
            if (!processing) {
                state = MAIN;
                options = mainOptions;
                curr = 0;
                updateLog(curr);
                refreshUI(curr);
            } else {
                int key = getKey();
                if (key == 'Q' || key == 27) {
                    processing = false;
                    shell::Shell shell;
                    shell.runShell("init", "0", "0");
                }
            }
            break;

        default: break;
    }
}