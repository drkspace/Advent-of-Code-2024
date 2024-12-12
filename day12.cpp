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

struct comp_pair {
    // copied from https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c/
    template <class T1, class T2>
    bool operator()(const std::pair<T1, T2>& p1, const std::pair<T1, T2>& p2) const
    {
        if(p1.first == p2.first){
            return p2.second > p1.second;
        }else{
            return p2.first > p1.first;
        }
    }
};

using edge_set = std::set<std::pair<size_t, size_t>, comp_pair>;

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
    std::cout << g[row][col] << ":" << edge_count << "*" << matching.size() << std::endl;
    return  {edge_count * matching.size(), matching};
}

enum class Dir{
    UP, DOWN, LEFT, RIGHT
};

u64 floodRegion2(Grid& g, std::unordered_set<std::pair<size_t, size_t>, hash_pair>& matching, edge_set& row_edges, edge_set& col_edges, size_t row, size_t col, const char c_to_match, const Dir d = Dir::UP) {
    auto c = g[row][col];
    if (c != c_to_match) {
        switch (d)
        {
        case Dir::UP:
            row_edges.emplace(std::make_pair(row+1, col));
            break;
        case Dir::DOWN:
            row_edges.emplace(std::make_pair(row, col));
            break;
        case Dir::LEFT:
            col_edges.emplace(std::make_pair(col+1, row));
            break;
        case Dir::RIGHT:
        // std::cout << "(" << row << ',' << col << ")," << std::endl;
            col_edges.emplace(std::make_pair(col, row));
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
    // std::cout << "(" << row << ',' << col << ")" << std::endl;
    if (row==0) {
        // std::cout << "r=0" << std::endl;
        count++;
        row_edges.emplace(std::make_pair(row, col));
    }else {
        count += floodRegion2(g, matching, row_edges, col_edges, row-1, col, c_to_match, Dir::UP);
    }
    if ((row+1) >= g.size()) {
        // std::cout << "r>=s" << std::endl;
        count++;
        row_edges.emplace(std::make_pair(row+1, col));
    }else {
        count += floodRegion2(g, matching, row_edges, col_edges, row+1, col, c_to_match, Dir::DOWN);
    }
    if (col == 0) {
        // std::cout << "c=0" << "(" << row << ',' << col << ")" << std::endl;
        count++;
        col_edges.emplace(std::make_pair(col, row));
    }else {
        count += floodRegion2(g, matching, row_edges, col_edges, row, col-1, c_to_match, Dir::LEFT);
    }
    if ((col+1) >= g[0].size()) {
        // std::cout << "c>=s" << std::endl;
        count++;
        col_edges.emplace(std::make_pair(col+1, row));
    }else {
        
        count += floodRegion2(g, matching, row_edges, col_edges, row, col+1, c_to_match, Dir::RIGHT);
    }

    return count;
}

std::tuple<u64, std::unordered_set<std::pair<size_t, size_t>, hash_pair>> getCost4Region2(Grid& g, size_t row, size_t col) {

    edge_set row_edges;
    edge_set col_edges;
    std::unordered_set<std::pair<size_t, size_t>, hash_pair> matching;
    std::cout << g[row][col] << std::endl;
    floodRegion2(g, matching, row_edges, col_edges, row, col, g[row][col]);
    // std::cout << g[row][col] << ":" << edge_count << "*" << matching.size() << "|" << row_edges.size() << "+" << col_edges.size() << std::endl;
    // std::cout << "R";
    // for(const auto&[row, col]: row_edges){
    //     std::cout << "(" << row << ',' << col << "),";
    // }
    // std::cout << std::endl;
    // std::cout << "C";
    // for(const auto&[row, col]: col_edges){
    //     std::cout << "(" << row << ',' << col << "),";
    // }
    // std::cout << std::endl;
    u64 row_edges_count = 0;
    u64 col_edges_count = 0;
    // std::cout << "R";
    for(auto it = row_edges.begin(); it != row_edges.end(); ++it){
        // std::cout << "(" << row << ',' << col << "),";
        auto s = *it;
        // const auto s_org = *it;
        while(std::next(it) != row_edges.end() && std::next(it)->first == s.first && (std::next(it)->second) == (s.second+1)){
            ++it;
            s.second = it->second;
        }
        // std::cout << "(" << s_org.first << ',' << s_org.second << ")->""(" << s.first << ',' << s.second << ")" << std::endl;
        ++row_edges_count;
    }
    // std::cout << std::endl;
    // std::cout << "C";
    for(auto it = col_edges.begin(); it != col_edges.end(); ++it){
        // std::cout << "(" << row << ',' << col << "),";
        auto s = *it;
        // const auto s_org = *it;
        // std::cout << std::next(it)->first << "|" << std::next(it)->second << std::endl;
        while(std::next(it) != col_edges.end() && std::next(it)->first == s.first && (std::next(it)->second) == (s.second+1)){
            ++it;
            s.second = it->second;
            // std::cout << std::next(it)->first << "|" << std::next(it)->second << std::endl;
        }
        // std::cout << "(" << s_org.second << ',' << s_org.first << ")->""(" << s.second << ',' << s.first << ")" << std::endl;
        ++col_edges_count;
    }
    // std::cout << std::endl;

    // for(const auto&[row, col]: row_edges_comb){
    //     std::cout << "(" << row << ',' << col << "),";
    // }
    // std::cout << std::endl;
    // for(const auto&[col, row]: col_edges_comb){
    //     std::cout << "(" << row << ',' << col << "),";
    // }
    // std::cout << std::endl;

    std::cout << col_edges_count << '+' << row_edges_count << '*' <<matching.size() << std::endl;
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
    // {
    //     auto inp_cpy = inp;
    //     u64 count = 0;
    //     std::unordered_set<std::pair<size_t, size_t>, hash_pair> found;

    //     for (size_t row = 0; row < inp_cpy.size(); row++) {
    //         for (size_t col = 0; col < inp_cpy[0].size(); col++) {
    //             if (!found.contains({row, col})) {
    //                 auto [c, matching] = getCost4Region(inp_cpy, row,col);
    //                 count += c;
    //                 found.insert(matching.begin(), matching.end());
    //             }
    //         }
    //     }
    //     // auto [c, matching] = getCost4Region(inp, 0,0);
    //     std::cout << count << std::endl;
    // }

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
        // auto [c, matching] = getCost4Region(inp, 0,0);
        std::cout << count << std::endl;
    }
}