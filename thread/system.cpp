#include <sys/syscall.h>
#include <unistd.h>

#include "system.h"

pid_t get_pid()
{
    return syscall(SYS_gettid);
}
