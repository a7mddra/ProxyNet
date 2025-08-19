#include "project.hpp"

using namespace std;
namespace ui {

void clearScreen() {
    cout << "\033[H\033[2J";
}

void iflush() {
    cout << flush;
}

string readLine(const string &prompt) {
    if (!prompt.empty()) {
        cout << prompt;
        iflush();
    }
    string line;
    
    int old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    termios oldt{};
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
        
        if (!getline(cin, line)) line.clear();
        return line;
    }
    
    termios newt = oldt;
    newt.c_lflag |= (ICANON | ECHO);         
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    if (old_flags & O_NONBLOCK) {
        fcntl(STDIN_FILENO, F_SETFL, old_flags & ~O_NONBLOCK); 
    }
    
    if (!getline(cin, line)) line.clear();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, old_flags);

    return line;
}

string readPassword(const string &prompt) {
    if (!prompt.empty()) {
        cout << prompt;
        iflush();
    }
    string pw;
    
    int old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    termios oldt{};
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
        
        if (!getline(cin, pw)) pw.clear();
        cout << el;
        return pw;
    }
    
    termios newt = oldt;
    newt.c_lflag |= ICANON;      
    newt.c_lflag &= ~ECHO;       
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    if (old_flags & O_NONBLOCK) {
        fcntl(STDIN_FILENO, F_SETFL, old_flags & ~O_NONBLOCK);
    }

    if (!getline(cin, pw)) pw.clear();   
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, old_flags);
    cout << el;
    return pw;
}

void reply(const string& text, int del) {
    for (const char c : text) {
        cout << c;
        iflush();
        sleep_for(milliseconds(del));
    }
    cout << endl;
}

int utf8w(const string &str) {
    setlocale(LC_ALL, "");
    mbstate_t state{};
    const char *ptr = str.c_str();
    size_t len = str.size();
    int width = 0;

    while (len > 0) {
        wchar_t wc;
        size_t bytes = mbrtowc(&wc, ptr, len, &state);
        if (bytes == (size_t)-1 || bytes == (size_t)-2) break;
        if (bytes == 0) break;
        ptr += bytes;
        len -= bytes;

        int w = wcwidth(wc);
        width += (w > 0 ? w : 0);
    }
    return width;
}

size_t center(const string &l, bool a, char c, int del) {
    struct winsize wd{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wd);
    size_t w = (wd.ws_col > 0) ? wd.ws_col : 80;
    size_t padd = (w - utf8w(l)) / 2;

    string out = a ? "" : string(padd, ' ');
    switch (c) {
        case 'o': out += orange(l); break;
        case 'y': out += yellow(l); break;
        case 'g': out += green(l);  break;
        case 'b': out += blue(l);   break;
        case 'r': out += red(l);    break;
        default:  out += l;         break;
    }
    if (a) {
        cout << string(padd, ' ');
        reply(out, del);
        cout << el;
    } else {
        cout << out << el;
    }
    return padd;
}

int getKey() {
    char c;
    ssize_t n = read(STDIN_FILENO, &c, 1);
    if (n <= 0) return -1;

    if (c == '\033') {
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) <= 0) return 27;
        if (read(STDIN_FILENO, &seq[1], 1) <= 0) return 27;

        if (seq[0] == '[') {
            switch(seq[1]) {
                case 'A': return 'U'; 
                case 'B': return 'D'; 
                case 'C': return  -1;
                case 'D': return  -1;
            }
        }
        return 27;
    }

    switch(c) {
        case '\n': case '\r': return 'E'; 
        case 'q' : case 'Q' : return 'Q';   
        case 'y' : case 'Y' : return 'Y';   
        default: return c;
    }
}

} 