#ifndef AXIOM_RANDOM_HPP
#define AXIOM_RANDOM_HPP

#include <initializer_list>
#include <iterator>
#include <random>
#include <chrono>
#include <vector>

#include "misc.hpp"

namespace axiom {
namespace fastrange {
/* https://github.com/lemire/fastrange */
inline static uint32_t fastrange32(uint32_t word, uint32_t p) {
	return (uint32_t)(((uint64_t)word * (uint64_t)p) >> 32);
}
inline static uint64_t fastrange64(uint64_t word, uint64_t p) {
#ifdef __SIZEOF_INT128__ // then we know we have a 128-bit int
	return (uint64_t)(((__uint128_t)word * (__uint128_t)p) >> 64);
#elif defined(_MSC_VER) && defined(_WIN64)
	// supported in Visual Studio 2005 and better
	uint64_t highProduct;
	_umul128(word, p, &highProduct); // ignore output
	return highProduct;
	unsigned __int64 _umul128(
	    unsigned __int64 Multiplier,
	    unsigned __int64 Multiplicand,
	    unsigned __int64 * HighProduct
	);
#else
	return word % p; // fallback
#endif // __SIZEOF_INT128__
}
inline static int mapping(int base, int l, int r) {
	return l + fastrange32(base, r - l + 1);
}
inline static long long mapping(long long base, long long l, long long r) {
	return l + fastrange64(base, r - l + 1);
}
inline static unsigned int mapping(unsigned int base, unsigned int l, unsigned int r) {
	return l + fastrange32(base, r - l + 1);
}
inline static unsigned long long mapping(unsigned long long base, unsigned long long l, unsigned long long r) {
	return l + fastrange64(base, r - l + 1);
}
};

struct xorshf96 {
	unsigned int x = 114514, y = 362436069, z = 521288629;
	xorshf96(unsigned int seed) {
		seed = x;
	}
	unsigned int operator()() {
		unsigned int t;
		x ^= x << 16;
		x ^= x >> 5;
		x ^= x << 1;
		t = x, x = y, y = z;
		z = t ^ x ^ y;
		return z;
	}
};

static xorshf96 random_base(std::chrono::system_clock::now().time_since_epoch().count());

struct Random {
	template<typename Tp, typename std::enable_if<
	             std::is_integral<Tp>::value>::type * = nullptr>
	static Tp next(Tp l, Tp r) {
		return fastrange::mapping((Tp)random_base(), l, r);
	}
	template<typename Tp, typename std::enable_if<
	             std::is_floating_point<Tp>::value>::type * = nullptr>
	static Tp next(Tp l, Tp r) {
		return std::uniform_real_distribution<Tp> {l, r}(random_base);
	}
	template<typename Iter, typename std::enable_if<
	             is_iterator<Iter>::value>::type * = nullptr>
	static Iter next(const Iter &a, const Iter &b) {
		return a + next((size_t)0, std::distance(a, b) - 1);
	}
	template<typename Tp>
	static typename Tp::value_type next(const Tp &container) {
		return *(begin(container) + next((size_t)0, container.size() - 1));
	}
	template<typename Tp>
	static Tp next(const std::initializer_list<Tp> &arr) {
		return *(arr.begin() + next((size_t)0, arr.size() - 1));
	}
} rnd;
};
#endif