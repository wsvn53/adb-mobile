/**
* transport.cpp is used to modify source code of adb/transport.cpp
*/

#define init_reconnect_handler(...)     init_reconnect_handler_real(__VA_ARGS__)

#include "adb/transport.cpp"

#undef init_reconnect_handler

void init_reconnect_handler(void) {
    // only init reconnect handler only once
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        init_reconnect_handler_real();
    }
}

void clear_transport_list(void) {
    std::lock_guard<std::recursive_mutex> lock(transport_lock);
    for (auto t : transport_list) {
        t->Kick();
    }
    transport_list.clear();
    pending_list.clear();
}