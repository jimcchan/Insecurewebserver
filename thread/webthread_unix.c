//
// Created by philip metzger on 20.09.2020.
//
#include "webthread_internal.h"

#include <stdbool.h>

#if defined(OS_LINUX) || defined(OS_UNIX)


os_thread os_thread_create(os_thread *os,os_thread_attr *attr,thread_cb cb,void *arg) {
    if (!os || !cb || !arg)
        return -1;
    return pthread_create(os,attr,cb,arg);

}
int os_thread_join(const os_thread *thread,void **result) {
    if (!thread || !result)
        return -1;
    return pthread_join(*thread,result);
}


#endif