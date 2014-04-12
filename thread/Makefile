CXXFLAGS=-g -Wall 
CXXFLAGS+=-O2

HEADERS = $(wildcard *.h)
TESTS = boundedblockingqueue_unitTest \
#		blockingqueue_unittest \
#		threadpool_unittest \
#		factory_unittest \
#		copyandwrite_unitTest \
#		observersafe_unittest \
#		atomic_unittest \
#		thread_unittest \
#		singleton_unittest \
#		threadlocal_unittest \
#		threadlocalsingleton_unitest \
#		singletonthreadlocal_unittest \
#		exception_unittest \
#		observer_unittest \

all: $(TESTS)

$(TESTS): $(HEADERS)

$(TESTS):
	g++ $(CXXFLAGS) -o $@ $(filter %.cc, $^) $(LDFLAGS) -lpthread

atomic_unittest: test/Atomic_UnitTest.cc
thread_unittest: test/Thread_UnitTest.cc Thread.cc

singleton_unittest: test/Singleton_UnitTest.cc Thread.cc
singleton_unittest: CXXFLAGS += -ftemplate-depth-1000

threadlocal_unittest: test/ThreadLocal_UnitTest.cc Thread.cc
threadlocalsingleton_unitest: test/ThreadLocalSingleton_UnitTest.cc Thread.cc

singletonthreadlocal_unittest: test/SingletonThreadLocal_UnitTest.cc Thread.cc
exception_unittest: test/Exception_UnitTest.cc Exception.cc

observer_unittest: test/Observer.cc
observersafe_unittest: test/ObserverSafe.cc Thread.cc
factory_unittest: test/Factory.cc Thread.cc

copyandwrite_unitTest: test/CopyAndWrite_UnitTest.cc Thread.cc

threadpool_unittest: test/ThreadPool_UnitTest.cc ThreadPool.cc Exception.cc Thread.cc
blockingqueue_unittest: test/BlockingQueue_UnitTest.cc Thread.cc
boundedblockingqueue_unitTest: test/BoundedBlockingQueue_UnitTest.cc Thread.cc

clean:
	rm -f $(TESTS)
