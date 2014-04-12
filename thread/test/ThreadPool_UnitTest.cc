#include "../ThreadPool.h"
#include "../CountDownLatch.h"

#include <boost/bind.hpp>
#include <stdio.h>

void print() {
	printf("tid = %d\n", CurrentThread::tid());
}

void printString(const std::string& strName) {
	printf("tid = %d, str = %s\n", CurrentThread::tid(), strName.c_str());
}

int main()
{
	ThreadPool threadPool("MainThreadPool");
	threadPool.start(5);

	threadPool.run(print);
	threadPool.run(print);

	for (int n = 0; n < 100; ++n) {
		char buf[32];
		snprintf(buf, sizeof(buf), "task %d", n);
		threadPool.run(boost::bind(printString, std::string(buf)));
	}
	
	CountDownLatch latch(1);
	threadPool.run(boost::bind(&CountDownLatch::countDown, &latch));
	latch.wait();
	threadPool.stop();
}
