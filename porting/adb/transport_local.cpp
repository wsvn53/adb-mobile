/**
* transport_local.cpp is used to modify source code of the adb/client/transport_local.cpp file.
*/

#define local_init(...)     local_init_unused(__VA_ARGS__)

#include "adb/client/transport_local.cpp"

#undef local_init

void local_init(const std::string& addr) {
    D("transport: local client init");
    // disable client_socket_thread, seems not used in adb
    // std::thread(client_socket_thread, addr).detach();
    adb_local_transport_max_port_env_override();
}