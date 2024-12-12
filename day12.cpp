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
#include "easyTimer.h"
#include <unordered_set>

using u64 = uint64_t;
using Grid = std::vector<std::vector<char>>;

enum class Dir{
    UP, DOWN, LEFT, RIGHT
};

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

struct comp_pair {
    // copied from https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c/
    template <class T1, class T2>
    bool operator()(const std::tuple<T1, T2, Dir>& p1, const std::tuple<T1, T2, Dir>& p2) const
    {
        if(std::get<0>(p1) == std::get<0>(p2)){
            return std::get<1>(p2) > std::get<1>(p1);
        }else{
            return std::get<0>(p2) > std::get<0>(p1);
        }
    }
};

struct edge {
    u64 row;
    u64 col;
    Dir dir;

    bool operator<(const edge& other) const {
        if(this->row == other.row){
            return other.col > this->col;
        }else{
            return other.row > this->row;
        }
    }
};

using edge_set = std::set<edge>;

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



u64 floodRegion2(Grid& g, std::unordered_set<std::pair<size_t, size_t>, hash_pair>& matching, edge_set& row_edges, edge_set& col_edges, size_t row, size_t col, const char c_to_match, const Dir d = Dir::UP) {
    auto c = g[row][col];
    if (c != c_to_match) {
        switch (d)
        {
            case Dir::UP:
            row_edges.emplace(row+1, col, Dir::UP);
            break;
            case Dir::DOWN:
            row_edges.emplace(row, col, Dir::DOWN);
            break;
            case Dir::LEFT:
            col_edges.emplace(col+1, row, Dir::LEFT);
            break;
        case Dir::RIGHT:
            col_edges.emplace(col, row, Dir::RIGHT);
            break;
        default:
            break;
        }
        return 1;
    }
    if (matching.contains(std::make_pair(row, col))) {
        return 0;
    }
    matching.emplace(row, col);

    u64 count = 0;
    if (row==0) {
        count++;
        row_edges.emplace(row, col, Dir::UP);
    }else {
        count += floodRegion2(g, matching, row_edges, col_edges, row-1, col, c_to_match, Dir::UP);
    }
    if ((row+1) >= g.size()) {
        count++;
        row_edges.emplace(row+1, col, Dir::DOWN);
    }else {
        count += floodRegion2(g, matching, row_edges, col_edges, row+1, col, c_to_match, Dir::DOWN);
    }
    if (col == 0) {
        count++;
        col_edges.emplace(col, row, Dir::LEFT);
    }else {
        count += floodRegion2(g, matching, row_edges, col_edges, row, col-1, c_to_match, Dir::LEFT);
    }
    if ((col+1) >= g[0].size()) {
        count++;
        col_edges.emplace(col+1, row, Dir::RIGHT);
    }else {
        
        count += floodRegion2(g, matching, row_edges, col_edges, row, col+1, c_to_match, Dir::RIGHT);
    }

    return count;
}

std::tuple<u64, std::unordered_set<std::pair<size_t, size_t>, hash_pair>> getCost4Region2(Grid& g, size_t row, size_t col) {

    edge_set row_edges;
    edge_set col_edges;
    std::unordered_set<std::pair<size_t, size_t>, hash_pair> matching;
    floodRegion2(g, matching, row_edges, col_edges, row, col, g[row][col]);

    u64 row_edges_count = 0;
    u64 col_edges_count = 0;

    for(auto it = row_edges.begin(); it != row_edges.end(); ++it){
        auto s = *it;
        while(std::next(it) != row_edges.end() && std::next(it)->row == s.row && (std::next(it)->col == s.col+1 && s.dir==std::next(it)->dir)){
            ++it;
            s.col = it->col;
        }
        ++row_edges_count;
    }

    for(auto it = col_edges.begin(); it != col_edges.end(); ++it){
        auto s = *it;

        while(std::next(it) != col_edges.end() && std::next(it)->row == s.row && (std::next(it)->col == s.col+1 && s.dir==std::next(it)->dir)){
            ++it;
            s.col = it->col;
        }
        ++col_edges_count;
    }

    return  {(col_edges_count + row_edges_count) * matching.size(), matching};
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> t("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const auto inp = getInput(argv[1]);

    for (const auto &row : inp) {
        for (const auto &c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
    {
        auto inp_cpy = inp;
        u64 count = 0;
        std::unordered_set<std::pair<size_t, size_t>, hash_pair> found;

        for (size_t row = 0; row < inp_cpy.size(); row++) {
            for (size_t col = 0; col < inp_cpy[0].size(); col++) {
                if (!found.contains({row, col})) {
                    auto [c, matching] = getCost4Region(inp_cpy, row,col);
                    count += c;
                    found.insert(matching.begin(), matching.end());
                }
            }
        }

        std::cout << "Part 1 solution: " << count << std::endl; // 1450422
    }

    {
        auto inp_cpy = inp;
        u64 count = 0;
        std::unordered_set<std::pair<size_t, size_t>, hash_pair> found;

        for (size_t row = 0; row < inp_cpy.size(); row++) {
            for (size_t col = 0; col < inp_cpy[0].size(); col++) {
                if (!found.contains({row, col})) {
                    auto [c, matching] = getCost4Region2(inp_cpy, row,col);
                    count += c;
                    found.insert(matching.begin(), matching.end());
                }
            }
        }
        std::cout << "Part 2 solution: " << count << std::endl; // 906606
    }
}