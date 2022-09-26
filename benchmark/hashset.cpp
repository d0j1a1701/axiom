#include "../axiom/axiom.hpp"
#include <unordered_set>
#include <iostream>
#include <ctime>
using namespace axiom;
using namespace std;
const int n = 10000000;
unordered_set<int> us;
HashSet<int> us1;
HashSet<int> us2(n);
HashSet<int, axiom::exhash> us3;
HashSet<int, axiom::exhash> us4(n);
int main() {
	clock_t st = clock();
	for(int i = 1; i <= n; i++)	us.insert(i);
	cout << "unordered_set " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << us.size() << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	us1.insert(i);
	cout << "axiom::hashset " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << us1.size() << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	us2.insert(i);
	cout << "axiom::hashset(n) " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << us2.size() << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	us3.insert(i);
	cout << "axiom::hashset<exhash> " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << us3.size() << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	us4.insert(i);
	cout << "axiom::hashset<exhash>(n) " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << us4.size() << endl;
	cout << "done" << endl;
	system("pause");
	return 0;
}