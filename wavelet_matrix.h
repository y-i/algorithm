#include "succinct_bit_vector.h"
#include <algorithm>
#include <map>
#include <vector>

// 1-index
template <typename T = int> class WaveletMatrix {
    std::vector<SuccinctBitVector<>> matrix;
    std::vector<int> border;
    std::map<int, T> start;

  public:
    WaveletMatrix(std::vector<T> a)
        : matrix(sizeof(T) * 8, SuccinctBitVector<>(a.size())),
          border(sizeof(T) * 8) {
        for (int i = 0; i < sizeof(T) * 8; ++i) {
            const T shift = 1 << (sizeof(T) * 8 - 1 - i);
            for (int j = 0; j < a.size(); ++j)
                matrix[i].set(j, a[j] & shift);
            std::stable_sort(a.begin(), a.end(),
                             [&i, &shift](auto &l, auto &r) {
                                 T lbit = l & shift, rbit = r & shift;
                                 return lbit < rbit;
                             });
            for (int j = 0; j < a.size() && (a[j] & shift) == 0; ++j)
                ++border[i];
        }
        start[a[0]] = 0;
        for (int i = 1; i < a.size(); ++i)
            if (start.count(a[i]) == 0) {
                start[a[i]] = i;
            }
    }

    void init() {
        for (auto &sbv : matrix)
            sbv.init();
    }

    T access(const int &n) const {
        int index = n;
        T ret = 0;
        // 上位bitから見てる
        for (int i = 0; i < matrix.size(); ++i) {
            ret += matrix[i][index - 1] << (sizeof(T) * 8 - 1 - i);
            index = matrix[i].rank(matrix[i][index - 1], index) +
                    border[i] * (matrix[i][index - 1] > 0);
        }
        return ret;
    }
    int rank(const T &target, const int &n) const {
        if (start.count(target) == 0)
            return 0;
        int index = n;
        // 上位bitから見てる
        for (int i = 0; i < matrix.size(); ++i) {
            index =
                matrix[i].rank(target & (1 << (matrix.size() - 1 - i)), index);
            if (target & (1 << (matrix.size() - 1 - i)))
                index += border[i];
        }
        return index - start.at(target);
    }
    int select(const T &target, const int &n) const {
        if (start.count(target) == 0)
            return -1;
        int index = start.at(target) + n;

        // 下位bitから見てる
        index = matrix[matrix.size() - 1].select(
            target & 1, index - border[matrix.size() - 1] * ((target & 1) > 0));
        for (int i = matrix.size() - 2; i >= 0; --i) {
            T target_bit = target & (1 << (matrix.size() - i - 1));

            index = matrix[i].select(target_bit,
                                     index - border[i] * (target_bit > 0));
        }

        if (access(index) != target)
            return -1;
        // 存在するより多いnを指定された時に上のどこかがバグってるのでrankで検証
        if (rank(target, index) != n)
            return -1;

        return index;
    }

    int size() const { return matrix[0].size(); }
};

