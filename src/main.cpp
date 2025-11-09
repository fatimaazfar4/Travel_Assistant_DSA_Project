#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cctype>
#include <dirent.h>
#include <sys/stat.h>
using namespace std;

const double INF = 1e18;

struct City {
    int id;
    string name;
    double lat, lon;
};

struct RegionData {
    vector<City> cities;
    vector<vector<double>> distMatrix;
    vector<vector<int>> adjMatrix;
    map<string, vector<string>> services; // key: hotels/restaurants/hospitals/lodges -> lines
};

static string trim(const string &s){
    size_t a = 0; while(a < s.size() && isspace((unsigned char)s[a])) ++a;
    size_t b = s.size(); while(b>a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b-a);
}

vector<string> split(const string &s, char delim){
    vector<string> out;
    string cur;
    for(char c: s){
        if(c==delim){ out.push_back(cur); cur.clear(); }
        else cur.push_back(c);
    }
    out.push_back(cur);
    return out;
}

bool loadCities(const string &p, RegionData &rd){
    string f = p + "/cities_list.txt";
    ifstream test(f.c_str());
    if(!test) return false;
    test.close();
    ifstream in(f);
    string line;
    rd.cities.clear();
    while(getline(in, line)){
        line = trim(line);
        if(line.empty()) continue;
        if(line[0]=='#') continue;
        // expected: id name lat lon (name may contain underscores)
        stringstream ss(line);
        City c; ss >> c.id >> c.name >> c.lat >> c.lon;
        rd.cities.push_back(c);
    }
    return true;
}

bool loadMatrixDouble(const string &p, const string &filename, vector<vector<double>> &mat){
    string f = p + "/" + filename;
    ifstream test(f.c_str());
    if(!test) return false;
    test.close();
    ifstream in(f);
    string line;
    mat.clear();
    while(getline(in, line)){
        line = trim(line);
        if(line.empty()) continue;
        if(line.size()>0 && line[0]=='#') continue;
        stringstream ss(line);
        vector<double> row;
        double x;
        while(ss >> x) row.push_back(x);
        if(!row.empty()) mat.push_back(row);
    }
    return !mat.empty();
}

bool loadMatrixInt(const string &p, const string &filename, vector<vector<int>> &mat){
    string f = p + "/" + filename;
    ifstream test(f.c_str());
    if(!test) return false;
    test.close();
    ifstream in(f);
    string line;
    mat.clear();
    while(getline(in, line)){
        line = trim(line);
        if(line.empty()) continue;
        if(line.size()>0 && line[0]=='#') continue;
        stringstream ss(line);
        vector<int> row;
        int x;
        while(ss >> x) row.push_back(x);
        if(!row.empty()) mat.push_back(row);
    }
    return !mat.empty();
}

void loadServices(const string &p, RegionData &rd){
    static vector<string> keys = {"hotels.txt","restaurants.txt","hospitals.txt","lodges.txt"};
    for(auto &k: keys){
        string f = p + "/" + k;
        rd.services[k] = {};
        ifstream test(f.c_str());
        if(!test) continue;
        test.close();
        ifstream in(f);
        string line;
        while(getline(in, line)){
            line = trim(line);
            if(line.empty()) continue;
            rd.services[k].push_back(line);
        }
    }
}

RegionData loadRegion(const string &p){
    RegionData rd;
    if(!loadCities(p, rd)){
        cerr << "Failed to load cities_list.txt from " << p << "\n";
    }
    loadMatrixDouble(p, "distance_matrix.txt", rd.distMatrix);
    loadMatrixInt(p, "adjacency_matrix.txt", rd.adjMatrix);
    loadServices(p, rd);
    return rd;
}

// Dijkstra using distance matrix as weights when > 0, else ignore
pair<vector<double>, vector<int>> dijkstra(const RegionData &rd, int src){
    int n = rd.cities.size();
    vector<double> dist(n, INF);
    vector<int> prev(n, -1);
    dist[src]=0;
    using P = pair<double,int>;
    priority_queue<P, vector<P>, greater<P>>pq;
    pq.push({0,src});
    while(!pq.empty()){
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if(d>dist[u]) continue;
        for(int v=0; v<n; ++v){
            double w = INF;
            if(!rd.distMatrix.empty() && (int)rd.distMatrix.size()==n) {
                w = rd.distMatrix[u][v];
            }
            // if adjacency exists and distMatrix has 0, skip
            if(!rd.adjMatrix.empty() && (int)rd.adjMatrix.size()==n){
                if(rd.adjMatrix[u][v]==0) continue;
                if(w==0) {
                    // some files use adjacency only; assign weight 1.0
                    w = 1.0;
                }
            } else {
                if(w==0) continue;
            }
            if(w<=0 || w>=INF) continue;
            if(dist[v] > dist[u] + w){
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return {dist, prev};
}

vector<int> reconstructPath(const vector<int>& prev, int t){
    vector<int> path;
    for(int v=t; v!=-1; v=prev[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}

void showRegionMenu(const string &regionName, RegionData &rd){
    while(true){
        cout << "\n--- Region: " << regionName << " ---\n";
        cout << "1) List cities\n";
        cout << "2) Show city services (hotels/restaurants/hospitals/lodges)\n";
        cout << "3) Shortest path between two cities\n";
        cout << "4) Find nearest hospital to a city\n";
        cout << "5) Search services by keyword across cities\n";
        cout << "6) Back to region selection\n";
        cout << "Choose option: ";
        int opt; if(!(cin>>opt)){ cin.clear(); string tmp; getline(cin,tmp); continue; }
        if(opt==1){
            cout << "Cities:\n";
            for(auto &c: rd.cities) cout << c.id << ": " << c.name << "\n";
        } else if(opt==2){
            cout << "Enter city id: "; int id; cin >> id;
            if(id<0 || id >= (int)rd.cities.size()){ cout<<"Invalid id\n"; continue; }
            cout << "Services in " << rd.cities[id].name << ":\n";
            for(auto key: {string("hotels.txt"), string("restaurants.txt"), string("hospitals.txt"), string("lodges.txt")}){
                cout << "\n" << key << ":\n";
                auto &vec = rd.services[key];
                bool any=false;
                for(auto &line: vec){
                    // simple heuristic: if city name appears in line
                    if(line.find(rd.cities[id].name)!=string::npos || line.find('_')==string::npos){
                        // we will still show all lines but mark those belonging to city
                    }
                }
                // Print all entries but highlight city matches
                for(auto &line: vec){
                    bool match = (line.find(rd.cities[id].name)!=string::npos);
                    if(match) cout << "* "; else cout << "  ";
                    cout << line << "\n";
                }
            }
        } else if(opt==3){
            cout << "Enter source city id: "; int s,t; cin>>s;
            cout << "Enter target city id: "; cin>>t;
            if(s<0||s>= (int)rd.cities.size()|| t<0||t>=(int)rd.cities.size()){ cout<<"Invalid ids\n"; continue; }
            pair<vector<double>, vector<int>> result = dijkstra(rd, s);
            vector<double>& dist = result.first;
            vector<int>& prev = result.second;
            if(dist[t]>=INF) cout<<"No path found.\n";
            else{
                cout << "Distance: " << dist[t] << "\n";
                auto path = reconstructPath(prev, t);
                cout << "Path: ";
                for(size_t i=0;i<path.size();++i){ cout << rd.cities[path[i]].name; if(i+1<path.size()) cout << " -> "; }
                cout << "\n";
            }
        } else if(opt==4){
            cout << "Enter city id: "; int id; cin>>id;
            if(id<0||id>=(int)rd.cities.size()){ cout<<"Invalid id\n"; continue; }
            // find hospitals entries and attempt to match city names to lines
            auto &hvec = rd.services["hospitals.txt"];
            if(hvec.empty()){ cout<<"No hospital data available.\n"; continue; }
            // simple approach: find hospital entries that contain city name; if multiple choose first; else compute nearest city that has hospital
            vector<int> citiesWithHospital;
            for(int i=0;i<(int)rd.cities.size(); ++i){
                string nm = rd.cities[i].name;
                for(auto &line: hvec){ if(line.find(nm)!=string::npos){ citiesWithHospital.push_back(i); break; } }
            }
            if(!citiesWithHospital.empty()){
                // compute distance from id to each and pick min
                int best=-1; double bestd=INF;
                for(int c: citiesWithHospital){
                    double d = INF;
                    if(!rd.distMatrix.empty()) d = rd.distMatrix[id][c];
                    if(d==0) {
                        // compute via dijkstra
                        auto pr = dijkstra(rd, id);
                        if(pr.first[c]<bestd){ bestd=pr.first[c]; best=c; }
                    } else {
                        if(d<bestd){ bestd=d; best=c; }
                    }
                }
                if(best==-1) cout<<"Could not determine nearest hospital.\n";
                else cout<<"Nearest hospital is in "<< rd.cities[best].name <<" (approx "<< bestd <<" km)\n";
            } else {
                cout<<"No hospitals matched to cities by name. Showing nearest city by dijkstra.\n";
                auto pr = dijkstra(rd, id);
                double bestd=INF; int best=-1;
                for(int i=0;i<(int)pr.first.size();++i){
                    // if city has any hospital line
                    bool has=false;
                    string nm = rd.cities[i].name;
                    for(auto &line: hvec) if(line.find(nm)!=string::npos){ has=true; break; }
                    if(has && pr.first[i]<bestd){ bestd=pr.first[i]; best=i; }
                }
                if(best==-1) cout<<"No hospital entries found.\n";
                else cout<<"Nearest hospital is in "<< rd.cities[best].name <<" (approx "<< bestd <<" km)\n";
            }
        } else if(opt==5){
            cout << "Enter keyword to search (e.g., 'Clifton' or 'Marriott'): "; string kw; cin >> ws; getline(cin, kw);
            kw = trim(kw);
            cout << "Results:\n";
            for(auto &p: rd.services){
                for(auto &line: p.second){
                    if(line.find(kw)!=string::npos){
                        cout << "["<< p.first <<"] "<< line << "\n";
                    }
                }
            }
        } else if(opt==6) break;
        else cout << "Unknown option\n";
    }
}

int main(int argc, char** argv){
    cout << "Travel Assistant CLI (C++ DSA project)\n";
    string dbpath = "database";
    if(argc>1) dbpath = argv[1];
    
    DIR* dir = opendir(dbpath.c_str());
    if(!dir){
        cerr << "Could not find 'database' directory at " << dbpath << "\n";
        cerr << "Please ensure the folder 'database' (with region subfolders) is next to the executable or pass path as argument.\n";
        return 1;
    }
    vector<pair<string, string>> regions;
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if(entry->d_name[0] == '.') continue; // Skip . and ..
        string fullpath = dbpath + "/" + entry->d_name;
        struct stat statbuf;
        if(stat(fullpath.c_str(), &statbuf) != 0) continue;
        if(S_ISDIR(statbuf.st_mode)){
            regions.push_back({entry->d_name, fullpath});
        }
    }
    closedir(dir);
    if(regions.empty()){
        cerr<<"No regions found in database folder.\n";
        return 1;
    }
    while(true){
        cout << "\nAvailable regions:\n";
        for(size_t i=0;i<regions.size();++i) cout << i << ") " << regions[i].first << "\n";
        cout << "q) Quit\n";
        cout << "Select region index: ";
        string sel; cin >> sel;
        if(sel=="q"||sel=="Q") break;
        int idx;
        try{ idx = stoi(sel); } catch(...) { cout<<"Invalid input\n"; continue; }
        if(idx<0||idx>=(int)regions.size()){ cout<<"Invalid index\n"; continue; }
        auto regionName = regions[idx].first;
        auto path = regions[idx].second;
        RegionData rd = loadRegion(path);
        showRegionMenu(regionName, rd);
    }
    cout<<"Goodbye\n";
    return 0;
}
