#include <algorithm>
#include <atomic>
#include <chrono>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <cerrno>
#include <cwchar>
#include <cstdio>
#include <ctime>
#include <errno.h>
#include <future>
#include <fcntl.h>
#include <fstream>
#include <functional>
#include <filesystem>
#include <iostream>
#include <locale>
#include <memory>
#include <mutex>
#include <pwd.h>
#include <string>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <sstream>
#include <stdexcept>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/ioctl.h>
#include <security/pam_appl.h>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#ifdef __linux__
#include <pthread.h>
#include <sched.h>
#endif

using namespace std;
using namespace chrono;
using namespace filesystem;
using namespace this_thread;
