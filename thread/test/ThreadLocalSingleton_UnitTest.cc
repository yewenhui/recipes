#include "../ThreadLocalSingleton.h"
#include "../Thread.h"

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <stdio.h>

class Test
	: public boost::noncopyable
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

void threadFunc(const char* pStrChangeTo) {
	printf("tid = %d, %p name = %s\n"
		, CurrentThread::tid()
		, &ThreadLocalSingleton<Test>::instance()
		, ThreadLocalSingleton<Test>::instance().name().c_str());

	ThreadLocalSingleton<Test>::instance().setName(pStrChangeTo);

	printf("tid = %d, %p name = %s\n"
		, CurrentThread::tid()
		, &ThreadLocalSingleton<Test>::instance()
		, ThreadLocalSingleton<Test>::instance().name().c_str());

	ThreadLocalSingleton<Test>::destroy();
}

int main()
{
	ThreadLocalSingleton<Test>::instance().setName("main one");
	Thread t1(boost::bind(threadFunc, "thread1"));
	Thread t2(boost::bind(threadFunc, "thread2"));
	t1.start();
	t2.start();
	t1.join();

	printf("tid = %d, %p name = %s\n"
		, CurrentThread::tid()
		, &ThreadLocalSingleton<Test>::instance()
		, ThreadLocalSingleton<Test>::instance().name().c_str());

	t2.join();
}


