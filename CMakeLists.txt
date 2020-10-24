cmake_minimum_required(VERSION 3.16)
project(InsecureWebServer C)

set(CMAKE_C_STANDARD 99)

add_executable(InsecureWebServer main.c log/logger.c thread/webthread.c thread/webthread.h base_config.h thread/webthread_unix.c thread/webthread_win.c thread/webthread_internal.h db/database.c db/database.h http/http.c http/http.h server.c server.h utils/utils.c utils/utils.h main.h utils/blob.c utils/blob.h)