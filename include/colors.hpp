#pragma once
#include "headers.hpp"

namespace cr {
    
    inline string red(char c) 
        { return "\033[1;31m" + string(1, c) + "\033[0m"; }

    inline string green(char c) 
        { return "\033[38;2;68;198;98m" + string(1, c) + "\033[0m"; }

    inline string blue(char c) 
        { return "\033[38;2;13;146;244m" + string(1, c) + "\033[0m"; }

    inline string orange(char c) 
        { return "\033[38;2;255;102;0m" + string(1, c) + "\033[0m"; }

    inline string yellow(char c) 
        { return "\033[38;2;255;244;85m" + string(1, c) + "\033[0m"; }

    inline string red(const string& s) 
        { return "\033[1;31m" + s + "\033[0m"; }

    inline string green(const string& s) 
        { return "\033[1;32m" + s + "\033[0m"; }

    inline string blue(const string& s) 
        { return "\033[38;2;13;146;244m" + s + "\033[0m"; }

    inline string orange(const string& s) 
        { return "\033[38;2;255;102;0m" + s + "\033[0m"; }

    inline string yellow(const string& s) 
        { return "\033[38;2;255;244;85m" + s + "\033[0m"; }
}
