#define FastIO
#include "../axiom/axiom.hpp"
#include <iostream>
using namespace axiom;
using namespace std;
int main() {
	for(int _ = 1; _ <= 10; _++) {
		IO io("path", _);
		int n = 100000, m = 200000;
		io.in.writeln(n, m);
		Graph g = graph.regular<int>(n, m);
		io.in.writeln(g.stringify());
		printf("Generated #%d\n", _);
	}
	system("pause");
	return 0;
}