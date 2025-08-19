#include "project.hpp"

void App::stateAction() {
    switch (state) {

        case MAIN:
            switch (curr) {
                case 0: startConfig(); break;
                case 1: connectADB(); break;
                case 2: drawSettings(); break;
                default: running = false; break;
            } break;

        case SETTINGS:
            switch (curr) {
                case 0: viewData(); break;
                case 1: editData(); break;
                case 2: resetSettings(); break;
                case 3: editDefaults();  break;
                default: updateState(MAIN, mainOptions); break;
            } break;

        case VIEWDATA:
            switch (curr) {
                case 12: updateState(SETTINGS, settingsOptions); break;
                default: break;
            } break;

        case EDITDATA:
            switch (curr) {
                case  0: editMDeviceHeight(); break;
                case  1: editMDeviceWidth (); break;                
                case  2: editDeviceHeight (); break;
                case  3: editDeviceWidth  (); break;
                case  4: editDevicePort   (); break;
                case  5: editDevicePIN    (); break;
                case  6: editDeviceIP     (); break;
                case  7: editApkX         (); break;
                case  8: editApkY         (); break;
                case  9: editProxyURL     (); break;
                case 10: editProxyPort    (); break;
                case 11: editConfigPath   (); break;
                default: updateState(SETTINGS, settingsOptions); break;
            } break;
        
        case CONNECT: updateState(MAIN, mainOptions); break;

        default: break;
    }
}
