#include "project.hpp"

void App::checkSplash(int x, int y) {
    Shell shell;
    int idx = 1;
    string name = to_string(idx);

    shell.runShell("shot", false, name);

    string splashColor = shell.runShell("color", false, name, to_string(x), to_string(y));

    while (true) {
        ++idx;
        name = to_string(idx);
        shell.runShell("shot", false, name);
        string currentColor = shell.runShell("color", false, name, to_string(x), to_string(y));
        if (currentColor != splashColor) {
            shell.runShell("clear", false);
            return;
        }
        sleep_for(milliseconds(100));
    }
}
