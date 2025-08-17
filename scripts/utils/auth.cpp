#include "project.hpp"

using namespace std;
extern "C" int pam_conv_fn(int num_msg,
                           const struct pam_message** msg,
                           struct pam_response** resp,
                           void* appdata_ptr)
{
    if (num_msg <= 0 || !msg) return PAM_CONV_ERR;

    *resp = (pam_response*)calloc(static_cast<size_t>(num_msg), sizeof(struct pam_response));
    if (!*resp) return PAM_BUF_ERR;

    const char* password = static_cast<const char*>(appdata_ptr);

    for (int i = 0; i < num_msg; ++i) {
        if (!msg[i]) continue;
        (*resp)[i].resp_retcode = 0;
        switch (msg[i]->msg_style) {
            case PAM_PROMPT_ECHO_OFF:
            case PAM_PROMPT_ECHO_ON:
                (*resp)[i].resp = password ? strdup(password) : nullptr;
                if (!(*resp)[i].resp) {
                    for (int j = 0; j <= i; ++j) free((*resp)[j].resp);
                    free(*resp);
                    *resp = nullptr;
                    return PAM_BUF_ERR;
                }
                break;
            case PAM_TEXT_INFO:
                if (msg[i]->msg) cout << msg[i]->msg << el;
                (*resp)[i].resp = nullptr;
                break;
            case PAM_ERROR_MSG:
                if (msg[i]->msg) cerr << msg[i]->msg << el;
                (*resp)[i].resp = nullptr;
                break;
            default:
                (*resp)[i].resp = nullptr;
                break;
        }
    }
    return PAM_SUCCESS;
}
struct TermGuard {
    termios oldt{};
    bool active = false;
    void disable_echo() {
        if (tcgetattr(STDIN_FILENO, &oldt) != 0) return;
        termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        active = true;
    }
    ~TermGuard() {
        if (active) tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
};
static string read_password_for(const string& username) {
    string prompt = string("[sudo] password for ") + username + ": ";
    cout << prompt << flush;

    TermGuard tg;
    tg.disable_echo();

    string pw;
    if (!getline(cin, pw)) pw.clear();
    cout << el;
    return pw;
}
static int authenticate_once(const char* username, const char* password) {
    pam_handle_t* pamh = nullptr;
    struct pam_conv conv = { pam_conv_fn, const_cast<char*>(password) };

    int retval = pam_start("sudo", username, &conv, &pamh);
    if (retval != PAM_SUCCESS) {
        if (pamh)
            cerr << "pam_start failed: " << pam_strerror(pamh, retval) << el;
        else
            cerr << "pam_start failed (no pam handle): " << retval << el;
        return -1;
    }

    retval = pam_authenticate(pamh, 0);
    if (retval != PAM_SUCCESS) {
        pam_end(pamh, retval);
        return retval;
    }

    retval = pam_acct_mgmt(pamh, 0);
    pam_end(pamh, retval);
    return retval;
}

bool auth() {
    struct passwd* pw = getpwuid(getuid());
    string username;
    if (pw && pw->pw_name) username = pw->pw_name;
    else {
        const char* envu = getenv("USER");
        username = envu ? envu : "user";
    }

    const int max_attempts = 3;
    for (int attempt = 0; attempt < max_attempts; ++attempt) {
        string password = read_password_for(username);

        if (password.empty()) {

            if (attempt < max_attempts - 1) cout << "Sorry, try again." << el;

            fill(password.begin(), password.end(), 0);
            password.clear();
            continue;
        }

        int r = authenticate_once(username.c_str(), password.c_str());

        fill(password.begin(), password.end(), 0);
        password.clear();

        if (r == PAM_SUCCESS) {

            return true;
        } else if (r == -1) {

            return false;
        } else {

            if (attempt < max_attempts - 1) cout << "Sorry, try again." << el;

        }
    }
    return false;
}
