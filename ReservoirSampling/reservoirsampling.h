// Reservoir Sampling
#include <vector>
#include <random>

template<typename T>
class ReservoirSampling {
	std::vector<T> ary_;
public:
	void update(const std::vector<T> &ary) {
		ary_ = ary;
	}
	std::vector<T> operator()(int k) {
		if (k >= ary_.size()) return ary_;
		static std::random_device rd;
		static std::mt19937 mt(rd());
		std::vector<T> ary(k);
		for (int i = 0; i < k; ++i) ary[i] = ary_[i];
		for (int i = k, n = ary_.size(); i < n; ++i) {
			std::uniform_int_distribution<> dist(0,i);
			int r = dist(mt);
			if (r < k) ary[r] = ary_[i];
		}
		return ary;
	}
};