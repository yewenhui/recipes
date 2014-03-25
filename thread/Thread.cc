#include "Thread.h"

#include <boost/weak_ptr.hpp>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace CurrentThread 
{
__thread const char* t_threadName = "unknown";
__thread pid_t t_cachedTid = 0;

pid_t tid() {
	return static_cast<pid_t>(::syscall(SYS_getttid));
}

}

