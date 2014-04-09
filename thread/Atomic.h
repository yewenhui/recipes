#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#include <stdint.h>
#include <boost/noncopyable.hpp>

template<typename T>
class AtomicIntegerT
	: public boost::noncopyable
{
public:
	AtomicIntegerT()
		: m_value(0)
	{
	}

	AtomicIntegerT(const AtomicIntegerT& that)
		: m_value(that.get())
	{
	}

	AtomicIntegerT& operator=(const AtomicIntegerT& that) {
		getAndSet(that.get());
		return *this;
	}
	
	T			get() const {
		return __sync_val_compare_and_swap(const_cast<volatile T*>(&m_value), 0, 0);
	}

	T			getAndAdd(T x) {
		return __sync_fetch_and_add(&m_value, x);
	}

	T			addAndGet(T x) {
		return getAndAdd(x) + x;
	}

	T			incrementAndGet() {
		return addAndGet(1);
	}

	void		add(T x) {
		getAndAdd(x);
	}

	void		increment() {
		incrementAndGet();
	}

	void		decrement() {
		getAndAdd(-1);
	}

	T			getAndSet(T newValue) {
		return __sync_lock_test_and_set(&m_value, newValue);
	}

private:
	volatile T m_value;
};

typedef AtomicIntegerT<int32_t> AtomicInt32;
typedef AtomicIntegerT<int64_t> AtomicInt64;

#endif // _ATOMIC_H_