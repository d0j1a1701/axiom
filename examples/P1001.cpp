#include "../axiom/axiom.hpp"
#include <iostream>
#include <cmath>
using namespace axiom;
using namespace std;
int main() {
	for(int _ = 1; _ <= 5; _++) {
		IO io("sum", _);
		int a = rnd.next(1, d2i(1e9)), b = rnd.next(1, d2i(1e9));
		io.in.writeln(a, b);
		io.out.writeln(a + b);
	}
	system("pause");
	return 0;
}