#ifndef AXIOM_HSET_HPP
#define AXIOM_HSET_HPP

#include <functional>
#include <algorithm>
#include <iostream>
#include <vector>

#include "misc.hpp"

namespace axiom {

template<typename Tp, typename hs = std::hash<Tp> >
class HashSet {
		const double alpha = 0.75;
		std::vector<unsigned long>::const_iterator size_iter = constant::prime_list.begin();
		std::vector<bool> used;
		std::vector<Tp> vec;
		hs hsfunc;
		int n;
		inline void rehash(Tp x) {
			auto Vec = vec;
			auto Used = used;
			vec.clear(), used.clear(), n = 0;
			vec.resize(*(++size_iter)), used.resize(*size_iter);
			for(size_t i = 0; i < Vec.size(); i++)
				if(Used[i])
					insert(Vec[i]);
			insert(x);
		}
	public:
		HashSet() {
			vec.resize(2), used.resize(2);
		}
		HashSet(size_t Size) {
			vec.resize(*(size_iter = std::upper_bound(constant::prime_list.begin(), constant::prime_list.end(), Size)));
			used.resize(*size_iter);
		}
		inline void clear() {
			std::fill(used.begin(), used.end(), 0);
			n = 0;
		}
		inline void insert(const Tp &x) {
			size_t idx = hsfunc(x) % vec.size(), fr = idx;
			while(idx < vec.size() && used[idx] && vec[idx] != x)   idx++;
			if(idx == vec.size())	idx = 0;
			while(idx < fr && used[idx] && vec[idx] != x)    idx++;
			if(used[idx] && vec[idx] == x)  return;
			if(!used[idx]) {
				if((++n) * 1.0 / vec.size() >= alpha) rehash(x);
				else used[idx] = 1, vec[idx] = x;
			} else rehash(x);
		}
		inline int count(const Tp &x) {
			size_t idx = hsfunc(x) % vec.size(), fr = idx;
			while(idx < vec.size() && !(used[idx] && vec[idx] == x))   idx++;
			if(idx == vec.size())	idx = 0;
			while(idx < fr && !(used[idx] && vec[idx] == x))           idx++;
			return used[idx] && vec[idx] == x;
		}
		inline void erase(const Tp &x) {
			size_t idx = hsfunc(x) % vec.size(), fr = idx;
			while(idx < vec.size() && !(used[idx] && vec[idx] == x))   idx++;
			if(idx == vec.size())	idx = 0;
			while(idx < fr && !(used[idx] && vec[idx] == x))           idx++;
			if(used[idx] && vec[idx] == x) used[idx] = 0;
		}
		inline size_t size() {
			return n;
		}
};
}

#endif