/**
* fdevent.cpp is used to modify source code of adb/fdevent.cpp
*/

#define fdevent_destroy(...)       fdevent_destroy_real(__VA_ARGS__)
#define protected                  public

#include "adb/fdevent/fdevent.cpp"

#undef fdevent_destroy

unique_fd fdevent_destroy(fdevent* fde) {
    // Use custom destroy function to avoid CHECK_EQ error
    // and avoid CheckMainThread error
    if (!fde) {
        return {};
    }

    fdevent_get_ambient()->Unregister(fde);

    unique_fd fd = std::move(fde->fd);

    auto erased = fdevent_get_ambient()->installed_fdevents_.erase(fd.get());
    return fd;
}

void fdevent_reset_porting() {
    fdevent_get_ambient()->main_thread_id_ = android::base::GetThreadId();
    fdevent_reset();
}