#include "../Singleton.h"
#include "../ThreadLocal.h"
#include "../Thread.h"

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <stdio.h>

class Test
	: private boost::noncopyable
{
public:
	Test() {
		printf("tid = %d, constructing %p\n", CurrentThread::tid(), this);
	}

	~Test() {
		printf("tid = %d, destructing %p %s\n", CurrentThread::tid(), this, m_strName.c_str());
	}

	const std::string& name() const { return m_strName; }
	void setName(const std::string& n) { m_strName = n; }

private:
	std::string			m_strName;

};

#define STL Singleton<ThreadLocal<Test> >::instance().value()

void print() {
	printf("tid = %d, %p name = %s\n"
		, CurrentThread::tid()
		, &STL
		, STL.name().c_str());
}

void threadFunc(const char* pStrChangeTo) {
	print();
	STL.setName(pStrChangeTo);
	sleep(1);
	print();
}

int main() {
	STL.setName("main one");
	Thread t1(boost::bind(threadFunc, "thread1"));
	Thread t2(boost::bind(threadFunc, "thread2"));
	t1.start();
	t2.start();
	t1.join();
	print();
	t2.join();

	pthread_exit(0);
}
