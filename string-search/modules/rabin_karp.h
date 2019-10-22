#include <random>
#include <string>
#include <vector>
#include <iostream>

// ref: https://qiita.com/keymoon/items/11fac5627672a6d6a9f6
class RabinKarp {
    const unsigned long long MASK30 = (1ull << 30) - 1;
    const unsigned long long MASK31 = (1ull << 31) - 1;
    const unsigned long long MOD = (1ull << 61) - 1;
    const unsigned long long POSITIVIZER = MOD * 3;
    unsigned int base;
    std::vector<unsigned long long> _hash, _pow;

    //a*b mod 2^61-1返す関数
    unsigned long long mul(unsigned long long a, unsigned long long b) const
    {
        unsigned long long au = a >> 31;
        unsigned long long ad = a & MASK31;
        unsigned long long bu = b >> 31;
        unsigned long long bd = b & MASK31;
        unsigned long long mid = ad * bu + au * bd;
        unsigned long long midu = mid >> 30;
        unsigned long long midd = mid & MASK30;
        return au * bu * 2 + midu + (midd << 31) + ad * bd;
    }

    //mod 2^61-1を計算する関数
    unsigned long long calcMod(unsigned long long val) const
    {
        val = (val & MOD) + (val >> 61);
        if (val > MOD) val -= MOD;
        return val;
    }

    void preprocess(const std::string &s, std::vector<unsigned long long> &hash, std::vector<unsigned long long> &pow) const {
        int len = s.size();
        pow[0] = 1;
        for (int i = 0; i < len; ++i) {
            hash[i+1] = calcMod(mul(hash[i], base) + s[i]);
            pow[i+1] = calcMod(mul(pow[i], base));
        }
    }

    unsigned long long hash(const int l, const int r) const {
        return calcMod(_hash[r] + POSITIVIZER - mul(_hash[l], _pow[r-l]));
    }

public:
    RabinKarp(const std::string &s): _hash(s.size()+1), _pow(s.size()+1) {
        std::random_device rng;
        base = rng() % ((1LL << 31) - 1 - 10001) + 10001;
        preprocess(s, _hash, _pow);
    }

    bool match(int l1, int r1, int l2, int r2) const {
        return hash(l1,r1) == hash(l2,r2);
    }

    bool match(int l, int r, unsigned long long h) const {
        return hash(l,r) == h;
    }

    std::string::size_type find(const std::string &pattern) const {
        int n = pattern.size(), m = _hash.size();
        std::vector<unsigned long long> hash(n+1), pow(n+1);
        preprocess(pattern, hash, pow);
        for (int i = 0; i <= m - n; ++i) {
            if (match(i,i+n,hash[n])) return i;
        }
        return std::string::npos;
    }
};
