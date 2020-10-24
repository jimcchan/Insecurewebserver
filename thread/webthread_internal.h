//
// Created by philip metzger on 20.09.2020.
//

#ifndef INSECUREWEBSERVER_WEBTHREAD_INTERNAL_H
#define INSECUREWEBSERVER_WEBTHREAD_INTERNAL_H
#include "webthread.h"//Module external declarations
#if defined(OS_LINUX) || defined(OS_UNIX)
#include "pthread.h"
typedef pthread_t os_thread;
typedef pthread_attr_t os_thread_attr;
#else//!OS_LINUX || !OS_UNIX
typedef HANDLE os_thread;
typedef HANDLE os_thread_attr;
#endif

os_thread os_thread_create(os_thread *, os_thread_attr *attr, thread_cb cb, void *arg);
int os_thread_join(const os_thread *thread, void **result);


bool os_listen(os_thread *thread,int port);

size_t os_accept(os_thread *thread,void *buf_into,size_t bufsize);
size_t os_send(os_thread *thread,void *buf,size_t bufsiez);


#endif//INSECUREWEBSERVER_WEBTHREAD_INTERNAL_H
