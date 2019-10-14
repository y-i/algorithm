#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

class BoyerMoore {
    int n;
    std::string pattern;
    std::vector<std::unordered_map<char,int>> bad;
    std::vector<int> l, h;

    void create_bad_array() {
        for (int i = 0; i < pattern.size() - 1; ++i) {
            for (const auto &p: bad[i]) {
                const char &c = p.first;
                bad[i+1][c] = bad[i][c];
            }
            bad[i+1][pattern[i]] = i;
        }
    }
    void create_suffix_array() {

    }
public:
    BoyerMoore(std::string pattern): n(pattern.size()), pattern(pattern), bad(n), l(n), h(n) {
        create_bad_array();
        create_suffix_array();
    }
    int find(std::string text) const {
        int m = text.size();
        if (n > m) return -1;
        int k = n;
        while (k < m && k >= n - 1) {
            int i = n - 1;
            for (; i >= 0; --i) {
                if (text[k - n + i] != pattern[i]) {
                    // miss matching
                    int km = 0, ks = 0;
                    if (bad[i].count(text[k - n + i]) > 0) {
                        km = i - bad[i].at(text[k - n + i]);
                    } else {
                        km = i + 1;
                    }
                    // TODO: suffix matching

                    k += std::max(ks, km);
                    break;
                }
            }
            if (i == -1) return k - n;
        }
        return -1;
    }
    static int find(std::string pattern, std::string text) {
        BoyerMoore bm(pattern);
        return bm.find(text);
    }
};
