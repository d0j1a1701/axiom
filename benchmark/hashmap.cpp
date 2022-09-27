#include "../axiom/axiom.hpp"
#include <unordered_map>
#include <iostream>
#include <ctime>
using namespace axiom;
using namespace std;
const int n = 10000000;
unordered_map<int, int> um;
HashMap<int, int> um1;
HashMap<int, int> um2(n);
HashMap<int, int, axiom::exhash> um3;
HashMap<int, int, axiom::exhash> um4(n);
int main() {
	clock_t st = clock();
	for(int i = 1; i <= n; i++)	um[i] = i;
	cout << "unordered_map " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << um.at(50) << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	um1[i] = i;
	cout << "axiom::hashmap " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << um1.at(50) << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	um2[i] = i;
	cout << "axiom::hashmap(n) " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << um2.at(50) << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	um3[i] = i;
	cout << "axiom::hashmap<exhash> " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << um3.at(50) << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	um4[i] = i;
	cout << "axiom::hashmap<exhash>(n) " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << um4.at(50) << endl;
	cout << "done" << endl;
	system("pause");
	return 0;
}