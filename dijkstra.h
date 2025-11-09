#pragma once
#include <vector>
#include <utility> // for std::pair

const double INF = 1e9;

// returns pair: {path as vector of indices, total distance}
std::pair<std::vector<int>, double> dijkstra(
    const std::vector<std::vector<int>> &adj,
    const std::vector<std::vector<double>> &distmat,
    int src, int dst);
