#include <queue>
#include <vector>
#include <type_traits>

template<class T> concept Numeric = std::is_arithmetic_v<T>;

template<Numeric T>
struct Edge {int to; T cap; int rev;};

template<Numeric T>
class Dinic {
    int n;
    std::vector<std::vector<Edge<T>>> G;
    std::vector<int> level, iter;

    void bfs(int src) {
        std::fill(level.begin(), level.end(), -1);

        std::queue<int> q;
        level[src] = 0;
        q.push(src);
        while (!q.empty()) {
            int idx = q.front(); q.pop();
            for (const auto& edge : G[idx]) {
                if (edge.cap > 0 && level[edge.to] < 0) {
                    level[edge.to] = level[idx] + 1;
                    q.push(edge.to);
                }
            }
        }
    }

    T dfs(int idx, int dst, T f) {
        if (idx == dst) return f;

        for (int &i = iter[idx]; i < G[idx].size(); i++) {
            auto &edge = G[idx][i];
            if (edge.cap > 0 && level[idx] < level[edge.to]) {
                T d = dfs(edge.to, dst, std::min(f, edge.cap));
                if (d > 0) {
                    edge.cap -= d;
                    G[edge.to][edge.rev].cap += d;
                    return d;
                }
            }
        }

        return 0;
    }
public:
    Dinic(int n) : n(n), G(n), level(n), iter(n) {}

    void add_edge(int from, int to, T cap) {
        G[from].emplace_back(to, cap, G[to].size());
        G[to].emplace_back(from, 0, G[from].size() - 1);
    }

    T max_flow(int src, int dst) {
        T flow = 0;
        while (true) {
            bfs(src);
            if (level[dst] < 0) return flow;

            std::fill(iter.begin(), iter.end(), 0);

            T f;
            while ((f = dfs(src, dst, std::numeric_limits<T>::max())) > 0) {
                flow += f;
            }
        }
    }
};
