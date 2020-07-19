#include <vector>
#include <algorithm>
#include <memory>

template<typename T, typename C=std::greater<T>>
class BinomialHeap {
    struct Node;
    std::vector<std::shared_ptr<Node>> nodes;
    int n;
    std::shared_ptr<Node> merge(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) {
        if (C()(left->value, right->value)) {
            left->children.push_back(right);
            return left;
        } else {
            right->children.push_back(left);
            return right;
        }
    }

    struct Node {
        T value;
        std::vector<std::shared_ptr<Node>> children;
        Node(T value): value(value) {}
    };
public:
    BinomialHeap(): n(0) {};
    BinomialHeap(const std::vector<T> &init): ary(init), n(init.size()) {
        for (const T& v: init) push(v);
    }
    int size() const {
        return n;
    }
    bool empty() const {
        return n == 0;
    }
    const T& top() const {
        std::shared_ptr<Node> maxv =nullptr;
        for (const auto &node: nodes) {
            if (node == nullptr) continue;
            if (maxv == nullptr) {
                maxv = node;
                continue;
            }
            if (C()(node->value, maxv->value)) maxv = node;
        }
        return maxv->value;
    }
    void pop() {
        if (empty()) return;

        const T& maxv = top();
        std::vector<std::shared_ptr<Node>> children;
        int cnt = 0;
        for (auto &node: nodes) {
            ++cnt;
            if (node == nullptr) continue;
            if (maxv != node->value) continue;
            children = move(node->children);
            node = nullptr;
            break;
        }
        cnt -= 2;
        reverse(children.begin(), children.end());
        for (auto &node: children) {
            for (int idx = cnt; idx < nodes.size(); ++idx) {
                if (nodes[idx] == nullptr) {
                    nodes[idx] = node;
                    break;
                }
                node = merge(node, nodes[idx]);
                nodes[idx] = nullptr;
            }
            --cnt;
        }
        if (nodes.back() == nullptr) nodes.pop_back();
        --n;
    }
    void push(T value) {
        std::shared_ptr<Node> node = std::make_shared<Node>(value);
        ++n;
        if ((n & (n-1)) == 0) nodes.emplace_back(nullptr);
        for (int idx = 0; idx < nodes.size(); ++idx) {
            if (nodes[idx] == nullptr) {
                nodes[idx] = node;
                break;
            }
            node = merge(node, nodes[idx]);
            nodes[idx] = nullptr;
        }
    }
};
