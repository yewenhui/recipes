#include "../BoundedBlockingQueue.h"
#include "../CountDownLatch.h"
#include "../Thread.h"

#include <string>
#include <stdio.h>
#include <assert.h>

#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

class Test
{
public:
	Test(int nNumThreads)
		: m_Queue(20) 
		, m_Latch(nNumThreads)
		, m_vThread(nNumThreads)
	{
		for (int n = 0; n < nNumThreads; ++n) {
			char name[32];
			snprintf(name, sizeof(name), "work thread %d", n);
			m_vThread.push_back(new Thread(boost::bind(&Test::threadFunc, this), std::string(name)));
		}
		for_each(m_vThread.begin(), m_vThread.end(), boost::bind(&Thread::start, _1));
	}

	void run(int nTimes) {
		if (nTimes <= 0) {
			assert(false);
			return;
		}

		printf("waiting for count down latch\n");
		m_Latch.wait();
		printf("all threads started\n");
		for (int n = 0; n < nTimes; ++ n) {
			char buf[32];
			snprintf(buf, sizeof(buf), "hello %d", n);
			m_Queue.put(buf);
			printf("tid = %d, put data = %s, size = %zd\n"
				, CurrentThread::tid(), buf, m_Queue.size());
		}
	}

	void joinAll() {
		for (size_t n = 0; n < m_vThread.size(); ++n) {
			m_Queue.put("stop");
		}
		for_each(m_vThread.begin(), m_vThread.end(), boost::bind(&Thread::join, _1));
	}

private:
	void threadFunc() {
		printf("tid = %d, %s started\n", CurrentThread::tid(), CurrentThread::name());
		m_Latch.countDown();
		bool bRunning = true;
		while (bRunning) {
			std::string str(m_Queue.take());
			printf("tid = %d, get data = %s, size = %zd\n", CurrentThread::tid()
				, str.c_str(), m_Queue.size());

			bRunning = (str != "stop");
		}

		printf("tid = %d, %s stoped\n", CurrentThread::tid(), CurrentThread::name());
	}

private:
	BoundedBlockingQueue<std::string> m_Queue;
	CountDownLatch				m_Latch;
	boost::ptr_vector<Thread>	m_vThread;

};

int main()
{
	printf("pid = %d, tid = %d\n", ::getpid(), CurrentThread::tid());
	Test t(5);
	t.run(100);
	t.joinAll();

	printf("number of created threads %d\n", Thread::getNumCreated());
}
