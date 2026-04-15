#include <memory>

template<typename T, auto Op, auto E>
requires
    std::same_as<decltype(Op(std::declval<T>(), std::declval<T>())), T> &&
    std::same_as<decltype(E()), T>
class PersistentSegmentTree {
    class PersistentSegmentTreeNode {
    public:
        int l, r;
        T value;
        std::shared_ptr<PersistentSegmentTreeNode> left_child, right_child;
        PersistentSegmentTreeNode(int l, int r, T value) : l(l), r(r), value(value) {};
        [[nodiscard]] std::shared_ptr<PersistentSegmentTreeNode> copy() const {
            auto node = std::make_shared<PersistentSegmentTreeNode>(l, r, value);
            node->left_child = left_child;
            node->right_child = right_child;
            return node;
        }
    };

    int size;
    std::shared_ptr<PersistentSegmentTreeNode> root;

    void initialize_node_dfs(std::shared_ptr<PersistentSegmentTreeNode> node) {
        if (node->r - node->l == 1) return;

        int m = (node->l + node->r) / 2;
        node->left_child = std::make_shared<PersistentSegmentTreeNode>(node->l, m, E());
        node->right_child = std::make_shared<PersistentSegmentTreeNode>(m, node->r, E());
        initialize_node_dfs(node->left_child);
        initialize_node_dfs(node->right_child);
        node->value = Op(node->left_child->value, node->right_child->value);
    }
    void update(std::shared_ptr<PersistentSegmentTreeNode> node, int index, T value) {
        if (node->r - node->l == 1) {
            node->value = value;
            return;
        }

        int m = (node->l + node->r) / 2;
        if (index < m) {
            node->left_child = node->left_child->copy();
            update(node->left_child, index, value);
        } else {
            node->right_child = node->right_child->copy();
            update(node->right_child, index, value);
        }
        node->value = Op(node->left_child->value, node->right_child->value);
    }
    [[nodiscard]] T query(std::shared_ptr<PersistentSegmentTreeNode> node, int left, int right) const {
        if (node->r <= left || node->l >= right) return E();
        if (node->l >= left && node->r <= right) return node->value;
        T left_value = query(node->left_child, left, right);
        T right_value = query(node->right_child, left, right);
        return Op(left_value, right_value);
    }
public:
    PersistentSegmentTree(int size) : size(size) {
        int n = 1;
        while (n < size) n *= 2;
        root = std::make_shared<PersistentSegmentTreeNode>(0, n, E());
        initialize_node_dfs(root);
    }
    PersistentSegmentTree<T, Op, E> update(int index, T value) {
        auto tree = *this;
        tree.root = tree.root->copy();
        tree.update(tree.root, index, value);

        return tree;
    }
    [[nodiscard]] T query(int left, int right) const {
        return query(root, left, right);
    }
    [[nodiscard]] T get(int index) const {
        return query(index, index + 1);
    }
    [[nodiscard]] T all() const {
        return query(0, size);
    }
};
