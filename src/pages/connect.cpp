#include "project.hpp"
#define run shell.runShell

void App::startConfig() {
    state = CONNECT;
    options = {"Cancel"};
    curr = 0;
    refreshUI(curr);
    
    Shell shell;
    steady_clock clk;
    atomic<bool> init_done(false);
    atomic<bool> init_success(false);
    atomic<bool> canceled(false);
    
    void cancel([&]() {
        run("reset");
        sleep(1);
        processing = false;
        canceled = true;
        init_done = true;
        state = MAIN;
        options = mainOptions;
        curr = 0;
        updateLog(curr);
        refreshUI(curr);
        return;
    });

    auto init_future = async(launch::async, [&]() {
        auto start = clk.now();
        while (!init_done) {
            bool success = run("init", data.DeviceIP, data.DevicePort, 
                              data.ProxyURL, data.ProxyPort) == "1";
                              
            if (success) {
                init_success = true;
                init_done = true;
                return true;
            }
            
            if (clk.now() - start > 30s) {
                log = "Connection timed out";
                init_done = true;
                return false;
            }
            
            sleep_for(milliseconds(1500));
            
            if (canceled) cancel();
        }
        return false;
    });
    
    int it = 0;
    int dir = 1;
    int attempt = 1;

    while (!init_done) {
        string status;
        switch (it) {
            case 0: status = "Connecting 🖳 =    🗂"; break;
            case 1: status = "Connecting 🖳 ==   🗂"; break;
            case 2: status = "Connecting 🖳  ==  🗂"; break;
            case 3: status = "Connecting 🖳   == 🗂"; break;
            case 4: status = "Connecting 🖳    ==🗂"; break;
            case 5: status = "Connecting 🖳     =🗂"; break;
        }

        log = status;
        refreshUI(curr);
        
        int key = getKey();
        if (key == 'E' || key == 'Q' || key == 27) cancel();
        
        if (key == 'R') attempt++;

        it += dir;

        if (it > 4) dir = -1;
        else if (it <= 0) dir = 1;

        sleep_for(milliseconds(150));
    }
    
    init_future.wait();
    processing = init_success;

    if (canceled) cancel();
    
    auto last_refresh = clk.now();
    while(processing) {
        auto now = clk.now();
        
        bool connect = run("vpn") == "1";
        bool share = run("proxy") == "1";
        
        if (connect && share) {
            log = "✓ VPN & Proxy: ACTIVE";
        } else if (connect) {
            log = "✓ VPN: ACTIVE, ✗ Proxy: DOWN";
        } else if (share) {
            log = "✗ VPN: DOWN, ✓ Proxy: ACTIVE";
        } else {
            log = "✗ VPN & Proxy: DOWN";
        }
        
        if (duration_cast<milliseconds>(now - last_refresh) > 100ms) {
            refreshUI(curr);
            last_refresh = now;
        }
        
        if (!connect) run("connect");
        if (!share) run("share");
        
        int key = getKey();
        if (key == 'E' || key == 'Q' || key == 27) cancel();
        
        sleep_for(milliseconds(100));
    }
    
    cancel();
}
