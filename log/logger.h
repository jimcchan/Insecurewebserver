//
// Created by philip metzger on 20.09.2020.
//

#ifndef INSECUREWEBSERVER_LOGGER_H
#define INSECUREWEBSERVER_LOGGER_H
#include "../base_config.h"

#include "mstring.h"

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/*
 * error state of logger, either from errno or internally
 */
typedef struct error error;
typedef struct logger logger;
struct error {
    uint32_t sys_err;//errno
    uint32_t log_err;//any other error
};
/*
 * Writes the current mstring to the logfile, general logger
 */
struct logger {
    FILE *logfile;     //logfile, here lands cur_string
    uint64_t nwritten; //number of times written
    error log_error;   //errno or internal error
    mstring cur_string;//current string, after use reset
    bool crlf;         //write CRLF or just LF
};
#define LOG_WARN 1
#define LOG_ERROR 2
#define LOG_MESSAGE 3
#define LOG_FATAL 4

#define LOG_LINUX 1
#define LOG_WINDOWS 2
logger default_log(void);
void logger_init(FILE *logfile, char flags);
WEBSERVER_PRINTFLIKE(3, 4)
void log_format(logger *logger, char flags, const char *fmt, ...);
WEBSERVER_PRINTFLIKE(2, 3)
void log_warning(logger *logger, const char *fmt, ...);
WEBSERVER_PRINTFLIKE(2, 3)
void log_error(logger *logger, const char *fmt, ...);
WEBSERVER_PRINTFLIKE(2, 3)
void log_message(logger *logger, const char *fmt, ...);
WEBSERVER_PRINTFLIKE(2, 3)
void log_fatal(logger *logger, const char *fmt, ...);
void log_fatal_message(logger *log, const char *message);
void log_errorv(logger *log, const char *fmt, va_list ap);
void log_warnv(logger *log, const char *fmt, va_list ap);
void log_fatalv(logger *log, const char *fmt, va_list ap);
void log_messagev(logger *log, const char *fmt, va_list ap);
void log_formatv(logger *log, uint8_t type, const char *fmt, va_list ap);
bool log_last_error(logger *log);
void log_text(logger *log, const char *text);
void log_warn(logger *log, const char *text);
void log_error_message(logger *log, const char *text);

#endif//INSECUREWEBSERVER_LOGGER_H
