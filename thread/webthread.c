//
// Created by philip metzger on 20.09.2020.
//


#include "webthread_internal.h"
#include "../server.h"

struct web_thread {
    os_thread os_thread;
    thread_cb current_callback;
    thread_cb done_callback;
    void *thread_result;
    void *done_result;
    void *thread_arg;
    void *done_arg;
    bool done;
};


bool web_thread_is_reusable(const web_thread **thread) {
    if (thread && *thread != NULL) {
        const web_thread *the = *thread;
        return the->done_result && the->done == true;
    }
    return false;
}
void *web_thread_get_result(const web_thread **thread) {
    if (thread && *thread != NULL) {
        const web_thread *the = *thread;
        return the->done_result;
    }
    return NULL;
}
web_thread *web_thread_create(thread_cb cb, void *thread_arg, thread_cb done_cb, void *done_arg) {
    if (!cb || !thread_arg)
        return NULL;
    int rc;
    web_thread *thread = malloc(sizeof(*thread));
    if (!thread) {
        server_error("Failed to create server thread");
        return NULL;
    }
    rc = os_thread_create(&thread->os_thread, NULL, done_cb, thread_arg);
    if (rc) {
        server_log_error("Failed to create %s thread ",OS_LINUX || OS_POSIX ? "Linux/Posix" : "Windows");
        return NULL;
    }
    web_thread s = {
            .current_callback = cb,
            .thread_arg = thread_arg,
            .done_arg = done_arg,
            .done_callback = done_cb,
            .done_result = NULL,
            .thread_result = NULL,
            .done = false
    };
    *thread = s;
    return thread;
}

bool web_thread_join(web_thread **thread,void *result) {
    if (thread && *thread != NULL) {
        web_thread *the = *thread;
        if (!the->done && !the->done_result)
            return false;
        if (the->done_callback)
            the->done_result = the->done_callback(the->done_arg);

        if (os_thread_join(&the->os_thread,&result))
            return false;
        return true;
    }
    return false;
}
