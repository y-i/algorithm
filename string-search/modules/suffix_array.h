#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

class SuffixArray {
    std::string s;
    int n;
    std::vector<int> suffixArray;

    // O(N^2 log N)
    std::vector<int> createSuffixArrayNaive() {
        std::vector<std::pair<std::string, int>> suffixArrayPair;
        for (int i = 0; i <= s.size(); ++i) suffixArrayPair.emplace_back(s.substr(i), i);
        std::sort(suffixArrayPair.begin(), suffixArrayPair.end());

        for (auto &p: suffixArrayPair) suffixArray.push_back(p.second);

        return suffixArray;
    }
public:
    SuffixArray(const std::string &s): s(s), n(s.size()) {
        createSuffixArrayNaive();
    }

    std::string::size_type find(const std::string &pattern)  const {
        int n = s.size(), m = pattern.size();
        int ok = n + 1, ng = -1;
        while (ok - ng > 1) {
            int mid = (ok + ng) / 2;
            int cmp = pattern.compare(s.substr(suffixArray[mid]));
            if (cmp == 0) return suffixArray[mid];
            if (cmp == -1) ok = mid;
            else ng = mid;
        }

        for (int i = 0; i < m; ++i) {
            if (pattern[i] != s[i + suffixArray[ok]]) return std::string::npos;
        }
        return suffixArray[ok];
    }
};


