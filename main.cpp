#include <iostream>
#include "utils.h"

int main() {
    std::string base = "database";
    auto cities = loadCities(base + "/cities_list.txt");
    auto adj = loadAdjacency(base + "/adjacency_matrix.txt");
    auto distmat = loadDistances(base + "/distance_matrix.txt");

    if (cities.empty() || adj.empty() || distmat.empty()) {
        std::cerr << "Database files missing or corrupted.\n";
        return 1;
    }

    runSimpleMenu(cities, adj, distmat);
    return 0;
}
