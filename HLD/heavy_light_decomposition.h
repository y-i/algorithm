#include <vector>
#include <utility>

template <typename R>
concept EdgeList =
    std::ranges::random_access_range<R> &&
    std::ranges::random_access_range<std::ranges::range_value_t<R>> &&
    std::same_as<
        std::ranges::range_value_t<std::ranges::range_value_t<R>>,
        int
    >;

template <EdgeList R>
std::pair<std::vector<int>, std::vector<int>> hld(R& edges) {
    int n = std::ranges::size(edges);

    std::vector<int> sub(n);
    auto dfs1 = [&](auto self, int index, int parent) -> int {
        auto &size = sub[index];
        size = 1;
        for (int next : edges[index]) {
            if (next == parent) continue;
            size += self(self, next, index);
            if (sub[next] > sub[edges[index][0]]) {
                std::swap(next, edges[index][0]);
            }
        }
        return sub[index] = size;
    };
    dfs1(dfs1, 0, -1);

    std::vector<int> order(n), head(n);
    int i = 0;
    auto dfs2 = [&](auto self, int index, int parent) -> void {
        order[index] = i++;
        for (int next : edges[index]) {
            if (next == parent) continue;
            head[next] = (next == edges[index][0] ? head[index] : next);
            self(self, next, index);
        }
    };
    dfs2(dfs2, 0, -1);

    return {order, head};
}
