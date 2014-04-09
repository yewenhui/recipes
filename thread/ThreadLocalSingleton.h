#ifndef _THREAD_LOCAL_SINGLETON_H_
#define _THREAD_LOCAL_SINGLETON_H_

#include <boost/noncopyable.hpp>

template<typename T>
class ThreadLocalSingleton
	: public boost::noncopyable
{
public:
	static T&		instance() {
		if (NULL == m_pValue) {
			m_pValue = new T();
		}
		return *m_pValue;
	}

	static void		destroy() {
		if (NULL != m_pValue) {
			delete m_pValue;
			m_pValue = NULL;
		}
	}

private:
	ThreadLocalSingleton();
	~ThreadLocalSingleton();

	static __thread T*	m_pValue;

};

template<typename T>
__thread T* ThreadLocalSingleton<T>::m_pValue = NULL;

#endif // _THREAD_LOCAL_SINGLETON_H_