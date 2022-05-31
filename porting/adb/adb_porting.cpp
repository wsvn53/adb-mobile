/**
 * adb_porting.cpp used to define some functions to c
 */
#include <stdio.h>
#include <stdarg.h>
#include "adb_trace.h"

int porting_log_print(int p, const char *tag, const char *fmt, ...) {
    va_list args_list;
    va_start(args_list, fmt);
    int ret = vprintf(fmt, args_list);
    va_end(args_list);

    return ret;
}

int porting_log_buf_print(int bufID, int prio, const char* tag, const char* fmt, ...) {
    va_list args_list;
    va_start(args_list, fmt);
    int ret = vprintf(fmt, args_list);
    va_end(args_list);

    return ret;
}

void usb_init() {
    return;
}

void usb_cleanup() {
    return;
}

void adb_trace_enable_porting(int trace_tag) {
    adb_trace_enable((AdbTrace)trace_tag);
}

void abort() {
    pthread_exit(NULL);
}
