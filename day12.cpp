//
// Created by vscode on 12/12/24.
//


#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <cmath>
#include "easyTimer.h"
#include <unordered_map>
#include <unordered_set>
#include <ranges>

using u64 = uint64_t;
using Grid = std::vector<std::vector<char>>;

constexpr  char SENTINAL = '.';
Grid getInput(const std::string &fp)
{
    std::ifstream file(fp);
    Grid out;
    while (file)
    {

        std::string tmp;
        std::getline(file, tmp);
        // out.emplace_back();
        if (!tmp.empty())
        {
            out.emplace_back(tmp.begin(), tmp.end());
        }
    }
    return out;
}

struct hash_pair {
    // copied from https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c/
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        // Hash the first element
        size_t hash1 = std::hash<T1>{}(p.first);
        // Hash the second element
        size_t hash2 = std::hash<T2>{}(p.second);
        // Combine the two hash values
        return hash1
               ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
                  + (hash1 >> 2));
    }
};

u64 floodRegion(Grid& g, std::unordered_set<std::pair<size_t, size_t>, hash_pair>& matching, size_t row, size_t col, const char c_to_match) {
    auto c = g[row][col];
    if (c != c_to_match) {
        return 1;
    }
    if (matching.contains(std::make_pair(row, col))) {
        return 0;
    }
    matching.emplace(row, col);

    u64 count = 0;

    if (row==0) {
        count++;
    }else {
        count += floodRegion(g, matching, row-1, col, c_to_match);
    }
    if ((row+1) >= g.size()) {
        count++;
    }else {
        count += floodRegion(g, matching, row+1, col, c_to_match);
    }
    if (col == 0) {
        count++;
    }else {
        count += floodRegion(g, matching, row, col-1, c_to_match);
    }
    if ((col+1) >= g[0].size()) {
        count++;
    }else {
        count += floodRegion(g, matching, row, col+1, c_to_match);
    }

    return count;
}

std::tuple<u64, std::unordered_set<std::pair<size_t, size_t>, hash_pair>> getCost4Region(Grid& g, size_t row, size_t col) {

    std::unordered_set<std::pair<size_t, size_t>, hash_pair> matching;
    u64 edge_count = floodRegion(g, matching, row, col, g[row][col]);

    return  {edge_count * matching.size(), matching};
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> t("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    auto inp = getInput(argv[1]);

    for (const auto &row : inp) {
        for (const auto &c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
    u64 count = 0;
    std::unordered_set<std::pair<size_t, size_t>, hash_pair> found;

    for (size_t row = 0; row < inp.size(); row++) {
        for (size_t col = 0; col < inp[0].size(); col++) {
            if (!found.contains({row, col})) {
                auto [c, matching] = getCost4Region(inp, 0,0);
                count += c;
                found.insert(matching.begin(), matching.end());
            }
        }
    }
    // auto [c, matching] = getCost4Region(inp, 0,0);
    std::cout << count << std::endl;
}