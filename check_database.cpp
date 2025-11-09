#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::string base = "database";

    std::string files[] = {
        "cities_list.txt",
        "adjacency_matrix.txt",
        "distance_matrix.txt"
    };

    bool allGood = true;

    for (const auto& file : files) {
        std::ifstream fin(base + "\\" + file);
        if (!fin.is_open()) {
            std::cout << "[ERROR] File missing: " << file << "\n";
            allGood = false;
        } else if (fin.peek() == std::ifstream::traits_type::eof()) {
            std::cout << "[ERROR] File empty: " << file << "\n";
            allGood = false;
        } else {
            std::cout << "[OK] File exists and has content: " << file << "\n";
        }
        fin.close();
    }

    if (allGood) {
        std::cout << "\nAll database files are present and valid.\n";
    } else {
        std::cout << "\nPlease fix the missing/empty files.\n";
    }

    return 0;
}
