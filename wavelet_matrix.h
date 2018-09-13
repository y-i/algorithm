#include <algorithm>
#include <vector>
#include <map>
#include <numeric>

// 1-indexed
template<typename T=int>
class SuccinctBitVector {
    int lsize = 1024, rsize = 32;
    std::vector<uint_least16_t> l;
    std::vector<uint8_t> r;
    int n;
    std::vector<int> a;
	int countbits(long long bits) const {
	    bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
	    bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
	    bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
	    bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
	    return (bits & 0x0000ffff) + (bits >>16 & 0x0000ffff);
	}
public:
    SuccinctBitVector(const int &n): 
        n(n), a(n), l((n + lsize - 1)/lsize), r((n + rsize - 1)/rsize) {
    }
    void init() {
        int cnt = 0;
        for (int i = 0; i < n; ++i) {
            if (i % lsize == 0) l[i/lsize] = cnt;
            if (i % rsize == 0) r[i/rsize] = cnt - l[i/lsize];
            if (a[i]) ++cnt;
        }
    }
    int access(const int &n) const {
        return a[n-1];
    }
    int rank(const T &target, const int &n) const {
        int bits = 0;
        for (int i = n - 1; i >= (n/rsize)*rsize; --i) {
            int shift = n-1-i;
            bits += ((a[i] > 0) << shift);
        }
        int rank1 = l[n/lsize] + r[n/rsize] + countbits(bits);
        if (target) return rank1;
        else return n - rank1;
    }
    int select(const T &target, const int &n) const {
        if (target) return select1(target, n);
        else return select0(target, n);
    }
    int select1(const T &target, const int &n) const {
        int ln,rn;
        {
            int ok = -1, ng = (a.size()+lsize-1)/lsize;
            while (ng - ok > 1) {
                int m = (ok + ng) / 2;
                if (l[m] < n) ok = m;
                else ng = m;
            }
            ln = ok;
        }
        {
            int ok = -1, ng = (a.size()+rsize-1)/rsize;
            while (ng - ok > 1) {
                int m = (ok + ng) / 2;
                if (l[ln] + r[m] < n) ok = m;
                else ng = m;
            }
            rn = ok;
        }
        int cnt = l[ln] + r[rn];
        for (int i = rn * rsize; i < (rn+1) * rsize; ++i) {
            if (a[i]) ++cnt;
            if (cnt == n) return i + 1;
        }
    }
    int select0(const T &target, const int &n) const {
        int ln,rn;
        {
            int ok = -1, ng = (a.size()+lsize-1)/lsize;
            while (ng - ok > 1) {
                int m = (ok + ng) / 2;
                if (m - l[m] < n) ok = m;
                else ng = m;
            }
            ln = ok;
        }
        {
            int ok = -1, ng = (a.size()+rsize-1)/rsize;
            while (ng - ok > 1) {
                int m = (ok + ng) / 2;
                if (m - (l[ln] + r[m]) < n) ok = m;
                else ng = m;
            }
            rn = ok;
        }
        int cnt = rn - (l[ln] + r[rn]);
        for (int i = rn * rsize; i < (rn+1) * rsize; ++i) {
            if (a[i] == 0) ++cnt;
            if (cnt == n) return i + 1;
        }
    }
    int &operator[](const int &index) {
        return a[index];
    }
    int operator[](const int &index) const {
        return a[index];
    }
    int size() const {
        return a.size();
    }
};

// 1-index
template<typename T=int>
class WaveletMatrix {
    std::vector<SuccinctBitVector<>> matrix;
    std::vector<int> border;
    std::map<int,T> start;
public:
    WaveletMatrix(std::vector<T> a): matrix(sizeof(T)*8, SuccinctBitVector<>(a.size())), border(sizeof(T)*8) {
        for (int i = 0; i < sizeof(T) * 8; ++i) {
            for (int j = 0; j < a.size(); ++j) matrix[i][j] = (a[j] >> ((sizeof(T)*8-1-i))) & 1;
            std::stable_sort(a.begin(),a.end(),[&i](auto &l, auto &r){
                return (l&(1<<(sizeof(T)*8-1-i))) < (r&(1<<(sizeof(T)*8-1-i)));
            });
            for (int j = 0; j < a.size() && (a[j]&(1<<(sizeof(T)*8-1-i))) == 0; ++j) ++border[i];
        }
        start[a[0]] = 0;
        for (int i = 1; i < a.size(); ++i) if (a[i] != a[i-1]) start[a[i]] = i;
    }
    void init() {
        for (auto &sbv: matrix) sbv.init();
    }
    T access(const int &n) const {
        int index = n;
        T ret = 0;
        for (int i = 0; i < matrix.size(); ++i) {
            ret += (matrix[i][index-1]) << (matrix.size() - i - 1);
            index = matrix[i].rank(matrix[i][index-1],index) + border[i] * (matrix[i][index-1] > 0);
        }
        return ret;
    }
    int rank(const T &target, const int &n) const {
        if (start.count(target) == 0) return 0;
        int index = n;
        for (int i = 0; i < matrix.size(); ++i) {
            T target_bit = (target >> (matrix.size() - 1 - i)) & 1;
            index = matrix[i].rank(target_bit, index);
            if (target_bit) index += border[i];
        }
        return index - start.at(target);
    }
    int select(const T &target, const int &n) const {
        if (start.count(target) == 0) return -1;
        int index = start.at(target) + n;
        index = matrix[matrix.size()-1].select(target & 1, index - border[matrix.size()-1] * ((target & 1) > 0));
        for (int i = matrix.size()-2; i >= 0; --i) {
            T target_bit = (target >> (matrix.size()-i-1)) & 1;
            index = matrix[i].select(target_bit, index - border[i] * target_bit);
        }
        return index;
    }
};
