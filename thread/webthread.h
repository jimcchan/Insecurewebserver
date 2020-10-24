//
// Created by philip metzger on 20.09.2020.
//


#ifndef INSECUREWEBSERVER_WEBTHREAD_H
#define INSECUREWEBSERVER_WEBTHREAD_H
#include "../base_config.h"


#include <stdbool.h>
#include <stdlib.h>
typedef struct web_thread web_thread;
/*TODO: Maybe implement a task based webthread  */
typedef void *(*thread_cb)(void *);
/**
 * Creates a OS specific thread
 */
web_thread *web_thread_create(thread_cb cb,void *thread_arg,thread_cb done_cb,void *done_arg);
/**
 * Returns the result of the done_cb
 */
void *web_thread_get_done(const web_thread **thread);
/**
 * Checks if the thread is reusable
 */
bool web_thread_is_reusable(const web_thread **thread);
/**
 * Joins a thread and writes its result into res
 */
bool web_thread_join(web_thread **thread,void *result);

bool web_thread_read(web_thread **thread,void *buf,size_t max);

bool web_thread_receive(web_thread **thread,void *data);
int web_thread_post(web_thread **thread);

#endif//INSECUREWEBSERVER_WEBTHREAD_H
