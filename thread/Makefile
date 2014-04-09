CXXFLAGS=-g -Wall 
CXXFLAGS+=-O2

HEADERS = $(wildcard *.h)
TESTS = \
		atomic_unittest \
		thread_unittest \
		singleton_unittest

all: $(TESTS)

$(TESTS): $(HEADERS)

$(TESTS):
	g++ $(CXXFLAGS) -o $@ $(filter %.cc, $^) $(LDFLAGS) -lpthread

atomic_unittest: test/Atomic_UnitTest.cc
thread_unittest: test/Thread_UnitTest.cc Thread.cc
singleton_unittest: test/Singleton_UnitTest.cc Thread.cc
singleton_unittest: CXXFLAGS += -ftemplate-depth-1000

clean:
	rm -f $(TESTS)
