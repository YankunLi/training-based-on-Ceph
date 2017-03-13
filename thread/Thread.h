/*
 * Training based on Ceph
 *
 * Author: Yankun Li <lioveni99@gmail.com>
 *
 */
#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>
#include <sys/types.h>

namespace system_page
{
    extern unsigned _page_size;
    extern unsigned long _page_mask;
}

#define PAGE_MASK system_page::_page_mask

class Thread{
    private:
        pthread_t thread_id;
        pid_t pid;
        int ioprio_class, ioprio_priority;

        void *entry_wrapper();

    public:
        Thread(const Thread &other);
        Thread();
        virtual ~Thread(){}

    protected:
        virtual void *entry() = 0;

    private:
        static void *_entry_func(void *arg);

    public:
        void create(int stacksize = 0);
        int try_create(int stacksize);
        int kill(int signal);
        int join(void **retval = 0);

};

#endif
