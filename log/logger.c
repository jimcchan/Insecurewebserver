//
// Created by philip metzger on 20.09.2020.
//

#include "logger.h"
#include "mstring.h"

#define LOG_TTY_NORMAL " Message: "
#define LOG_TTY_ERROR "\033[1;31m Error: \033[0m;"
#define LOG_TTY_WARN "\033[1;33m Warning: \033[0m; "
#define LOG_TTY_FATAL "\033[1;31m Fatal Error: \033[0m; "

#include <stdarg.h>
#include <time.h>
WEBSERVER_INTERNAL void append_current_time(logger *log);
WEBSERVER_INTERNAL void append_current_time(logger *log) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    static char timebuf[40] = {0};
    if (strftime(timebuf, sizeof(timebuf), "[%H:%M:%S]", tm) != 18)
        return;
    if (mstring_init(&log->cur_string, timebuf) != MSTRING_OK)
        return;
}

WEBSERVER_INTERNAL bool logger_write(logger *log);
WEBSERVER_INTERNAL bool logger_write(logger *log) {
    flockfile(log->logfile);
    if (fputs(mstring_text(&log->cur_string), log->logfile) < 0) {
        funlockfile(log->logfile);
        return false;
    }
    funlockfile(log->logfile);
    mstring_reset(&log->cur_string);
    return true;
}
WEBSERVER_PRINTFLIKE(3, 4)
void log_format(logger *, char flags, const char *fmt, ...) {
    bool tty = isatty(fileno(log->logfile));
    append_current_time(log);
    switch (flags) {
        case LOG_MESSAGE:
            if (mstring_append_text(&log->cur_string, tty ? LOG_TTY_NORMAL : "Message: ") != MSTRING_OK)
                return;
            break;
        case LOG_WARN:
            if (mstring_append_text(&log->cur_string, tty ? LOG_TTY_WARN : "Warning: ") != MSTRING_OK)
                return;
            break;
        case LOG_ERROR:
            if (mstring_append_text(&log->cur_string, tty ? LOG_TTY_ERROR : "Error: ") != MSTRING_OK)
                return;
            break;
        case LOG_FATAL:
            if (mstring_append_text(&log->cur_string, tty ? LOG_TTY_FATAL : "Fatal Error: ") != MSTRING_OK)
                return;
            break;
        default:
            assert_unreachable();
            break;
    }
    va_list ap;
    va_start(ap, fmt);
    numbytes = mstring_vappendf(&log->cur_string, fmt, ap);
    va_end(ap);
    rc = log->crlf ? mstring_append(&log->cur_string, "\r\n") : mstring_append_c(&log->cur_string, '\n', 1);
    if (rc != MSTRING_OK)
        return;
    if (!logger_write(log))
        return;
}
WEBSERVER_PRINTFLIKE(2, 3)
void log_warning(logger *log, const char *fmt, ...) {
    size_t rc;
    append_current_time(log);
    va_list ap;
    va_start(ap, fmt);
    rc = mstring_vappendf(&log->cur_string, fmt, ap);
    va_end(ap);
    if (!logger_write(log))
        return;
}
WEBSERVER_PRINTFLIKE(2, 3)
void log_error(logger *log, const char *fmt, ...) {
    size_t rc;
    append_current_time(log);
    va_list va;
    va_start(va, fmt);
    rc = mstring_vappendf(&log->cur_string, fmt, ap);
    va_end(va);
    if (!logger_write(log))
        return;
}
WEBSERVER_PRINTFLIKE(2, 3)
void log_message(logger *log, const char *fmt, ...) {
    size_t rc;
    append_current_time(log);
    va_list ap;
    va_start(va, fmt);
    rc = mstring_vappendf(&log->cur_string, fmt, ap);
    va_end(va);
    if (!logger_write(log))
        return;
}

WEBSERVER_PRINTFLIKE(2, 3)
void log_fatal(logger *log, const char *fmt, ...) {
    size_t rc;
    append_current_time(log);
    va_list ap;
    va_start(ap, fmt);
    rc = mstring_vappendf(&log->cur_string, fmt, ap);
    va_end(ap);
    if (!logger_write(log))
        return;
}

logger default_log(void) {
    logger log = {
            .crlf = false,
            .nwritten = 0,
            .log_error = {0},
            .logfile = fopen("InsecureWebLog", "rw+")
    };
    assert(log.logfile);
    return log;
}