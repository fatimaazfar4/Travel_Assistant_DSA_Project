#include "dijkstra.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// ---------------------- file reading ----------------------
vector<string> loadCities(const string &path) {
    vector<string> cities;
    ifstream fin(path);
    string line;
    while (getline(fin, line)) {
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (!line.empty()) cities.push_back(line);
    }
    return cities;
}

vector<vector<int>> loadAdjacency(const string &path) {
    vector<vector<int>> adj;
    ifstream fin(path);
    string line;
    while (getline(fin, line)) {
        vector<int> row;
        istringstream iss(line);
        int val;
        while (iss >> val) row.push_back(val);
        if (!row.empty()) adj.push_back(row);
    }
    return adj;
}

vector<vector<double>> loadDistances(const string &path) {
    vector<vector<double>> distmat;
    ifstream fin(path);
    string line;
    while (getline(fin, line)) {
        vector<double> row;
        istringstream iss(line);
        double val;
        while (iss >> val) row.push_back(val);
        if (!row.empty()) distmat.push_back(row);
    }
    return distmat;
}

// ---------------------- POI loading ----------------------
vector<string> loadPoiForCity(const string &city, const string &type) {
    vector<string> poi;
    string filename = "database/" + city + "_" + type + ".txt";
    ifstream fin(filename);
    string line;
    while (getline(fin, line)) {
        if (!line.empty()) poi.push_back(line);
    }
    return poi;
}

vector<string> loadLodgesForCity(const string &city) { return loadPoiForCity(city, "lodges"); }
vector<string> loadRestaurantsForCity(const string &city) { return loadPoiForCity(city, "restaurants"); }
vector<string> loadHospitalsForCity(const string &city) { return loadPoiForCity(city, "hospitals"); }

// ---------------------- printing ----------------------
void printCities(const vector<string> &cities) {
    for (size_t i = 0; i < cities.size(); i++)
        cout << i << ". " << cities[i] << "\n";
}

void printShortestRoute(const vector<string> &cities,
                        const vector<int> &path,
                        double totalDistance) {
    if (path.empty()) {
        cout << "No route found!\n";
        return;
    }
    cout << "Shortest route:\n";
    for (size_t i = 0; i < path.size(); i++) {
        cout << cities[path[i]];
        if (i != path.size() - 1) cout << " -> ";
    }
    cout << "\nTotal distance: " << totalDistance << " km\n";
}

// ---------------------- menu helpers ----------------------
int promptIndex(const vector<string> &cities, const string &prompt) {
    int idx;
    cout << prompt;
    cin >> idx;
    while (idx < 0 || idx >= (int)cities.size()) {
        cout << "Invalid. Try again: ";
        cin >> idx;
    }
    return idx;
}

void runSimpleMenu(const vector<string> &cities,
                   const vector<vector<int>> &adj,
                   const vector<vector<double>> &distmat) {
    while (true) {
        cout << "\n--- Travel Assistant Menu ---\n";
        cout << "1. Show cities\n";
        cout << "2. Find shortest route\n";
        cout << "3. Show POIs\n";
        cout << "0. Exit\n";
        int choice; cin >> choice;

        if (choice == 0) break;
        if (choice == 1) {
            printCities(cities);
        } else if (choice == 2) {
            int src = promptIndex(cities, "Enter source index: ");
            int dst = promptIndex(cities, "Enter destination index: ");
            auto [path, dist] = dijkstra(adj, distmat, src, dst);
            printShortestRoute(cities, path, dist);
        } else if (choice == 3) {
            int cityIdx = promptIndex(cities, "Enter city index: ");
            string city = cities[cityIdx];
            cout << "--- Lodges ---\n";
            for (auto &l : loadLodgesForCity(city)) cout << l << "\n";
            cout << "--- Restaurants ---\n";
            for (auto &r : loadRestaurantsForCity(city)) cout << r << "\n";
            cout << "--- Hospitals ---\n";
            for (auto &h : loadHospitalsForCity(city)) cout << h << "\n";
        } else {
            cout << "Invalid choice.\n";
        }
    }
}
