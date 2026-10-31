// graph.hpp - Stage 6: knowledge graph + Dijkstra search
// The graph is generated with a fixed-seed Park-Miller generator so the C++
// backend and the browser visualiser always build the identical 26-node graph.
#pragma once
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <vector>

namespace nt {

struct Edge { int to, w; };
struct EdgeRec { int a, b, w; };
struct SearchResult {
    std::vector<int> visitOrder;  // nodes in the order Dijkstra settled them
    std::vector<int> path;        // START -> target
    int cost = -1;
};

class KnowledgeGraph {
public:
    static constexpr int N = 26;
    double x[N], y[N];
    std::vector<EdgeRec> edges;
    std::vector<Edge> adj[N];

    KnowledgeGraph() {
        for (int i = 0; i < N; ++i) {            // x is drawn before y (order matters)
            double px = .06 + (i % 6) * .176 + (rnd() - .5) * .09;
            double py = .1 + (i / 6) * .2 + (rnd() - .5) * .08;
            x[i] = px; y[i] = py;
        }
        x[0] = .07; y[0] = .12;                  // START node
        for (int i = 1; i < N; ++i) {            // spanning tree -> graph is connected
            int best = 0; double bd = 9;
            for (int j = 0; j < i; ++j)
                if (dist(i, j) < bd) { bd = dist(i, j); best = j; }
            addEdge(i, best);
        }
        for (int i = 0; i < N; ++i) {            // 2 nearest neighbours each
            std::vector<int> idx;
            for (int j = 0; j < N; ++j) if (j != i) idx.push_back(j);
            std::stable_sort(idx.begin(), idx.end(),
                             [&](int a, int b) { return dist(i, a) < dist(i, b); });
            for (int k = 0; k < 2; ++k) addEdge(i, idx[k]);
        }
    }

    static char label(int i) { return static_cast<char>('A' + i); }

    SearchResult dijkstra(int src, int target) const {
        const int INF = 1000000000;
        std::vector<int> d(N, INF), prev(N, -1);
        std::vector<bool> done(N, false);
        SearchResult r;
        d[src] = 0;
        for (;;) {
            int u = -1;
            for (int i = 0; i < N; ++i)
                if (!done[i] && d[i] < INF && (u < 0 || d[i] < d[u])) u = i;
            if (u < 0) break;
            done[u] = true;
            r.visitOrder.push_back(u);
            if (u == target) break;
            for (const Edge& e : adj[u])
                if (!done[e.to] && d[u] + e.w < d[e.to]) { d[e.to] = d[u] + e.w; prev[e.to] = u; }
        }
        for (int v = target; v != -1; v = prev[v]) r.path.insert(r.path.begin(), v);
        r.cost = d[target] >= INF ? -1 : d[target];
        return r;
    }

private:
    std::uint64_t seed_ = 7;
    double rnd() { seed_ = (seed_ * 16807ULL) % 2147483647ULL; return double(seed_) / 2147483647.0; }
    double dist(int a, int b) const { return std::hypot(x[a] - x[b], (y[a] - y[b]) * .7); }
    void addEdge(int a, int b) {
        for (const Edge& e : adj[a]) if (e.to == b) return;
        int w = 1 + static_cast<int>(std::floor(rnd() * 9));
        edges.push_back({a, b, w});
        adj[a].push_back({b, w});
        adj[b].push_back({a, w});
    }
};

}  // namespace nt
