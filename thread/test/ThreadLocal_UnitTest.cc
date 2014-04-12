#include "../ThreadLocal.h"
#include "../Thread.h"

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

ThreadLocal<Test> testObj1;
ThreadLocal<Test> testObj2;

void print()
{
	printf("tid = %d, obj1 %p name = %s\n"
		, CurrentThread::tid()
		, &testObj1.value()
		, testObj1.value().name().c_str());
	printf("tid = %d, obj2 %p name = %s\n"
		, CurrentThread::tid()
		, &testObj2.value()
		, testObj2.value().name().c_str());
}

void threadFunc()
{
	print();
	testObj1.value().setName("changed 1");
	testObj2.value().setName("changed 42");
	print();
}

int main()
{
	testObj1.value().setName("main one");
	print();

	Thread t1(threadFunc);
	t1.start();
	t1.join();
	testObj2.value().setName("main two");
	print();

	pthread_exit(0);
}
