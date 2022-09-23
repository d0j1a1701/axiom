#ifndef AXIOM_MISC_HPP
#define AXIOM_MISC_HPP

#include <initializer_list>
#include <string>
#include <vector>

namespace axiom::constant {
const double pi = 3.14159265358;
const double e = 2.71828182845;
const std::string lower = "abcdefghijklmnopqrstuvwxyz";
const std::string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string numbers = "0123456789";
const long long pow10[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000, 1000000000000000, 10000000000000000};
};
namespace axiom {
template <typename... >
using void_t = void;

template<class _Ty1, class _Ty2>
struct is_not_same : std::true_type {};
template<class _Ty1>
struct is_not_same<_Ty1, _Ty1> : std::false_type {};


template <class T, class = void>
struct is_iterator : std::false_type { };

template <class T>
struct is_iterator<T, void_t<
	typename std::iterator_traits<T>::iterator_category
	        >> : std::true_type { };
inline int d2i(double x) {
	return x;
}
inline long long d2ll(double x) {
	return x;
}
inline std::vector<int> d2i(std::initializer_list<double> arr) {
	std::vector<int> res(arr.size());
	int i = 0;
	for(double x : arr)   res[i++] = x;
	return res;
}
inline std::vector<long long> d2ll(std::initializer_list<double> arr) {
	std::vector<long long> res(arr.size());
	int i = 0;
	for(double x : arr)   res[i++] = x;
	return res;
}
}

#endif