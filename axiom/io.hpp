#ifndef AXIOM_IO
#define AXIOM_IO

#include <type_traits>
#include <iostream>
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <string>
#include <cmath>

#include "graph.hpp"
#include "misc.hpp"

namespace axiom {
struct IO {
		class Printer {
				std::string sep = " ";
				int k = 2;
				FILE *f = nullptr;
#ifdef FastIO
				const static int BUFSIZE = 1 << 16;
				char *obuf, *pp;
#endif
			public:
#ifdef FastIO
				inline Printer &putchar(const char x) {
					return ((pp - obuf == BUFSIZE && (fwrite(obuf, 1, BUFSIZE, f), pp = obuf)), *pp = x, pp++), *this;
				}
				inline Printer &flush() {
					return fwrite(obuf, 1, pp - obuf, f), pp = obuf, *this;
				}
				Printer() {
					obuf = new char[BUFSIZE];
					pp = obuf, f = stdout;
				}
				~Printer() {
					fwrite(obuf, 1, pp - obuf, f);
				}
#else
				inline const Printer &putchar(const char ch) {
					return fputc(ch, f), *this;
				}
				inline void flush() {};
#endif
				inline void freopen(FILE *F) {
					flush();
					if(f && f != stdout) fclose(f);
					f = F;
				}
				inline const Printer &write(const char &ch) {
					return putchar(ch), *this;
				}
				template<typename Tp, typename std::enable_if<
				             std::is_integral<Tp>::value>::type * = nullptr>
				inline const Printer & write(Tp x) {
					if (x < 0)	write('-'), x = -x;
					static char sta[20];
					int top = 0;
					do sta[top++] = x % 10 ^ 48, x /= 10;
					while (x);
					while (top)
						putchar(sta[--top]);
					return *this;
				}
				inline const Printer &write(const std::string &str) {
					for(char ch : str)    putchar(ch);
					return *this;
				}
				inline const Printer &write(const char *str) {
					const char *p = str;
					while(*p != '\0')	putchar(*p), p++;
					return *this;
				}
				template<typename Tp, typename std::enable_if<
				             std::is_floating_point<Tp>::value>::type * = nullptr>
				inline const Printer & write(Tp x) {
					if(std::isnan(x))	return write("nan"), *this;
					if(std::isinf(x))	return write("inf"), *this;
					int n = constant::pow10[k];
					if (x == 0) {
						putchar('0'), putchar('.');
						for (int i = 1; i <= k; ++i) putchar('0');
						return *this;
					}
					if (x < 0) putchar('-'), x = -x;
					long long y = (long long)(x * n) % n;
					long long d = (long long)x;
					write(d), putchar('.');
					static char sta[20];
					int top = 0;
					for (; top < k; y /= 10) sta[++top] = y % 10 ^ 48;
					for (int i = top; i > 0; i--) putchar(sta[i]);
					return *this;
				}
				template<typename Tp, typename dir, typename std::enable_if<std::is_same<Tp, unweighted>::value>::type * = nullptr>
				inline const Printer & write(Graph<Tp, dir> g) {
					for(int i = 1; i <= g.n; i++)
						for(int j : g.edges[i])
							writeln(i, j);
					return *this;
				}
				template < typename Tp, typename dir, typename std::enable_if < !std::is_same<Tp, unweighted>::value >::type * = nullptr >
				inline const Printer & write(Graph<Tp, dir> g) {
					for(int i = 1; i <= g.n; i++)
						for(auto edge : g.edges[i])
							writeln(i, edge.v, edge.w);
					return *this;
				}
				template<typename Tp, typename... Ts>
				const Printer &write(Tp x, Ts... val) {
					write(x);
					write(sep);
					write(val...);
					return *this;
				}
				template<typename... Ts>
				const Printer &writeln(Ts... val) {
					write(val...);
					putchar('\n');
					return *this;
				}
				inline const Printer &writeln(void) {
					putchar('\n');
					return *this;
				}
				inline int printf(const char *form, ...) {
					flush();
					int res;
					va_list arglist;
					va_start(arglist, form);
					res = vfprintf(f, form, arglist);
					va_end(arglist);
					return res;
				}
				inline Printer setsep(const std::string &s) {
					return sep = s, *this;
				}
				inline Printer setprecision(const int &K) {
					return k = K, *this;
				}
		} in, out;
		inline void freopen(std::string prefix, int id, std::string out_ext) {
			auto fin = fopen((prefix + std::to_string(id) + ".in").c_str(), "w");
			in.freopen(fin);
			auto fout = fopen((prefix + std::to_string(id) + out_ext).c_str(), "w");
			out.freopen(fout);
		}
		inline void freopen(std::string prefix, int id) {
			auto fin = fopen((prefix + std::to_string(id) + ".in").c_str(), "w");
			in.freopen(fin);
		}
		inline void freopen(std::string inf, std::string outf) {
			auto fin = fopen(inf.c_str(), "w");
			in.freopen(fin);
			auto fout = fopen(outf.c_str(), "w");
			out.freopen(fin);
		}
		inline void freopen(std::string inf) {
			auto fin = fopen(inf.c_str(), "w");
			in.freopen(fin);
		}
		IO() {
			in.freopen(stdout), out.freopen(stdout);
		}
		IO(std::string prefix, int id, std::string out_ext) {
			this->freopen(prefix, id, out_ext);
		}
		IO(std::string prefix, int id) {
			this->freopen(prefix, id);
		}
		IO(std::string inf, std::string outf) {
			this->freopen(inf, outf);
		}
		IO(std::string inf) {
			this->freopen(inf);
		}
};
}

#endif
