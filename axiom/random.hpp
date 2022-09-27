#ifndef AXIOM_RANDOM_HPP
#define AXIOM_RANDOM_HPP

#include <initializer_list>
#include <iterator>
#include <random>
#include <chrono>
#include <vector>

#include "hmap.hpp"
#include "misc.hpp"

namespace axiom {
namespace fastrange {
/* https://github.com/lemire/fastrange */
inline uint32_t fastrange32(uint32_t word, uint32_t p) {
	return (uint32_t)(((uint64_t)word * (uint64_t)p) >> 32);
}
inline uint64_t fastrange64(uint64_t word, uint64_t p) {
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
inline int mapping(int base, int l, int r) {
	return l + fastrange32(base, r - l + 1);
}
inline long long mapping(long long base, long long l, long long r) {
	return l + fastrange64(base, r - l + 1);
}
inline unsigned int mapping(unsigned int base, unsigned int l, unsigned int r) {
	return l + fastrange32(base, r - l + 1);
}
inline unsigned long long mapping(unsigned long long base, unsigned long long l, unsigned long long r) {
	return l + fastrange64(base, r - l + 1);
}
};

struct xorshf128 {
	unsigned long long a = 114514, b = 362436069, t = 1919810;
	xorshf128(unsigned int seed = 114514) {
		a = seed;
	}
	inline void seed(unsigned int s) {
		a = s;
	}
	inline unsigned int operator()() {
		a ^= a << 23, a ^= a >> 18;
		a ^= b, a ^= b >> 5;
		t = a, a = b, b = t;
		return a + b;
	}
};

struct xorshf128_64 {
	unsigned long long a = 114514, b = 362436069, t = 1919810;
	xorshf128_64(unsigned long long seed = 114514) {
		a = seed;
	}
	inline void seed(unsigned long long s) {
		a = s;
	}
	inline unsigned long long operator()() {
		a ^= a << 23, a ^= a >> 18;
		a ^= b, a ^= b >> 5;
		t = a, a = b, b = t;
		return a + b;
	}
};

template<typename Rng = xorshf128>
struct Random {
	struct Sequence {
		std::vector<unsigned long long> vec;
		inline Sequence(int n, unsigned long long m, const Random &rnd) {
			HashMap<unsigned long long, unsigned long long> rest(n);
			vec.clear(), vec.resize(n);
			for(int i = 0; i < n; i++)	vec[i] = i + 1;
			for(int i = 0; i < n; i++) {
				int j = rnd.next(i, m - 1);
				if(j < n)	std::swap(vec[i], vec[j]);
				else if(!rest.count(j))	rest[j] = vec[i], vec[i] = j + 1;
				else std::swap(vec[i], rest[j]);
			}
		}
	};
	Rng random_base;
	Random() {
		random_base.seed(std::chrono::system_clock::now().time_since_epoch().count());
	}
	template<typename Tp, typename std::enable_if<
	             std::is_integral<Tp>::value>::type * = nullptr>
	Tp next(Tp l, Tp r) {
		return fastrange::mapping((Tp)random_base(), l, r);
	}
	template<typename Tp, typename std::enable_if<
	             std::is_floating_point<Tp>::value>::type * = nullptr>
	Tp next(Tp l, Tp r) {
		return std::uniform_real_distribution<Tp> {l, r}(random_base);
	}
	template<typename Iter, typename std::enable_if<
	             is_iterator<Iter>::value>::type * = nullptr>
	Iter next(const Iter &a, const Iter &b) {
		return a + next((size_t)0, std::distance(a, b) - 1);
	}
	template<typename Tp>
	typename Tp::value_type next(const Tp &container) {
		return *(begin(container) + next((size_t)0, container.size() - 1));
	}
	template<typename Tp>
	Tp next(const std::initializer_list<Tp> &arr) {
		return *(arr.begin() + next((size_t)0, arr.size() - 1));
	}
};

Random<xorshf128> rnd;

};
#endif