#pragma once
#include "headers.hpp"

namespace sh {

class Shell {
public:
    Shell();
    ~Shell();
    string execute(const string& command);
    void setOutputCallback(function<void(const string&)> callback);
    template <typename... Args>
    string runShell(const string& scriptName, bool inNewTerminal, Args&&... args) {
        buffer.clear();
        path scriptPath = absolute("scripts/shell/" + scriptName + ".sh");
        string script = scriptPath.string();
        const string allowedPrefix = absolute("scripts/shell").string();

        if (script.rfind(allowedPrefix, 0) != 0) 
            return "Security violation: Invalid script path";
        
        if (!exists(script)) 
            return "Script not found: " + script;
        
        permissions(script,
            perms::owner_exec | perms::group_exec | perms::others_exec,
            perm_options::add);
            
        vector<string> argv;
        argv.reserve(1 + sizeof...(Args));
        argv.push_back(script); 
        
        (argv.push_back(string(forward<Args>(args))), ...);

        if (inNewTerminal) {
            string realUser = execute("logname");  
            realUser = cleanOutput(realUser);

            string dbusAddr = "unix:path=/run/user/$(id -u " + realUser + ")/bus";
            string cmd = "sudo -u " + realUser +
                        " DBUS_SESSION_BUS_ADDRESS=" + dbusAddr +
                        " gnome-terminal -- bash -c '";

            for (size_t i = 0; i < argv.size(); ++i) {
                cmd += argv[i];
                if (i + 1 < argv.size()) cmd += " ";
            }

            cmd += "; exec bash'";

            system(cmd.c_str());
            return "";
        }

        int status = runCommandVec(argv);
        if (status != 0) 
            return "Command failed: " + to_string(status);
        
        size_t last_newline = buffer.find_last_of('\n');
        if (last_newline != string::npos && last_newline != buffer.size()-1) 
            return buffer.substr(last_newline + 1);
        
        return cleanOutput(buffer);
    }


private:
    int runCommandVec(const vector<string>& argv);
    int runCommand(const string& full_command);
    string cleanOutput(const string& output);
    string buffer;
    function<void(const string&)> outputCallback;
};

} 