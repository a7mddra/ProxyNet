#include "project.hpp"

void App::connectADB() {
    Shell shell;
    shell.runShell("adbd", true, data.DeviceIP, data.DevicePort);
}
