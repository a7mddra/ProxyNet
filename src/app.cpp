#include "project.hpp"

string el = "\n";
volatile sig_atomic_t g_winch = 0;
extern "C" void winch_handler(int) {g_winch = 1;}

App::App() {
    try {data = loaddata(DATA_FILE);}
    catch (const exception& e) {log = "Error loading data";}
}

void App::drawHeader() {
    for (size_t i=0; i<ASCII.size(); i++) 
        padd = center(ASCII[i], 0, 'r');
}

void App::drawLog(bool a, int del) {
    if (!log.empty()) 
        center(log, a, 'b', del);
}

void App::updateState(MenuState st, vector<string>& opt) {
    if (state == CONNECT) 
        Shell().runShell("reset", false);
    curr = 0;
    state = st;
    options = opt;
    updateLog(0);
    refreshUI();
}

void App::drawOptions(size_t curr) {
    for (size_t i = 0; i < options.size(); i++) 
        center(options[i], 0, i==curr?'g':'w');
}

void App::updateLog(size_t curr) {
    if (options[curr] == "Edit Defaults") 
        log = "Warning: this will overwrite factory defaults";
    else if (unreachable)
        log = "Please connect your phone via ADB first.";
    else if (state == CONNECT) 
        log = "Connecting 🖳 =    🗂";
    else if (mp.count(options[curr]))
        log = *mp[options[curr]].value;
    else log = LOG;
}

void App::refreshUI(size_t idx) {
    clearScreen();
    drawHeader();
    cout << el << el;
    if (log.empty()) log = LOG;
    drawLog(false);
    cout << el;
    drawOptions(idx);
}

void App::render() {
    clearScreen();
    TermiosGuard tg; 

    drawHeader();
    cout << el << el;
    drawLog(true, 40);
    drawOptions(0);

    curr = 0;
    while (running) { 
        if (g_winch) {
            g_winch = 0;
            refreshUI(curr);
        }
        int key = getKey(), op = options.size();
        if(!connecting.load()) switch (key) {
            case -1: continue; break;

            case 'U': curr = (curr-1+op) % op;
                      updateLog(curr); break;

            case 'D': curr = (curr+1) % op; 
                      updateLog(curr); break;

            case 'E': stateAction(); break;
            
            case 27 : updateState(MAIN, mainOptions); break;
            
            case 'Q': return;
            
            default: break; 

        } if (key == 'U' or key == 'D') refreshUI(curr);
    }
}

void App::run() {
    struct sigaction sa{};
    sa.sa_handler = winch_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGWINCH, &sa, nullptr);
    render();
}
