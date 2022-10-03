#define FastIO
#include "../axiom/axiom.hpp"
#include <iostream>
using namespace axiom;
using namespace std;
IO io;
int main() {
	for(int _ = 1; _ <= 10; _++) {
		io.freopen("path", _);
		int n = 100000, m = 200000;
		io.in.writeln(n, m);
		Graph g = graph.regular<int>(n, m, {0, 1000000000});
		io.in.write(g);
		printf("Generated #%d\n", _);
	}
	system("pause");
	return 0;
}