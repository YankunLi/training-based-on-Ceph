/*
 * Training based on Ceph
 *
 * Author: Yankun Li <lioveni99@gmail.com>
 *
 */

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "Thread.h"
#include "system.cpp"

namespace system_page
{
    unsigned _page_size = sysconf(_SC_PAGESIZE);
    unsigned long _page_mask = ~(unsigned long)(_page_size - 1);
}

//Thread::Thread(const Thread &other)
//{}

Thread::Thread()
    :thread_id(0),
    pid(0),
    ioprio_class(-1),
    ioprio_priority(-1)
{
}

void Thread::create(int stacksize)
{
    int ret;
    ret = try_create(stacksize);
    if(ret != 0)
    {
        exit(-1);
    }

}

int Thread::try_create(int stacksize)
{
    pthread_attr_t *thread_attr = NULL;
    pthread_attr_t thread_attr_loc;

    stacksize &= PAGE_MASK;
    if(stacksize)
    {
        thread_attr = &thread_attr_loc;
        pthread_attr_init(thread_attr);
        pthread_attr_setstacksize(thread_attr, stacksize);
    }

    int r;
    r = pthread_create(&thread_id, thread_attr, _entry_func, (void*) this);

    if(thread_attr)
    {
        pthread_attr_destroy(thread_attr);
    }

    return r;
}

int Thread::kill(int signal)
{
    if(thread_id)
    {
        return pthread_kill(thread_id, signal);
    }
    else
    {
        return -1;
    }
}

int Thread::join(void **retval)
{
    if(thread_id == 0)
    {
        return -1;
    }
    int ret = pthread_join(thread_id, retval);
    if(ret != 0)
    {
        return -1;
    }
    return ret;

}

void *Thread::entry_wrapper()
{
    int p = get_pid();
    if(p > 0)
        pid = p;
    return entry();

}

void *Thread::_entry_func(void *arg)
{
    void *r = ((Thread*)arg)->entry_wrapper();
    return r;
}

