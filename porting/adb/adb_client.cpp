/**
* adb_client.cpp used to fix adb_set_socket_spec function
*/

extern "C" {
bool adb_kill_server_porting(void);
}

#define adb_set_socket_spec(...)    adb_set_socket_spec_unused(__VA_ARGS__)
#define adb_check_server_version(...)    adb_check_server_version_unused(__VA_ARGS__)

#include "adb/client/adb_client.cpp"

#undef adb_set_socket_spec
#undef adb_check_server_version

bool adb_check_server_version(std::string* _Nonnull error) {
    // If this is first time be called, ignore, because __adb_server_socket_spec not ready
    static bool first_call = true;
    if (first_call) {
        first_call = false;
        return true;
    }

    // Make adb check server version can be called more than once
    static std::once_flag once;
    static bool result;
    static std::string* err;
    err = new std::string();
    result = __adb_check_server_version(err);
    *error = *err;
    return result;
}

void adb_set_socket_spec(const char* socket_spec) {
    if (__adb_server_socket_spec) {
        // prevent fatal error here, just return
        return;
    }
    __adb_server_socket_spec = socket_spec;
}
