/**
* adb_io.cpp is used to handle ReadExactly with select which can be used to control thread exit
*/

//#define ReadFdExactly(...)  ReadFdExactly_unused(__VA_ARGS__)

//#include <map>
#include "adb/adb_io.cpp"

//#undef ReadFdExactly
//#undef read
//
//// create map to store thread unique_fd
//std::vector<unique_fd*> thread_pipe_list() {
//    static std::vector<unique_fd*> pipe_list;
//    return pipe_list;
//}
//
//// save pipe fd
//void thread_pipe_list_save(unique_fd *pipe_fd) {
//    thread_pipe_list().push_back(pipe_fd);
//}
//
//void thread_pipe_list_remove(unique_fd *pipe_fd) {
//    int i = 0;
//    for (auto fd : thread_pipe_list()) {
//        if (pipe_fd->get() == pipe_fd->get()) {
//            // remove from vector
//            thread_pipe_list().erase(thread_pipe_list().begin() + i);
//            break;
//        }
//        i++;
//    }
//}
//
//// close all thread io
//void adb_shutdown_read_threads() {
//    for (auto pipe_fd : thread_pipe_list()) {
//        printf("sending quit control message to io thread pipe: %d\n", pipe_fd->get());
//        adb_write(pipe_fd->get(), "Q", 1);
//    }
//    thread_pipe_list().clear();
//}
//
//// return bool to indicated whether continue to read
//bool read_select(borrowed_fd fd, int ctrl_fd) {
//    // select read fds
//    // fd_set with select
//    fd_set fds;
//    FD_ZERO(&fds);
//    FD_SET(ctrl_fd, &fds);
//    FD_SET(fd.get(), &fds);
//
//    do {
//        int ret = select(FD_SETSIZE, &fds, NULL, NULL, NULL);
//        if (ret == 0) {
//            continue;
//        }
//
//        if (FD_ISSET(ctrl_fd, &fds)) {
//            printf("quit control message received, quit read thread\n");
//            pthread_exit((void *)0);
//        }
//
//        if (FD_ISSET(fd.get(), &fds)) {
//            return true;
//        }
//    } while (true);
//
//    return false;
//}
//
//bool ReadFdExactly(borrowed_fd fd, void* buf, size_t len) {
//    char* p = reinterpret_cast<char*>(buf);
//
//    size_t len0 = len;
//
//    D("readx: fd=%d wanted=%zu", fd.get(), len);
//
//    // Create pipes to control thread exit
//    unique_fd pipe_read, pipe_write;
//    if (!Pipe(&pipe_read, &pipe_write)) {
//        printf("pipe failed for create thread control, errno: %d\n", errno);
//        return -1;
//    }
//
//    // Save pipe to thread io map
//    thread_pipe_list_save(&pipe_write);
//
//    while (len > 0) {
//        if (!read_select(fd, pipe_read.get())) {
//            thread_pipe_list_remove(&pipe_write);
//            return false;
//        }
//
//        int r = adb_read(fd, p, len);
//        if (r > 0) {
//            len -= r;
//            p += r;
//        } else if (r == -1) {
//            D("readx: fd=%d error %d: %s", fd.get(), errno, strerror(errno));
//            thread_pipe_list_remove(&pipe_write);
//            return false;
//        } else {
//            D("readx: fd=%d disconnected", fd.get());
//            errno = 0;
//            thread_pipe_list_remove(&pipe_write);
//            return false;
//        }
//    }
//
//    VLOG(RWX) << "readx: fd=" << fd.get() << " wanted=" << len0 << " got=" << (len0 - len) << " "
//              << dump_hex(reinterpret_cast<const unsigned char*>(buf), len0);
//
//    thread_pipe_list_remove(&pipe_write);
//    return true;
//}
