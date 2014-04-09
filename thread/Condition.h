#ifndef _CONDITION_H_
#define _CONDITION_H_

#include "Mutex.h"
#include <boost/noncopyable.hpp>
#include <pthread.h>
#include <errno.h>

class Condition
	: public boost::noncopyable
{
public:
	explicit Condition(MutexLock& mutex)
		: m_Mutex(mutex)
	{
		pthread_cond_init(&m_PCond, NULL);
	}

	~Condition() {
		pthread_cond_destroy(&m_PCond);
	}

	void			wait() {
		pthread_cond_wait(&m_PCond, m_Mutex.getPthreadMutex());
	}

	bool			waitForSeconds(int nSeconds) {
		struct timespec abstime;
		clock_gettime(CLOCK_REALTIME, &abstime);
		abstime.tv_sec = nSeconds;
		return ETIMEDOUT == pthread_cond_timewait(&m_PCond, m_Mutex.getPthreadMutex(), &abstime);
	}

	void			notify() {
		pthread_cond_signal(&m_PCond);
	}

	void			notifyAll() {
		pthread_cond_broadcast(&m_PCond);
	}

private:
	MutexLock&		m_Mutex;
	pthread_cond_t	m_PCond;

};

#endif // _CONDITION_H_
