#include <algorithm>
#include <utility>
#include <memory>
#include <vector>

#include <iostream>
using std::cerr;
using std::endl;

template<typename T=long long, typename U=long long>
class BTree {
    class Node;
    using node_ptr = std::shared_ptr<Node>;
    class Node {
    public:
        const int m;
        std::vector<std::pair<T, U>> keys;
        std::vector<node_ptr> branches;
    public:
        Node(const int &m): m(m), keys(0), branches(1, nullptr) {}
        U get_key(const int &index) const {return keys[index];}
        bool has(const T &key) const {
            auto it = std::lower_bound(keys.begin(), keys.end(), std::pair<T, U>{key,0}, [](auto &l, auto &r) {
                return l.first < r.first;
            });

            if (it->first == key) return true;
            int d = std::distance(keys.begin(), it);
            if (branches[d] == nullptr) return false;
            return branches[d]->has(key);
        }
        U find(const T &key) const {
            auto it = std::lower_bound(keys.begin(), keys.end(), std::pair<T, U>{key,0}, [](auto &l, auto &r) {
                return l.first < r.first;
            });

            if (it->first == key) return it->second;
            int d = std::distance(keys.begin(), it);
            if (branches[d] == nullptr) throw "Not Found";
            return branches[d]->find(key);
        }
        // TODO: insertとupdateを定義して内部でupsertを使う(true,true)
        bool insert(const T &key, const U &value) {
            auto it = std::lower_bound(keys.begin(), keys.end(), std::pair<T, U>{key,0}, [](auto &l, auto &r) {
                return l.first < r.first;
            });
            if (it != keys.end() && it->first == key) throw "Already Exist";
            int d = std::distance(keys.begin(), it);
            if (branches[d] == nullptr) {
                keys.emplace(keys.begin() + d, key, value);
                branches.insert(branches.begin() + d, nullptr);
            } else {
                bool is_active = branches[d]->insert(key, value);

                // あふれた場合新しく作った子供を自身に追加
                if (is_active) {
                    node_ptr active_ptr = branches[d];
                    keys.insert(keys.begin() + d, std::make_pair(active_ptr->keys[0].first, active_ptr->keys[0].second));
                    branches.insert(branches.begin() + d + 1, active_ptr->branches[1]);
                    branches[d] = active_ptr->branches[0];
                }
            }

            if (branches.size() <= m) return false;

            // サイズを超えたら分割
            int mid = m / 2;
            T root_key = keys[mid].first;
            U root_value = keys[mid].second;

            node_ptr left_node = std::make_shared<Node>(m);
            node_ptr right_node = std::make_shared<Node>(m);

            // 左の子になるnodeの作成
            left_node->keys.resize(mid); // 要らないかも
            std::move(keys.begin(), keys.begin() + mid, left_node->keys.begin());

            left_node->branches.resize(mid + 1); // 要らないかも
            std::move(branches.begin(), branches.begin() + mid + 1, left_node->branches.begin());

            // 右の子になるnodeの作成
            right_node->keys.resize(m - mid - 1); // 要らないかも
            std::move(keys.begin() + mid + 1, keys.begin() + m, right_node->keys.begin());

            right_node->branches.resize(m - mid); // 要らないかも
            std::move(branches.begin() + mid + 1, branches.begin() + m + 1, right_node->branches.begin());

            this->keys.clear();
            this->branches.clear();

            this->keys.emplace_back(root_key, root_value);
            this->branches.emplace_back(left_node);
            this->branches.emplace_back(right_node);

            return true;
        }

        std::vector<std::pair<T, U>> debug_print() {
            std::vector<std::pair<T, U>> ret;
            for (int i = 0; i < keys.size(); ++i) {
                if (branches[i] != nullptr) {
                    auto tmp = branches[i]->debug_print();
                    for (const auto &p: tmp) ret.push_back(p);
                }
                ret.push_back(keys[i]);
            }
            if (branches[keys.size()] != nullptr) {
                auto tmp = branches[keys.size()]->debug_print();
                for (const auto &p: tmp) ret.push_back(p);
            }
            return ret;
        }
    };

    int m;
    node_ptr root;
public:
    BTree(): m(4), root(new Node(m)) {}
    bool has(const T &key) {
        return root->has(key);
    }
    U find(const T &key) const {
        return root->find(key);
    }
    void insert(const T &key, const U &value) {
        root->insert(key, value);
    }

    std::vector<std::pair<T, U>> debug() const {
        return root->keys;
    }
    std::vector<std::pair<T, U>> debug_print() {
        return root->debug_print();
    }
};
