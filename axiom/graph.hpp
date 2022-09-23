#ifndef AXIOM_GRAPH_HPP
#define AXIOM_GRAPH_HPP

#include <initializer_list>
#include <type_traits>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <list>

#include "random.hpp"
#include "misc.hpp"

namespace axiom {
struct unweighted {};
using directed = std::true_type;
using undirected = std::false_type;
template<typename Tp>
struct Range {
	Tp a, b;
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
template<typename dir, typename Tp>
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
			for(Edge e : edges[i])
				ss << i << ' ' << e.v << ' ' << e.w << '\n';
		return ss.str();
	}
};
template<typename dir>
struct Graph<dir, unweighted> {
	std::vector<std::list<int> > edges;
	int n = 0, m = 0;
	Graph() = delete;
	Graph(int N) {
		this->resize(N);
	}
	Graph direct(bool d = true) {
		return this->directed = d, *this;
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
struct GraphFactory {
	bool self_loop = false, repeat_edge = false;
	inline GraphFactory selfloop() {
		auto res = *this;
		return res.self_loop = true, res;
	}
	inline GraphFactory repeatedge() {
		auto res = *this;
		return res.repeat_edge = true, res;
	}
	template <typename dir>
	inline Graph<dir, unweighted> regular(int n, int m) {
		if(m == -1)	m = rnd.next(1, n);
		Graph<dir, unweighted> res(n);
		while(m--) res.add(rnd.next(1, n), rnd.next(1, n));
		return res;
	}
	template <typename dir, typename Tp, typename = void >
	inline Graph<dir, Tp> regular(int n, int m, Range<Tp> rng = {1, 1}) {
		if(m == -1)	m = rnd.next(1, n);
		Graph<dir, Tp> res(n);
		while(m--) res.add(rnd.next(1, n), rnd.next(1, n), rnd.next(rng.a, rng.b));
		return res;
	}
} graph;
}

#endif
