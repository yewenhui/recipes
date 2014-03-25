#ifndef THREAD_H
#define THREAD_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <pthread.h>

#include "Atomic.h"

class Thread
	: public boost::noncopyable
{
public:
	typedef boost::function<void()> ThreadFunc;

	explicit Thread(const ThreadFunc& func, const std::string& name = std::string());
	~Thread();

	void start();
	void join();

	bool started() const { return m_bStarted; }
	pid_t tid() const { return *m_tIdPtr; }
	const std::string& name() const { return m_strName; }

	static int getNumCreated() { return m_AtomicNumCreated.get(); }

private:
	bool		m_bStarted;
	bool		m_bJoined;
	pthread_t	m_threadId;
	boost::shared_ptr<pid_t> m_tIdPtr;
	ThreadFunc	m_ThreadFunc;
	std::string	m_strName;

	static AtomicInt32 m_AtomicNumCreated;

};

namespace CurrentThread
{
	pid_t tid();
	const char* name();
	bool isMainThread();

}

#endif // THREAD_H