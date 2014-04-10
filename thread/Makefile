CXXFLAGS=-g -Wall 
CXXFLAGS+=-O2

HEADERS = $(wildcard *.h)
TESTS = \
		atomic_unittest \
		thread_unittest \
		singleton_unittest \
		threadlocal_unittest \
		threadlocalsingleton_unitest \
		singletonthreadlocal_unittest \
		exception_unittest \
		observer_unittest

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

clean:
	rm -f $(TESTS)
