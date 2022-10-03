#include "../axiom/axiom.hpp"
#include <unordered_map>
#include <iostream>
#include <ctime>
#include <map>
using namespace axiom;
using namespace std;
const int n = 10000000;
map<int, int> mp;
unordered_map<int, int> um;
HashMap<int, int> um1;
HashMap<int, int> um2(n);
HashMap<int, int, axiom::exhash> um3;
HashMap<int, int, axiom::exhash> um4(n);
int main() {
	clock_t st = clock();
	for(int i = 1; i <= n; i++)	mp[rnd.next(1, 1 << 20)] = i;
	cout << "map " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << mp.at(50) << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	um[rnd.next(1, 1 << 20)] = i;
	cout << "unordered_map " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << um.at(50) << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	um1[rnd.next(1, 1 << 20)] = i;
	cout << "axiom::hashmap " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << um1.at(50) << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	um2[rnd.next(1, 1 << 20)] = i;
	cout << "axiom::hashmap(n) " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << um2.at(50) << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	um3[rnd.next(1, 1 << 20)] = i;
	cout << "axiom::hashmap<exhash> " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << um3.at(50) << endl;
	st = clock();
	for(int i = 1; i <= n; i++)	um4[rnd.next(1, 1 << 20)] = i;
	cout << "axiom::hashmap<exhash>(n) " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << um4.at(50) << endl;
	cout << "done" << endl;
	system("pause");
	return 0;
}