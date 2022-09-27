#include "../axiom/axiom.hpp"
#include <iostream>
#include <chrono>
#include <random>
#include <ctime>
using namespace axiom;
using namespace std;
const int n = 100000000;
unsigned long long sum;
auto seed = chrono::steady_clock::now().time_since_epoch().count();
uniform_int_distribution<int> ui(1, 114514);
xorshf128 xs128(seed);
mt19937 mt(seed);
int main() {
	clock_t st = clock();
	sum = 0;
	for(int i = 1; i <= n; i++)	sum ^= xs128();
	cout << "axiom::xorshf128 " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << sum << endl;
	st = clock(), sum = 0;
	for(int i = 1; i <= n; i++)	sum ^= mt();
	cout << "mt19937 " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << sum << endl;
	st = clock(), sum = 0;
	for(int i = 1; i <= n; i++)	sum ^= rnd.next(1, 114514);
	cout << "axiom::rnd.next() " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << sum << endl;
	st = clock(), sum = 0;
	for(int i = 1; i <= n; i++)	sum ^= ui(mt);
	cout << "mt19937 & uniform_int_distribution " << ((clock() - st) * 1.0 / CLOCKS_PER_SEC) << ' ' << sum << endl;
	cout << "done" << endl;
	system("pause");
	return 0;
}