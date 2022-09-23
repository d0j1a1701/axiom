#include "../axiom/axiom.hpp"
#include <iostream>
using namespace axiom;
using namespace std;
IO io;
int main() {
	auto g = graph.flower(10);
	io.in.write(g);
	system("pause");
	return 0;
}