#include "Thread.h"

#include <boost/weak_ptr.hpp>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace CurrentThread 
{
__thread const char* t_threadName = "unknown";

}

namespace
{
__thread pid_t t_cachedTid = 0;

pid_t gettid() {
	return static_cast<pid_t>(::syscall(SYS_gettid));
}

// void afterFork() {
// 	t_cachedTid = gettid();
// 	CurrentThread::t_threadName = "main";
// }
//
// class ThreadNameInitializer
// {
// public:
// 	ThreadNameInitializer() {
// 		CurrentThread::t_threadName = "main";
// 		pthread_atfork(NULL, NULL, &afterFork);
// 	}
// };
// 
// ThreadNameInitializer init;

struct ThreadData {
	typedef Thread::ThreadFunc ThreadFunc;
	ThreadFunc func;
	std::string name;
	boost::weak_ptr<pid_t> wkTidPtr;

	ThreadData(const ThreadFunc& func
			, const std::string& name
			, const boost::shared_ptr<pid_t>& tidPtr)
		: func(func), name(name), wkTidPtr(tidPtr)
	{
	}

	void runInThread() {
		pid_t tid = CurrentThread::tid();
		boost::shared_ptr<pid_t> ptid = wkTidPtr.lock();

		if (ptid) {
			*ptid = tid;
			ptid.reset();
		}

		CurrentThread::t_threadName = name.c_str();
		func();
		CurrentThread::t_threadName = "finished";
	}

};

void* startThread(void* obj) {
	ThreadData* pData = static_cast<ThreadData*>(obj);
	pData->runInThread();
	delete pData;
	return NULL;
}

}

pid_t CurrentThread::tid() {
	if (0 == t_cachedTid) {
		t_cachedTid = gettid();
	}
	return t_cachedTid;
}

const char* CurrentThread::name() {
	return t_threadName;
}

bool CurrentThread::isMainThread() {
	return tid() == ::getpid();
}

AtomicInt32 Thread::m_AtomicNumCreated;

Thread::Thread(const ThreadFunc& func, const std::string& name /*= std::string()*/) 
	: m_bStarted(false)
	, m_bJoined(false)
	, m_threadId(0)
	, m_tIdPtr(new pid_t(0))
	, m_ThreadFunc(func)
	, m_strName(name)
{
	m_AtomicNumCreated.increment();
}

Thread::~Thread() {
	if (m_bJoined && !m_bJoined) {
		pthread_detach(m_threadId);
	}
}

void Thread::start() {
	assert(!m_bStarted);
	m_bStarted = true;

	ThreadData* pData = new ThreadData(m_ThreadFunc, m_strName, m_tIdPtr);
	if (pthread_create(&m_threadId, NULL, &startThread, pData)) {
		m_bStarted = false;
		delete pData;
		abort();
	}
}

void Thread::join() {
	assert(m_bStarted);
	assert(!m_bJoined);
	m_bJoined = false;
	pthread_join(m_threadId, NULL);
}
