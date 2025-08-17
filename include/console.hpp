#pragma once
#include "headers.hpp"

namespace ui {
    int getKey();
    void iflush();
    void clearScreen();
    void reply(const string& text, int del = 50);
    string readLine(const string &prompt = "");
    string readPassword(const string &prompt = "");
    size_t center(const string &l, bool a, char c='w', int del=75);
    
    struct TermiosGuard {
        struct termios oldt;
        int old_flags;
        
        TermiosGuard() {
            tcgetattr(STDIN_FILENO, &oldt);
            old_flags = fcntl(STDIN_FILENO, F_GETFL);
            
            struct termios newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);
        }
        
        ~TermiosGuard() { 
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            fcntl(STDIN_FILENO, F_SETFL, old_flags);
        }
    };
}