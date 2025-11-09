# Travel Assistant CLI (C++ DSA project)

This is a simple command-line "Travel Assistant" that demonstrates DSA concepts (graphs and shortest path) in C++.

Features
- Load region data from `database/<Region>/` folders (already included).
- Parse `cities_list.txt`, `adjacency_matrix.txt`, `distance_matrix.txt` and service files (`hotels.txt`, `restaurants.txt`, `hospitals.txt`, `lodges.txt`).
- Shortest path computation using Dijkstra's algorithm.
- Search services and find nearest hospital heuristically.

Build (Windows PowerShell)

Prerequisite: a C++17-capable compiler. Example using MSYS2/MinGW or g++:

# Using g++ directly (no CMake):
# From project root run:
# g++ -std=c++17 -O2 -I. -o travel_assistant src/main.cpp

# Using CMake:
# mkdir build; cd build; cmake -G "MinGW Makefiles" ..; cmake --build .

Running

From project root (where the `database` folder is located):

# If you built using g++ and created travel_assistant.exe in the project root:
# .\travel_assistant.exe

# Or if using CMake and the executable is in build/:
# .\build\travel_assistant.exe

You can also pass a custom database path as the first argument:

# .\travel_assistant.exe "C:\path\to\database"

Usage
- Choose a region (e.g., Karachi).
- Use the menu to list cities, view services, compute shortest paths, search services.

Notes and next steps
- The program expects the data format already present in the `database` folder. It tolerates comment lines starting with `#` and blank lines.
- Improvements: better matching between service entries and cities (fuzzy match), add routing via lat/lon (Haversine), more advanced recommendations, unit tests.

If you want, I can now compile and run the program here to verify it. Let me know if you'd like that, or if you'd prefer additional features (e.g., interactive fuzzy search, route export).
