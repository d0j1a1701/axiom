#ifndef AXIOM_GRAPH_HPP
#define AXIOM_GRAPH_HPP

#include <initializer_list>
#include <type_traits>
#include <iostream>
#include <sstream>
#include <utility>
#include <cstdio>
#include <vector>
#include <list>
#include <set>

#include "random.hpp"
#include "hset.hpp"
#include "misc.hpp"

namespace axiom {
struct unweighted {};
using directed = std::true_type;
using undirected = std::false_type;

template<typename Tp>
struct WeightGenerator {
	Tp a = 1, b = 1;
	WeightGenerator() = default;
	WeightGenerator(Tp A, Tp B) {
		a = A, b = B;
	}
	WeightGenerator(Tp x) {
		a = 1, b = x;
	}
	inline Tp operator()() {
		return rnd.next(a, b);
	}
};

template<typename Tp>
struct Edge {
	int v;
	Tp w;
	inline Edge() = default;
	inline Edge(int V, Tp W) {
		v = V, w = W;
	}
	inline Edge(int V) {
		v = V;
	}
};

template<typename Tp, typename dir>
struct Graph {
	std::vector<std::list<Edge<Tp> > > edges;
	int n = 0, m = 0;
	Graph() = delete;
	Graph(int N) {
		this->resize(N);
	}
	inline void add(int u, int v, Tp w = 1) {
		edges[u].emplace_back(v, w);
		if(!directed::value && u != v)	edges[v].emplace_back(u, w);
		m++;
	}
	inline void resize(int N) {
		edges.resize((n = N) + 1);
	}
	inline void clear() {
		edges.clear();
		n = m = 0;
	}
	inline std::string stringify() {
		std::stringstream ss;
		for(int i = 1; i <= n; i++)
			for(auto e : edges[i])
				ss << i << ' ' << e.v << ' ' << e.w << '\n';
		return ss.str();
	}
};

template<typename dir>
struct Graph<unweighted, dir> {
	std::vector<std::list<int> > edges;
	int n = 0, m = 0;
	Graph() = delete;
	Graph(int N) {
		this->resize(N);
	}
	inline void add(int u, int v) {
		edges[u].emplace_back(v);
		if(!directed::value && u != v)	edges[v].emplace_back(u);
		m++;
	}
	inline void resize(int N) {
		edges.resize((n = N) + 1);
	}
	inline void clear() {
		edges.clear();
		n = m = 0;
	}
	inline std::string stringify() {
		std::stringstream ss;
		for(int i = 1; i <= n; i++)
			for(int v : edges[i])
				ss << i << ' ' << v << '\n';
		return ss.str();
	}
};

class GraphFactory {
		inline std::pair<int, int> calc_uv(int n, long long edge_id) {
			int l = 1, r = n, u = -1, v = -1;
			while(l <= r) {
				long long mid = l + ((r - l) >> 1);
				if((((n << 1) - mid) * (mid - 1) >> 1) < edge_id)	u = mid, l = mid + 1;
				else r = mid - 1;
			}
			v = edge_id - ((long long)((n << 1) - u) * (u - 1) >> 1) + u;
			return std::make_pair(u, v);
		}
	public:
		template <typename dir = undirected, class = std::enable_if_t<std::is_same<undirected, dir>::value > >
		inline Graph<unweighted, dir> regular(int n, int m) {
			Graph<unweighted, dir> res(n);
			for(auto x : rnd.sequence(m, 1ll, (long long)n * (n - 1) >> 1)) {
				auto edge = calc_uv(n, x);
				res.add(edge.first, edge.second);
			}
			return res;
		}
		template <typename Tp, typename dir = undirected, typename wg = WeightGenerator<Tp> >
		inline Graph<Tp, dir> regular(int n, int m, wg rng = {1, 1}) {
			Graph<Tp, dir> res(n);
			for(auto x : rnd.sequence(m, 1ll, (long long)n * (n - 1) >> 1)) {
				auto edge = calc_uv(n, x);
				res.add(edge.first, edge.second, rng());
			}
			return res;
		}
		template <typename dir = undirected, class = std::enable_if_t<std::is_same<undirected, dir>::value > >
		inline Graph<unweighted, dir> tree(int n, double chain = 0, double flower = 0) {
			Graph<unweighted, dir> res(n);
			int chain_count = std::min((int)((n - 1) * chain), n - 1);
			int flower_count = (n - 1) * flower;
			if(chain_count + flower_count > n - 1)	flower_count = n - 1 - chain_count;
			int random_count = n - 1 - chain_count - flower_count;
			for(int i = 2; i <= chain_count + 1; i++)	res.add(i - 1, i);
			for(int i = chain_count + 2; i <= chain_count + flower_count + 1; i++)	res.add(1, i);
			for(int i = n - random_count + 1; i <= n; i++)	res.add(rnd.next(1, i - 1), i);
			return res;
		}
		template <typename Tp, typename dir = undirected, typename wg = WeightGenerator<Tp> >
		inline Graph<Tp, dir> tree(int n, wg rng = {1, 1}, double chain = 0, double flower = 0) {
			Graph<Tp, dir> res(n);
			int chain_count = std::min((int)((n - 1) * chain), n - 1);
			int flower_count = (n - 1) * flower;
			if(chain_count + flower_count > n - 1)	flower_count = n - 1 - chain_count;
			int random_count = n - 1 - chain_count - flower_count;
			for(int i = 2; i <= chain_count + 1; i++)	res.add(i - 1, i, rng());
			for(int i = chain_count + 2; i <= chain_count + flower_count + 1; i++)	res.add(1, i, rng());
			for(int i = n - random_count + 1; i <= n; i++)	res.add(rnd.next(1, i - 1), i, rng());
			return res;
		}
		template <typename dir = undirected, class = std::enable_if_t<std::is_same<undirected, dir>::value > >
		inline Graph<unweighted, dir> chain(int n) {
			return tree<dir>(n, 1, 0);
		}
		template <typename Tp, typename dir = undirected, typename wg = WeightGenerator<Tp> >
		inline Graph<Tp, dir> chain(int n, wg rng = {1, 1}) {
			return tree<Tp, dir>(n, rng, 1, 0);
		}
		template <typename dir = undirected, class = std::enable_if_t<std::is_same<undirected, dir>::value > >
		inline Graph<unweighted, dir> flower(int n) {
			return tree<dir>(n, 0, 1);
		}
		template <typename Tp, typename dir = undirected, typename wg = WeightGenerator<Tp> >
		inline Graph<Tp, dir> flower(int n, wg rng = {1, 1}) {
			return tree<Tp, dir>(n, rng, 0, 1);
		}
		template <typename dir = undirected, class = std::enable_if_t<std::is_same<undirected, dir>::value > >
		inline Graph<unweighted, dir> DAG(int n, int m) {
			Graph<unweighted, dir> res(n);
			for(int i = 2; i <= n; i++)
				res.add(1, i);
			int i = n - 1;
			for(auto x : rnd.sequence(m - n + 1, (long long)n, (long long)n * (n - 1) >> 1)) {
				auto edge = calc_uv(n, x);
				res.add(edge.first, edge.second);
			}
			return res;
		}
		template <typename Tp, typename dir = undirected, typename wg = WeightGenerator<Tp> >
		inline Graph<Tp, dir> DAG(int n, int m, wg rng = {1, 1}) {
			Graph<Tp, dir> res(n);
			for(int i = 2; i <= n; i++)
				res.add(1, i, rng());
			int i = n - 1;
			for(auto x : rnd.sequence(m - n + 1, (long long)n, (long long)n * (n - 1) >> 1)) {
				auto edge = calc_uv(n, x);
				res.add(edge.first, edge.second, rng());
			}
			return res;
		}
} graph;
}

#endif
