#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <chrono>
#include "walkersaliasmethod.h"
using namespace std;

class NormalMethod {
	vector<double> list;
public:
	NormalMethod() {}
	void update(vector<double> ary) {
		double sum = 0;
		int len = ary.size();
		list = vector<double>(len);
		for (const auto &i: ary) sum += i;
		for (auto &i: ary) i =  i * len / sum;
		list[0] = ary[0];
		for (int i = 0; i < len; ++i) {
			list[i] = list[i-1] + ary[i];
		}
	}
	int operator()() {
		static random_device rd;
		static mt19937 mt(rd());
		static uniform_real_distribution<double> dist;
		auto pos = dist(mt) * list.size();
		for (int i = 0, n = list.size(); i < n; ++i)
			if (pos < list[i]) return i;
		return -1;
	}
};

int main() {
	int times = 100000;

	//vector<double> ary{{1,4,3,12,16,64}};
	vector<double> ary{{1,4,3,12,16,24,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

	WalkersAliasMethod wam;
	wam.update(ary);
	vector<int> cnt(ary.size());
	auto t1 = std::chrono::system_clock::now();
	for (int i = 0; i < times; ++i) ++cnt[wam()];
	auto t2 = std::chrono::system_clock::now();

	NormalMethod nm;
	nm.update(ary);
	vector<int> cnt2(ary.size());
	auto t3 = std::chrono::system_clock::now();
	for (int i = 0; i < times; ++i) ++cnt2[nm()];
	auto t4 = std::chrono::system_clock::now();

	cout << "---" << endl;
	for (const auto &i: cnt) cout << i << endl;
	cout << "---" << endl;
	for (const auto &i: cnt2) cout << i << endl;
	cout << "---" << endl;
	cout << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << endl;
	cout << std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count() << endl;

	return 0;
}