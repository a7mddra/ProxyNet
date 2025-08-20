#include "project.hpp"

namespace sh {

Shell::Shell() : outputCallback(nullptr) {}
Shell::~Shell() {}

string Shell::execute(const string& command) {
    buffer.clear();
    int status = runCommand(command);
    if (status != 0) return "Command failed: " + to_string(status);
    return cleanOutput(buffer);
}

void Shell::setOutputCallback(function<void(const string&)> callback) {
    outputCallback = callback;
}


string Shell::cleanOutput(const string& output) {
    size_t end = output.find_last_not_of(" \n\r\t");
    return (end == string::npos) ? string() : output.substr(0, end + 1);
}

int Shell::runCommand(const string& full_command) {
    vector<string> argv;
    argv.push_back("/bin/sh");
    argv.push_back("-c");
    argv.push_back(full_command);
    return runCommandVec(argv);
}

int Shell::runCommandVec(const vector<string>& argv) {
    
    buffer.clear();
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;
    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    } if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);
        vector<char*> cargv;
        cargv.reserve(argv.size() + 1);
        for (const auto &s : argv) cargv.push_back(const_cast<char*>(s.c_str()));
        cargv.push_back(nullptr);
        execv(cargv[0], cargv.data());
        _exit(127);
    } else {
        close(pipefd[1]);
        int flags = fcntl(pipefd[0], F_GETFL, 0);
        if (flags != -1) fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);
        char buf[1024];
        ssize_t count;
        while (true) {
            count = read(pipefd[0], buf, sizeof(buf) - 1);
            if (count > 0) {
                buf[count] = '\0';
                buffer += buf;
                if (outputCallback) outputCallback(string(buf, count));
            } else if (count == 0) break;
            else {
                if (errno == EAGAIN || errno == EWOULDBLOCK) { } 
                else  break;
            } usleep(10000);
        } close(pipefd[0]);
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) return WEXITSTATUS(status);
        return -1;
    }
}
}