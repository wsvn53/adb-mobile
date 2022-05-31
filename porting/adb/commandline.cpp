/**
* commandline.cpp is use to handle command outputs
*/
#include <stdio.h>
#include <string.h>

int printf_hijack(const char *format, ...);
size_t fwrite_hijack(const void *__ptr, size_t __size, size_t __nitems, FILE *__stream);
int fprintf_hijack(FILE *file, const char *format, ...);
[[noreturn]] void error_exit_hijack(const char* fmt, ...);

#define printf(...)  printf_hijack(__VA_ARGS__)
#define fwrite(...)  fwrite_hijack(__VA_ARGS__)
#define fprintf(...) fprintf_hijack(__VA_ARGS__)
#define error_exit(...)  error_exit_hijack(__VA_ARGS__);

#include "adb/client/commandline.cpp"
#include "adb/client/line_printer.cpp"

#undef printf
#undef fwrite

// Declare global variable to catch outputs
static std::string adb_commandline_stdout = "";
void append_commandline_stdout(char *text) {
    static pthread_mutex_t adb_append_stdout_lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&adb_append_stdout_lock);
    adb_commandline_stdout.append(text);
    pthread_mutex_unlock(&adb_append_stdout_lock);
}

extern "C" {
int adb_commandline_porting(int argc, const char** argv, char **message);
char *adb_commandline_last_output();
}

bool adb_check_server_version(std::string* _Nonnull error);
int adb_commandline_porting(int argc, const char** argv, char **message) {
    // Empty stdout handler
    adb_commandline_stdout = "";

    // Trigger adb_check_server_version once before adb_connect
    // Because adb_check_server_version only be called once, hacked here
    static std::string *error;
    error = new std::string();
    if (!adb_check_server_version(error)) {
        *message = strdup(error->c_str());
        return -1;
    }

    // Execute adb commandline
    int ret = adb_commandline(argc, argv);
    *message = strdup(adb_commandline_stdout.c_str());
    if (ret != 0 && strlen(*message) == 0) {
        *message = strdup("adb_commandline failed");
    }
    return ret;
}

int printf_hijack(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int size = vsnprintf(NULL, 0, format, args);
    char text[size+1];
    vsprintf(text, format, args);
    text[size] = '\0';
    va_end(args);
    append_commandline_stdout(text);
    return 0;
}

size_t fwrite_hijack(const void *__ptr, size_t __size, size_t __nitems, FILE *__stream) {
    if (__ptr == NULL || strlen((char *)__ptr) == 0) {
        return 0;
    }
    char text[__nitems+1];
    strncpy(text, (char *)__ptr, __nitems);
    text[__nitems] = '\0';
    append_commandline_stdout(text);
    return 0;
}

int fprintf_hijack(FILE *file, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int size = vsnprintf(NULL, 0, format, args);
    char text[size+1];
    vsprintf(text, format, args);
    text[size] = '\0';
    va_end(args);
    append_commandline_stdout(text);
    return 0;
}

void error_exit_hijack(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int size = vsnprintf(NULL, 0, fmt, args);
    char text[size+1];
    vsprintf(text, fmt, args);
    text[size] = '\0';
    va_end(args);
    append_commandline_stdout(text);

    // finally exit thread
    pthread_exit(NULL);
}

// when adb command errors occur, thread will exit without return code and message
// so we need a function to get last ouput
char *adb_commandline_last_output() {
    return strdup(adb_commandline_stdout.c_str());
}
