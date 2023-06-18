#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <ctime>
#include <sys/stat.h>
#include <sys/time.h>

#include "print.h"

using namespace std;

int print_level = 4;

const char* get_current_time() {
    struct timeval tmnow{};
    gettimeofday(&tmnow, nullptr);
    time_t rawtime = tmnow.tv_sec;
    struct tm* tm = localtime(&rawtime);

    char time_buf[30];
    char usec_buf[10];

    strftime(time_buf, 30, "%y%m%d%H%M%S", tm);
    snprintf(usec_buf, 10, "%03d", static_cast<int>(tmnow.tv_usec / 1000));

    static char timestamp[40];
    snprintf(timestamp, 40, "%s%s", time_buf, usec_buf);

    return timestamp;
}

void print(int level, const char *format, ...)
{
    va_list args;
    char buf[256];
    char time_buf[30];
    char usec_buf[10];
    char log_file_path[256];
    struct stat st{};
    struct timeval tmnow{};
    struct tm *tm;

    FILE *log;

    int ret;

    if (level <= print_level) {

        sprintf(log_file_path, "%s%s%s", LOG_FILE_PATH, LOG_FILE_NAME, LOG_FILE_EXTENSION);

        va_start(args, format);
        vsnprintf(buf, sizeof(buf), format, args);
        va_end(args);

        ret = stat(log_file_path, &st);

        gettimeofday(&tmnow, nullptr);
        time_t rawtime = tmnow.tv_sec;
        tm = localtime(&rawtime);
        strftime(time_buf, 30, "%Y.%m.%d %H:%M:%S.", tm);
        snprintf(usec_buf, 10, "%03d", (int)tmnow.tv_usec / 1000); // ms
        strcat(time_buf, usec_buf);

        if (st.st_size > LOG_SIZE || ret) {
            if (ret == 0) {
                char old_name[256];
                sprintf(old_name, "%s%s_%s%s", LOG_FILE_PATH, LOG_FILE_NAME, get_current_time(), LOG_FILE_EXTENSION);
                remove(old_name);
                rename(log_file_path, old_name);
            }
            log = fopen(log_file_path, "w");
        } else {
            log = fopen(log_file_path, "a+");
        }

        if (log) {
            fprintf(log, "[%s]: %s\n", time_buf, buf);
            fclose(log);
        }
    }
}