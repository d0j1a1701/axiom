#include "../axiom/axiom.hpp"
#include <unordered_set>
#include <iostream>
#include <ctime>
#include <set>
using namespace axiom;
using namespace std;
const int n = 10000000;
set<int> s;
unordered_set<int> us;
HashSet<int> us1;
HashSet<int> us2(n);
HashSet<int> us3;
HashSet<int> us4(n);
int main() {
	clock_t st = clock();
	for(int i = 1; i <= n; i++)	s.insert(rnd.next(1, 1 << 20));
	cout << "set " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << s.size() << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	us.insert(rnd.next(1, 1 << 20));
	cout << "unordered_set " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << us.size() << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	us1.insert(rnd.next(1, 1 << 20));
	cout << "axiom::hashset " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << us1.size() << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	us2.insert(rnd.next(1, 1 << 20));
	cout << "axiom::hashset(n) " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << us2.size() << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	us3.insert(rnd.next(1, 1 << 20));
	cout << "axiom::hashset<exhash> " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << us3.size() << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	us4.insert(rnd.next(1, 1 << 20));
	cout << "axiom::hashset<exhash>(n) " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << us4.size() << endl;
	cout << "done" << endl;
	system("pause");
	return 0;
}