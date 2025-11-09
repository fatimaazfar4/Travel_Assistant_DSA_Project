#include "dijkstra.h"
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

std::pair<std::vector<int>, double> dijkstra(
    const std::vector<std::vector<int>> &adj,
    const std::vector<std::vector<double>> &distmat,
    int src, int dst) {

    int n = adj.size();
    std::vector<double> dist(n, INF);
    std::vector<int> parent(n, -1);
    dist[src] = 0;

    using PDI = std::pair<double, int>;
    std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI>> pq;
    pq.push({0.0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (int v = 0; v < n; v++) {
            if (adj[u][v] != 0) {
                double w = distmat[u][v];
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    std::vector<int> path;
    if (dist[dst] == INF) return {path, INF}; // no path

    for (int v = dst; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    std::reverse(path.begin(), path.end());
    return {path, dist[dst]};
}
