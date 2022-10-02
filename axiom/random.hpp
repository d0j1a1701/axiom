#ifndef AXIOM_RANDOM_HPP
#define AXIOM_RANDOM_HPP

#include <initializer_list>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <random>
#include <chrono>
#include <vector>
#include <limits>

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

template<typename Tp>
struct xorshf128_base {
	using result_type = Tp;
	unsigned long long a = 114514, b = 362436069, t = 1919810;
	inline constexpr static result_type max() {
		return std::numeric_limits<result_type>::max();
	}
	inline constexpr static result_type min() {
		return std::numeric_limits<result_type>::min();
	}
	xorshf128_base(Tp seed = 114514) {
		a = seed;
	}
	inline void seed(Tp s) {
		a = s;
	}
	inline Tp operator()() {
		a ^= a << 23, a ^= a >> 18;
		a ^= b, a ^= b >> 5;
		t = a, a = b, b = t;
		return a + b;
	}
};

using xorshf128 = xorshf128_base<unsigned int>;
using xorshf128_64 = xorshf128_base<unsigned long long>;

template<typename Rng = xorshf128>
struct Random {
	Rng random_base;
	Random() {
		random_base.seed(std::chrono::system_clock::now().time_since_epoch().count());
	}
	template<typename Tp, typename std::enable_if<
	             std::is_integral<Tp>::value>::type * = nullptr>
	inline Tp next(Tp l, Tp r) {
		return fastrange::mapping((Tp)random_base(), l, r);
	}
	template<typename Tp, typename std::enable_if<
	             std::is_floating_point<Tp>::value>::type * = nullptr>
	inline Tp next(Tp l, Tp r) {
		return std::uniform_real_distribution<Tp> {l, r}(random_base);
	}
	template<typename Iter, typename std::enable_if<
	             is_iterator<Iter>::value>::type * = nullptr>
	inline Iter next(const Iter &a, const Iter &b) {
		return a + next((size_t)0, std::distance(a, b) - 1);
	}
	template<typename Tp>
	typename Tp::value_type next(const Tp &container) {
		return *(begin(container) + next((size_t)0, container.size() - 1));
	}
	template<typename Tp>
	inline Tp next(const std::initializer_list<Tp> &arr) {
		return *(arr.begin() + next((size_t)0, arr.size() - 1));
	}
	template<class Tp, class = std::enable_if_t<std::is_integral_v<Tp> > >
	inline std::vector<Tp> sequence(size_t n, Tp lo, Tp hi) {
		if (!n) return {};
		const auto len = hi - lo + 1;
		if (len <= 1000000 && false) { //O(m)
			Tp *tmp = new Tp[len];
			std::iota(tmp, tmp + len, (Tp)0);
			std::shuffle(tmp, tmp + len, random_base);
			std::vector<Tp> ret;
			ret.resize(n);
			for (size_t i = 0; i < n; ++i) ret[i] = lo + tmp[i];
			return ret;
		} else { //O(n)
			HashMap<Tp, Tp> rest(n);
			Tp *tmp = new Tp[n];
			for (Tp i = 0; i < n; i++) tmp[i] = lo + i;
			for (Tp i = 0; i < n; i++) {
				const Tp j = next(i, len - 1);
				if (j < n) std::swap(tmp[i], tmp[j]);
				else {
					if (!rest.count(j)) {
						rest[j] = tmp[i];
						tmp[i] = lo + j;
					} else std::swap(tmp[i], rest[j]);
				}
			}
			return std::vector<Tp>(tmp, tmp + n);
		}
	}
};

Random<xorshf128> rnd;

};
#endif