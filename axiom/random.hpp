#ifndef AXIOM_RANDOM_HPP
#define AXIOM_RANDOM_HPP

#include <initializer_list>
#include <iterator>
#include <random>
#include <chrono>
#include <vector>

#include "misc.hpp"

namespace axiom {
static std::mt19937 random_base = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
struct Random {
	template<typename Tp, typename std::enable_if<
	             std::is_integral<Tp>::value>::type * = nullptr>
	static Tp next(Tp l, Tp r) {
		return std::uniform_int_distribution<Tp> {l, r}(random_base);
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