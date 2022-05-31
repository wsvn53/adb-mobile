/**
 * adb.cpp is used to replace original adb.cpp
 */

#include <stdio.h>
#include <pthread.h>

#define launch_server(...)		launch_server_unused(__VA_ARGS__)
#define update_transport_status(...)		update_transport_status_real(__VA_ARGS__)
#include "adb/adb.cpp"
#undef launch_server
#undef update_transport_status

void fdevent_reset_porting(void);
void clear_listener_list(void);
void clear_transport_list(void);

// replace exit to pthread_exit
void exit(int code) {
    pthread_exit(0);
}

void launch_server_main(int pipe_write, const char *thread_socket_spec) {
    // Launch server with pipe
    fcntl(pipe_write, F_SETFD, 0);
    printf("launch_server_main: %d\n", pipe_write);

    // We don't need to do clear or reset at first time
    static bool not_first = false;
    if (not_first == true) {
        clear_listener_list();
        clear_transport_list();
        fdevent_reset_porting();
    }
    not_first = true;

    // Start main server
    adb_server_main(false, thread_socket_spec, pipe_write);
}

int launch_server(const std::string& socket_spec) {
    static char *thread_socket_spec;
    static int pipe_server[2];

	printf("socket %s\n", socket_spec.c_str());
    thread_socket_spec = strdup(socket_spec.c_str());

    // Reset last used pipe
    pipe(pipe_server);

    // set up a pipe so the child can tell us when it is ready.
    if (pipe_server[0] < 0 || pipe_server[1] < 0) {
        fprintf(stderr, "pipe failed in launch_server, errno: %d\n", errno);
        return -1;
    }

    std::string path = android::base::GetExecutablePath();

    // launch server in new thread
    std::thread adb_thread([]() {
        launch_server_main(pipe_server[1], thread_socket_spec);
    });
    adb_thread.detach();

    // parent side
    printf("launch_server pipe_write: %d\n", pipe_server[0]);
    printf("launch_server pipe_read: %d\n", pipe_server[1]);
    
    // wait for the "OK\n" message from the child
    char temp[3] = {};
    int ret = adb_read(pipe_server[0], temp, 3);
    int saved_errno = errno;
    if (ret < 0) {
        fprintf(stderr, "could not read ok from ADB Server, errno = %d\n", saved_errno);
        return -1;
    }
    if (ret != 3 || temp[0] != 'O' || temp[1] != 'K' || temp[2] != '\n') {
        ReportServerStartupFailure(0);
        return -1;
    }

    return 0;
}

extern "C" {

__attribute__ ((weak))
void adb_connect_status_updated(const char *serial, const char *status) {
    printf("%s", __FUNCTION__);
}

}

void update_transport_status() {
    update_transport_status_real();

    // Check all the tcp transports connect status
    iterate_transports([](const atransport *t) {
        printf("adb connect status changed: %s -> %s\n", t->serial.c_str(), to_string(t->GetConnectionState()).c_str());
        adb_connect_status_updated(t->serial.c_str(), to_string(t->GetConnectionState()).c_str());
        return true;
    });
}
