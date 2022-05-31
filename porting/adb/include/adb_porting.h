#ifndef ADB_PORTING
#define ADB_PORTING 1

#define __android_log_print(...)   porting_log_print(__VA_ARGS__)
#define __android_log_buf_print(...)   porting_log_buf_print(__VA_ARGS__)
#define main(...)   porting_main_diabled(__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

void adb_trace_init(char**);
void adb_trace_enable_porting(int trace_tag);
int adb_commandline(int argc, const char** argv);
void usb_init(void);
void usb_cleanup(void);
int porting_log_print(int p, const char *tag, const char *fmt, ...);
int porting_log_buf_print(int bufID, int prio, const char* tag, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
