// Walker's alias method
#include <vector>
#include <set>
#include <random>

class WalkersAliasMethod {
	std::vector<double> list, aliasList;
public:
	WalkersAliasMethod() {}
	void update(std::vector<double> ary) {
		double sum = 0;
		int len = ary.size();
		list = std::vector<double>(len);
		aliasList = std::vector<double>(len);
		for (const auto &i: ary) sum += i; // aryの合計値
		for (auto &i: ary) i =  i * len / sum; // 合計が配列の長さになるように正規化
		int pos = 0; // 埋まっていないpos
		std::set<int> s; // 1より大きい値の集合
		for (int i = 0; i < len; ++i) { // 1以下の値は先に埋める
			if (ary[i] <= 1) {
				list[i] = ary[i];
			} else {
				s.insert(i);
			}
		}
		int bpos = -1;
		for (const auto &i: s) { // 1を超えるの値を割り振る
			while (pos < len && 1 - list[pos] <= ary[i]) {
				if (pos != i) {
					ary[i] -= 1 - list[pos];
					aliasList[pos] = i;
				}
				++pos;
			}
			if (bpos >= 0) {
				ary[i] -= 1 - list[bpos];
				aliasList[bpos] = i;
				bpos = i;
			}
			list[i] = ary[i];
		} 
	}
	int operator()() {
		static std::random_device rd;
		static std::mt19937 mt(rd());
		static std::uniform_real_distribution<double> dist;
		auto pos = dist(mt) * list.size();
		int i = (int)pos;
		if (list[i] > pos - i) return i;
		else return aliasList[i];
	}
};