//
// Created by philip metzger on 22.09.2020.
//

#include "server.h"

#include "log/logger.h"
#include "mstring.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
struct server {
    logger default_log;
    database db;
    int *fds;
    web_thread *threads; //thread[n] is a reader thread[n&1] is a writer
    web_thread *db_thread;
    size_t num_threads;
    size_t num_fds;
    const size_t max_threads;
    const size_t max_fds;
    size_t pending_actions;
    int start_argc;
    const char **start_argv;
    bool kill_with_leak;
    bool panic_on_failure;
    bool crlf_logs;
};
// clang-format off
static server default_server = {
        .logger = default_log(),
        .db = default_db(),
        .max_threads = 5,
        .max_fds = 5,
        .threads = NULL,
        .fds = NULL,
        .panic_on_failure = true,
        .kill_with_leak = false,
        .pending_actions = 0,
        .start_argc = 0,
        .start_argv = NULL
};
// clang-format on
WEBSERVER_INTERNAL void register_atexit_handler(void (*handler)(void));
WEBSERVER_INTERNAL void register_atexit_handler(void (*handler)(void)) {
    atexit(handler);
}
WEBSERVER_INTERNAL void panic_handler(void);
WEBSERVER_INTERNAL void panic_handler(void) {
}
server *server_default(void) {
    register_atexit_handler(panic_handler);
    return &default_server;
}

server *server_from_args(int argc, char **argv) {
    /* Arguments from the command line always come in this format
     * argv[1] = name of the program
     * argv[2] = argument 1
     * argv[3] = argument 2
     * argv[n] = arg n
     * argv[n + 1] = 0
     */
    char *log_file;//InsecureWebserverLog
    char *db_name; //InsecureWebserver
    int port = 80; //Http
    for (int i = 0; i < argc; ++i) {
        printf("argc[%d] = %s\n", i, argv[i]);
    }
    if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-help"))) {
        display_help();
        exit(1);
        assert_unreachable();
    }
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "-port")) {
            char *endptr;
            port = strtol(argv[i + 1], &endptr, 10);
            if (port >= 65536) {
                log_error(&default_server.default_log, "Tried to use port '%d' which is greater than the port limit 65536", port);
                log_text(&default_server.default_log, "Setting port to 8080");
                port = 8080;
            }
        } else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "-logfile")) {
            log_file = argv[i + 1];
        } else if (!strcmp(argv[i], "-db") || !strcmp(argv[i], "-database")) {
            db_name = argv[i + 1];
        } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "-help")) {
            display_help();
            exit(0);
        }
    }
    if (db_name && log_file) {
        server_settings sets = {
                .log_filename = log_file,
                .port = port,
                .db_name = db_name,
                .panic_on_failure = true,
                .kill_with_leak = false,
                .crlf_logs = system_windows() ? true : false,
                .use_child_processes = false,
                .memory_db = false,
                .max_listeners = 8};
        return server_from_settings(&sets);
    }
    return server_default();
}


void server_log_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_errorv(&default_server.default_log, fmt, ap);
    va_end(ap);
}
void server_log_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_fatalv(&default_server.default_log, fmt, ap);
    va_end(ap);
}
void server_log_warn(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_warnv(&default_server.default_log, fmt, ap);
    va_end(ap);
}
void server_log_message(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_messagev(&default_server.default_log, fmt, ap);
    va_end(ap);
}
void server_log(uint8_t type, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_formatv(&default_server.default_log, type, fmt, ap);
    va_end(ap);
}
void server_message(const char *message) {
    log_text(&default_server.default_log, message);
}
void server_warning(const char *message) {
    log_warn(&default_server.default_log, message);
}
void server_fatal(const char *message) {
    log_fatal_message(&default_server.default_log, message);
}