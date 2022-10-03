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
		const double alpha = 0.75;
		const size_t npos = -1ull;
		std::vector<unsigned long>::const_iterator size_iter = constant::prime_list.begin();
		std::vector<bool> used;
		std::vector<pkv> vec;
		std::pair<Key, size_t> cache;
		hs hsfunc;
		int n;
		inline Value &rehash(const Key &k, const Value &v) {
			auto Vec = vec;
			auto Used = used;
			vec.clear(), used.clear(), n = 0;
			vec.resize(*(++size_iter)), used.resize(*size_iter);
			for(size_t i = 0; i < Vec.size(); i++)
				if(Used[i])
					insert(Vec[i]);
			return insert(std::make_pair(k, v));
		}
		inline size_t find(const Key &k) {
			if(cache.first == k && used[cache.second])	return cache.second;
			size_t idx = hsfunc(k) % vec.size(), fr = idx;
			while(idx < vec.size() && !(used[idx] && vec[idx].first == k))   idx++;
			if(idx == vec.size())	idx = 0;
			while(idx < fr && !(used[idx] && vec[idx].first == k))           idx++;
			return used[idx] && vec[idx].first == k ? (cache = std::make_pair(k, idx), idx) : npos;
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
		inline Value &insert(const pkv &p) {
			const Key &k = p.first;
			const Value &v = p.second;
			size_t idx = hsfunc(k) % vec.size(), fr = idx;
			while(idx < vec.size() && used[idx] && vec[idx].first != k)   idx++;
			if(idx == vec.size())	idx = 0;
			while(idx < fr && used[idx] && vec[idx].first != k)    idx++;
			if(used[idx] && vec[idx].first == k)  vec[idx].second = v;
			if(!used[idx]) {
				if((++n) * 1.0 / vec.size() >= alpha) return rehash(k, v);
				else return used[idx] = 1, cache = std::make_pair(k, idx), vec[idx] = std::make_pair(k, v), vec[idx].second;
			}
			return rehash(k, v);
		}
		inline Value &emplace(const Key &k, const Value &v) {
			return insert(std::make_pair(k, v));
		}
		inline int count(const Key &x) {
			return find(x) != npos;
		}
		inline void erase(const Key &x) {
			size_t idx = find(x);
			if(idx != npos) used[idx] = 0;
		}
		inline Value at(const Key &x) {
			size_t idx = find(x);
			return idx != npos ? vec[idx].second : Value();
		}
		inline Value &operator[](const Key &k) {
			if(cache.first == k && used[cache.second])	return vec[cache.second].second;
			size_t idx = hsfunc(k) % vec.size(), fr = idx;
			while(idx < vec.size() && used[idx] && vec[idx].first != k)   idx++;
			if(idx == vec.size())	idx = 0;
			while(idx < fr && used[idx] && vec[idx].first != k)    idx++;
			if(used[idx] && vec[idx].first == k)  return cache = std::make_pair(k, idx), vec[idx].second;
			if(!used[idx]) {
				if((++n) * 1.0 / vec.size() >= alpha) return rehash(k, Value());
				else return used[idx] = 1, vec[idx] = std::make_pair(k, Value()), vec[idx].second;
			}
			return rehash(k, Value());
		}
		inline size_t size() {
			return n;
		}
};
}

#endif