//
// Created by philip metzger on 22.09.2020.
//

#ifndef INSECUREWEBSERVER_SERVER_H
#define INSECUREWEBSERVER_SERVER_H
#include "base_config.h"
#include "utils/utils.h"
#include "main.h"
#include "db/database.h"
#include "log/logger.h"
#include "thread/webthread.h"

typedef struct server server;
typedef struct server_settings server_settings;

struct server_settings {
    const char *log_filename;
    const char *db_name;
    void (*panic_handler)(void);
    int port;
    int max_listeners;
    int db_settings;
    bool crlf_logs;
    bool use_child_processes;
    bool memory_db;
    bool multi_thread_logging;
    bool panic_on_failure;
    bool kill_with_leak;
};
server *server_default(void);
server *server_from_args(int argc,char **argv);
int server_start_default(server **default_server);
int server_restore_defaults(server **server);
server *server_from_settings(server_settings *settings);
int server_start(server **server, size_t max_listeners);
int server_create_default_db(server **server);
void server_stats(server **server);
void server_close(server **server);
int server_handle_incoming(server **server);
bool server_has_any_error(server **server);
void server_crash(server **server);
bool server_exit_from_cmd(server **server);

void server_log_error(const char *fmt,...);
void server_log_fatal(const char *fmt, ...);
void server_log_warn(const char *fmt,...);
void server_log_message(const char *fmt,...);
void server_log(uint8_t type, const char *fmt,...);
void server_message(const char *message);
void server_warning(const char *message);
void server_fatal(const char *message);
#endif//INSECUREWEBSERVER_SERVER_H
