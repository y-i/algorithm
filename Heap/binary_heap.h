#include <vector>
#include <algorithm>

template<typename T, typename C=std::greater<T>>
class BinaryHeap {
    std::vector<T> ary;
    int n;
    int maxswap(int idx) {
        int maxidx = idx * 2 + 1;
        if (idx * 2 + 2 < n && C()(ary[idx * 2 + 2], ary[maxidx])) maxidx = idx * 2 + 2;
        if (C()(ary[idx], ary[maxidx])) return idx;
        swap(ary[idx], ary[maxidx]);
        return maxidx;
    }
    void upheap(int idx) {
        if (idx == 0) return;
        idx = (idx - 1) / 2;
        int maxidx = maxswap(idx);
        if (maxidx != idx) upheap(idx);
    }
    void downheap(int idx) {
        if (idx * 2 + 1 >= n) return;
        int maxidx = maxswap(idx);
        if (maxidx != idx) downheap(maxidx);
    }
public:
    BinaryHeap(): n(0) {};
    BinaryHeap(const std::vector<T> &init): ary(init), n(init.size()) {
        std::sort(ary.begin(), ary.end(), V);
    }
    int size() const {
        return n;
    }
    bool empty() const {
        return n == 0;
    }
    const T& top() const {
        return ary[0];
    }
    void pop() {
        if (empty()) return;
        swap(ary[0], ary[n - 1]);
        ary.pop_back();
        --n;
        if (empty()) return;
        downheap(0);
    }
    void push(T value) {
        int idx = ary.size();
        ary.emplace_back(value);
        ++n;
        upheap(idx);
    }
};
