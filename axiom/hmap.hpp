#ifndef AXIOM_HMAP_HPP
#define AXIOM_HMAP_HPP

#include <functional>
#include <algorithm>
#include <iostream>
#include <utility>
#include <chrono>
#include <vector>

#include "misc.hpp"

namespace axiom {

template<typename Key, typename Value, typename hs = std::hash<Key> >
class HashMap {
		using pkv = std::pair<Key, Value>;
		const double alpha = 0.8;
		std::vector<unsigned long>::const_iterator size_iter = constant::prime_list.begin();
		std::vector<bool> used;
		std::vector<pkv> vec;
		hs hsfunc;
		int n;
		inline void rehash(const Key &k, const Value &v) {
			auto Vec = vec;
			auto Used = used;
			vec.clear(), used.clear(), n = 0;
			vec.resize(*(++size_iter)), used.resize(*size_iter);
			for(size_t i = 0; i < Vec.size(); i++)
				if(Used[i])
					insert(Vec[i]);
			insert(std::make_pair(k, v));
		}
	public:
		HashMap() {
			vec.resize(2), used.resize(2);
		}
		HashMap(size_t Size) {
			vec.resize(*(size_iter = std::upper_bound(constant::prime_list.begin(), constant::prime_list.end(), Size)));
			used.resize(*size_iter);
		}
		inline void clear() {
			std::fill(used.begin(), used.end(), 0);
			n = 0;
		}
		inline void insert(const pkv &p) {
			const Key &k = p.first;
			const Value &v = p.second;
			size_t idx = hsfunc(k) % vec.size(), fr = idx;
			while(idx < vec.size() && used[idx] && vec[idx].first != k)   idx++;
			if(idx == vec.size())	idx = 0;
			while(idx < fr && used[idx] && vec[idx].first != k)    idx++;
			if(used[idx] && vec[idx].first == k)  vec[idx].second = v;
			if(!used[idx]) {
				if((++n) * 1.0 / vec.size() >= alpha) rehash(k, v);
				else used[idx] = 1, vec[idx] = std::make_pair(k, v);
			} else rehash(k, v);
		}
		inline void insert(const Key &k, const Value &v) {
			insert(std::make_pair(k, v));
		}
		inline int count(const Key &x) {
			size_t idx = hsfunc(x) % vec.size(), fr = idx;
			while(idx < vec.size() && !(used[idx] && vec[idx].first == x))   idx++;
			if(idx == vec.size())	idx = 0;
			while(idx < fr && !(used[idx] && vec[idx].first == x))           idx++;
			return used[idx] && vec[idx].first == x;
		}
		inline void erase(const Key &x) {
			size_t idx = hsfunc(x) % vec.size(), fr = idx;
			while(idx < vec.size() && !(used[idx] && vec[idx].first == x))   idx++;
			if(idx == vec.size())	idx = 0;
			while(idx < fr && !(used[idx] && vec[idx].first == x))           idx++;
			if(used[idx] && vec[idx].first == x) used[idx] = 0;
		}
		inline Value at(const Key &x) {
			size_t idx = hsfunc(x) % vec.size(), fr = idx;
			while(idx < vec.size() && !(used[idx] && vec[idx].first == x))   idx++;
			if(idx == vec.size())	idx = 0;
			while(idx < fr && !(used[idx] && vec[idx].first == x))           idx++;
			return vec[idx].second;
		}
		inline size_t size() {
			return n;
		}
};
}

#endif