#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

// 1-indexed
template <typename T = int> class SuccinctBitVector {
    // N = 2^32 -> (log N)^2, log(N)/2
    // const int chunk_size = 1024, block_size = 16;
    const int chunk_size = 64, block_size = 16;
    const T n;

    std::vector<T> chunk;
    std::vector<uint16_t> block;

    std::vector<uint16_t> ary;
    int countbits(long long bits) const {
        bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
        bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
        bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
        bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
        return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);
    }
    int getNeedBox(const int div) const { return (n + div - 1) / div; }

    T rank(int n) const {
        --n;
        int ans = 0;
        ans += chunk[n / chunk_size];
        ans += block[n / block_size];
        ans += countbits(uint16_t(ary[n / 16] << (16 - 1 - n % 16)));

        return ans;
    }

  public:
    SuccinctBitVector(const T n)
        : n(n), ary(getNeedBox(16)), chunk(getNeedBox(chunk_size)),
          block(getNeedBox(block_size)) {
        assert((1LL << (sizeof(T) * 8)) >= n);
        assert((1LL << (sizeof(uint16_t) * 8)) >= chunk_size);
    }

    // 0-indexed
    // Before init
    void set(int index, T value) {
        ary[index / 16] &= (~(1 << (index % 16)));
        if (value != 0)
            ary[index / 16] |= 1 << (index % 16);
    }

    void init() {
        const int block_per_chunk = chunk_size / block_size,
                  byte_per_block = block_size / 16;
        for (int i = 0; i < chunk.size(); ++i) {
            const int offset = i * block_per_chunk;
            for (int j = 0; j < block_per_chunk; ++j) {
                const int target = offset + j;
                int sum = 0;
                for (int k = 0; k < byte_per_block; ++k) {
                    if (target >= block.size() ||
                        target * byte_per_block + k >= ary.size())
                        break;

                    sum += countbits(ary[target * byte_per_block + k]);
                }
                if (j + 1 != block_per_chunk)
                    block[target + 1] = sum;
                if (i + 1 != chunk.size())
                    chunk[i + 1] += sum;
            }
            for (int j = 1; j < block_per_chunk; ++j) {
                block[offset + j] += block[offset + j - 1];
            }
        }
        for (int i = 1; i < chunk.size(); ++i) {
            chunk[i] += chunk[i - 1];
        }
    }
    void init(std::vector<bool> b) {
        for (int i = 0; i < n; ++i) {
            if (!b[i])
                continue;
            ary[i / 16] += 1 << (i % 16);
        }

        init();
    }

    bool access(const int n) const { return (*this)[n - 1]; }
    T rank(const bool &target, int n) const {
        if (target)
            return rank(n);
        else
            return n - rank(n);
    }
    int select(const T &target, const int n) const {
        int ok = this->n;
        int ng = -1;

        while (ok - ng > 1) {
            int mid = ng + (ok - ng) / 2;
            if (rank(target, mid) >= n)
                ok = mid;
            else
                ng = mid;
        }
        return ok;
    }

    bool operator[](const int index) {
        return (ary[index / 16] & (1 << (index % 16))) != 0;
    }
    const bool operator[](const int index) const {
        return (ary[index / 16] & (1 << (index % 16))) != 0;
    }
    int size() const { return n; }
};