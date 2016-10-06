#include <iostream>
#include "reservoirsampling.h"
using namespace std;

int main() {
	constexpr int times = 10000;
	ReservoirSampling<int> rs;
	vector<int> v(100), cnt(100, 0);
	iota(begin(v), end(v), 0);
	rs.update(v);
	for (int i = 0; i < times; ++i) {
		auto ary = rs(10);
		for (const auto &i: ary) ++cnt[i];
	}
	for (const auto &i: cnt) cout << i << endl;

	return 0;
}