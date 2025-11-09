#pragma once
#include <vector>
#include <string>
#include <utility>

extern const double INF;

// File loaders
std::vector<std::string> loadCities(const std::string &path);
std::vector<std::vector<int>> loadAdjacency(const std::string &path);
std::vector<std::vector<double>> loadDistances(const std::string &path);

// Dijkstra
std::pair<std::vector<int>, double> dijkstra(
    const std::vector<std::vector<int>> &adj,
    const std::vector<std::vector<double>> &distmat,
    int src, int dst);

// POI loaders
std::vector<std::string> loadPoiForCity(const std::string &city, const std::string &type);
std::vector<std::string> loadLodgesForCity(const std::string &city);
std::vector<std::string> loadRestaurantsForCity(const std::string &city);
std::vector<std::string> loadHospitalsForCity(const std::string &city);

// Display helpers
void printCities(const std::vector<std::string> &cities);
void printShortestRoute(const std::vector<std::string> &cities,
                        const std::vector<int> &path,
                        double totalDistance);

// CLI menu helpers
int promptIndex(const std::vector<std::string> &cities, const std::string &prompt);
void runSimpleMenu(const std::vector<std::string> &cities,
                   const std::vector<std::vector<int>> &adj,
                   const std::vector<std::vector<double>> &distmat);
