#include "../Atomic.h"
#include <assert.h>

int main()
{
	AtomicInt32 a0;
	assert(a0.get() == 0);
	assert(a0.getAndAdd(1) == 0);
	assert(a0.get() == 1);
	assert(a0.addAndGet(2) == 3);
	assert(a0.get() == 3);
	assert(a0.incrementAndGet() == 4);
	assert(a0.get() == 4);
	assert(a0.getAndSet(100) == 4);
	assert(a0.get() == 100);

	AtomicInt64 a1;
	assert(a1.get() == 0);
	assert(a1.getAndAdd(1) == 0);
	assert(a1.get() == 1);
	assert(a1.addAndGet(2) == 3);
	assert(a1.get() == 3);
	assert(a1.incrementAndGet() == 4);
	assert(a1.get() == 4);
	assert(a1.getAndSet(100) == 4);
	assert(a1.get() == 100);
}
