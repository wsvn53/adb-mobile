/**
* adb_io.cpp is used to handle ReadExactly with select which can be used to control thread exit
*/

#define ReadFdExactly(...)  ReadFdExactly_unused(__VA_ARGS__)

#include "adb/adb_io.cpp"

#undef ReadFdExactly
#undef read

// return bool to indicated whether continue to read
bool read_select(borrowed_fd fd) {
    char name[10];
    memset(name, 0, sizeof(name));
    pthread_getname_np(pthread_self(), name, sizeof(name));
    if (strlen(name) == 0) {
        return true;
    }

    std::string thread_name = name;
    // thread_name split by ':'
    size_t prefix_pos = thread_name.find("adb:");
    if (prefix_pos == std::string::npos) {
        // not prefix with 'adb:', continue to read with adb_read
        return true;
    }

    // split fds
    thread_name.erase(0,4);
    std::string thread_fd_str = thread_name.substr(0, thread_name.find(":"));
    int thread_fd = atoi(thread_fd_str.c_str());
    if (thread_fd <= 0) return true;

    // select read fds
    // fd_set with select
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(thread_fd, &fds);
    FD_SET(fd.get(), &fds);

    while (1) {
        int ret = select(FD_SETSIZE, &fds, NULL, NULL, NULL);
        if (ret == 0) {
            continue;
        }

        if (FD_ISSET(thread_fd, &fds)) {
            char thread_cmd;
            size_t n = read(thread_fd, &thread_cmd, 1);
            if (n > 0 && thread_cmd == 'Q') {
                pthread_exit((void *)0);
                return false;
            }
            printf("unknown thread command: %c\n", thread_cmd);
        }

        if (FD_ISSET(fd.get(), &fds)) {
            return true;
        }
    }

    return false;
}

bool ReadFdExactly(borrowed_fd fd, void* buf, size_t len) {
    char* p = reinterpret_cast<char*>(buf);

    size_t len0 = len;

    D("readx: fd=%d wanted=%zu", fd.get(), len);
    while (len > 0) {
        if (!read_select(fd)) {
            return false;
        }

        int r = adb_read(fd, p, len);
        if (r > 0) {
            len -= r;
            p += r;
        } else if (r == -1) {
            D("readx: fd=%d error %d: %s", fd.get(), errno, strerror(errno));
            return false;
        } else {
            D("readx: fd=%d disconnected", fd.get());
            errno = 0;
            return false;
        }
    }

    VLOG(RWX) << "readx: fd=" << fd.get() << " wanted=" << len0 << " got=" << (len0 - len) << " "
              << dump_hex(reinterpret_cast<const unsigned char*>(buf), len0);

    return true;
}


