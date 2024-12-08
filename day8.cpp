
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <ranges>
#include <unordered_map>

using dtype = int;

struct Pos {
    dtype x;
    dtype y;

    Pos() = default;
    Pos(dtype x, dtype y): x(x), y(y){};

    [[nodiscard]] Pos delta(const Pos& other) const {
        return {this->x-other.x, this->y-other.y};
    }

    [[nodiscard]] Pos add(const Pos& other) const {
        return {this->x+other.x, this->y+other.y};
    }

    [[nodiscard]] Pos sub(const Pos& other) const {
        return {this->x-other.x, this->y-other.y};
    }

    [[nodiscard]] bool in_bounds(int nrows, int ncols) const {
        return x >= 0 && x < ncols && y >= 0 && y < nrows;
    }
    bool operator==(const Pos &other) const {
        return this->x == other.x && this->y == other.y;
    }
};

struct pos_hash {
    size_t operator()(const Pos& p) const{
        size_t hash1 = std::hash<dtype>{}(p.x);
        // Hash the second element
        size_t hash2 = std::hash<dtype>{}(p.y);
        // Combine the two hash values
        return hash1
               ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
                  + (hash1 >> 2));
    }
};

using Map = std::unordered_map<char, std::vector<Pos>>;


std::tuple<Map, int, int> getInput(const std::string& fp){
    std::ifstream file (fp);
    Map out;
    int row = 0;
    size_t cols = 0;
    while(file){
        std::string tmp;

        std::getline(file, tmp);
        if (cols == 0) {
            cols = tmp.size();
        }
        for (const auto& [i,c]: std::ranges::views::enumerate(tmp)) {
            if (c != '.') {
                out[c].emplace_back(i, row);
            }

        }
        row++;
    }
    return std::make_tuple(out, row-1, cols);
}

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    auto [inp, nrows, ncols] = getInput(argv[1]);

    {
        std::unordered_set<Pos, pos_hash> node_locations;
        for (const auto& eles: std::views::values(inp)) {

            for (size_t i = 0; i< eles.size(); i++) {
                for (size_t j = i+1; j< eles.size(); j++) {
                    auto d = eles[i].delta(eles[j]);
                    auto p1 = eles[i].add(d);
                    auto p2 = eles[j].sub(d);
                    if (p1.in_bounds(nrows, ncols)) {
                        node_locations.emplace(p1);
                    }
                    if (p2.in_bounds(nrows, ncols)) {
                        node_locations.emplace(p2);
                    }
                }
            }
        }
        std::cout << "Part 1 solution: " << node_locations.size() << std::endl;
    }
    {
        std::unordered_set<Pos, pos_hash> node_locations;
        for (const auto& eles: std::views::values(inp)) {

            for (size_t i = 0; i< eles.size(); i++) {
                for (size_t j = i+1; j< eles.size(); j++) {
                    node_locations.emplace(eles[i]);
                    node_locations.emplace(eles[j]);
                    auto d = eles[i].delta(eles[j]);
                    auto p1 = eles[i].add(d);
                    while (p1.in_bounds(nrows, ncols)) {
                        node_locations.emplace(p1);
                        p1 = p1.add(d);
                    }

                    auto p2 = eles[j].sub(d);
                    while (p2.in_bounds(nrows, ncols)) {
                        node_locations.emplace(p2);
                        p2 = p2.sub(d);
                    }
                }
            }
        }
        std::cout << "Part 2 solution: " << node_locations.size() << std::endl;
    }
}