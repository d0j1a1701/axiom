#ifndef AXIOM_IO
#define AXIOM_IO

#include <iostream>
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <stdio.h>
#include <string>
#include <cmath>

#include "misc.hpp"

namespace axiom {
struct IO {
	struct Printer {
#ifdef FastIO
		const static int BUFSIZE = 1 << 20;
		char obuf[BUFSIZE], *pp;
		inline void putchar(char x) {
			((pp - obuf == BUFSIZE && (fwrite(obuf, 1, BUFSIZE, f), pp = obuf)), *pp = x, pp++);
		}
		inline void flush() {
			fwrite(obuf, 1, pp - obuf, f), pp = obuf;
		}
		Printer() {
			pp = obuf, f = stdout;
		}
		~Printer() {
			fwrite(obuf, 1, pp - obuf, f);
		}
#else
		int putchar(const char ch) {
			return fputc(ch, f);
		}
		inline void flush() {};
#endif
		std::string sep = " ";
		int k = 2;
		FILE *f = nullptr;
		inline void freopen(FILE *F) {
			flush();
			if(f && f != stdout) fclose(f);
			f = F;
		}
		inline void write(const char &ch) {
			putchar(ch);
		}
		template<typename Tp, typename std::enable_if<
		             std::is_integral<Tp>::value>::type * = nullptr>
		inline void write(Tp x) {
			if (x < 0)	write('-'), x = -x;
			static char sta[20];
			int top = 0;
			do sta[top++] = x % 10 ^ 48, x /= 10;
			while (x);
			while (top)
				putchar(sta[--top]);
		}
		inline void write(const std::string &str) {
			for(char ch : str)    putchar(ch);
		}
		inline void write(const char *str) {
			const char *p = str;
			while(*p != '\0')	putchar(*p), p++;
		}
		template<typename Tp, typename std::enable_if<
		             std::is_floating_point<Tp>::value>::type * = nullptr>
		inline void write(Tp x) {
			if(std::isnan(x))	return write("nan"), (void)0;
			if(std::isinf(x))	return write("inf"), (void)0;
			int n = constant::pow10[k];
			if (x == 0) {
				putchar('0'), putchar('.');
				for (int i = 1; i <= k; ++i) putchar('0');
				return;
			}
			if (x < 0) putchar('-'), x = -x;
			long long y = (long long)(x * n) % n;
			long long d = (long long)x;
			write(d), putchar('.');
			static char sta[20];
			int top = 0;
			for (; top < k; y /= 10) sta[++top] = y % 10 ^ 48;
			for (int i = top; i > 0; i--) putchar(sta[i]);
		}
		template<typename Tp, typename... Ts>
		void write(Tp x, Ts... val) {
			write(x);
			write(sep);
			write(val...);
		}
		template<typename... Ts>
		void writeln(Ts... val) {
			write(val...);
			putchar('\n');
		}
		inline void writeln() {
			putchar('\n');
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
	IO() {
		in.freopen(stdout), out.freopen(stdout);
	}
	IO(std::string prefix, int id, std::string out_ext = ".out") {
		auto fin = fopen((prefix + std::to_string(id) + ".in").c_str(), "w");
		auto fout = fopen((prefix + std::to_string(id) + out_ext).c_str(), "w");
		in.freopen(fin), out.freopen(fout);
	}
	inline void freopen(std::string prefix, int id, std::string out_ext = ".out") {
		auto fin = fopen((prefix + std::to_string(id) + ".in").c_str(), "w");
		auto fout = fopen((prefix + std::to_string(id) + out_ext).c_str(), "w");
		in.freopen(fin), out.freopen(fout);
	}
};
}

#endif