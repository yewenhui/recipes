#ifndef _BOUNDED_BLOCKING_QUEUE_H_
#define _BOUNDED_BLOCKING_QUEUE_H_

#include "Condition.h"

#include <boost/circular_buffer.hpp>
#include <boost/noncopyable.hpp>
#include <assert.h>

template<typename T>
class BoundedBlockingQueue
	: private boost::noncopyable
{
public:
	explicit BoundedBlockingQueue(int nMaxSize)
		: m_Mutex()
		, m_NotEmptyCond(m_Mutex)
		, m_NoeFullCond(m_Mutex)
		, m_Queue(nMaxSize)
	{
	}

	void put(const T& t) {
		MutexLockGuard lock(m_Mutex);
		while (m_Queue.full()) {
			m_NoeFullCond.wait();
		}
		assert(!m_Queue.full());
		m_Queue.push_back(t);
		m_NotEmptyCond.notify();
	}

	T take() {
		MutexLockGuard lock(m_Mutex);
		while (m_Queue.empty()) {
			m_NotEmptyCond.wait();
		}
		assert(!m_Queue.empty());
		T front(m_Queue.front());
		m_Queue.pop_front();
		m_NoeFullCond.notify();
		return front;
	}

	bool empty() const {
		MutexLockGuard lock(m_Mutex);
		return m_Queue.empty();
	}

	bool full() const {
		MutexLockGuard lock(m_Mutex);
		return m_Queue.full();
	}

	size_t size() const {
		MutexLockGuard lock(m_Mutex);
		return m_Queue.size();
	}

	size_t capacity() const {
		MutexLockGuard lock(m_Mutex);
		return m_Queue.capacity();
	}

private:
	mutable MutexLock		m_Mutex;
	Condition				m_NotEmptyCond;
	Condition				m_NoeFullCond;
	boost::circular_buffer<T> m_Queue;

};

#endif // _BOUNDED_BLOCKING_QUEUE_H_