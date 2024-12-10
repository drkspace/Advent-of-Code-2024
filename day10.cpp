#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include "easyTimer.h"
#include <unordered_set>

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

using set_p = std::unordered_set<std::pair<int, int>, hash_pair>;

std::vector<std::vector<int>> getInput(const std::string& fp){
    std::ifstream file (fp);
    std::vector<std::vector<int>> out;
    int i = 0;
    while(file){
        
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty()) {
            out.emplace_back();
            for(const auto& c: tmp){
              out[i].emplace_back(std::stoi(std::string(1,c)));  
            }
        }
        i++;
    }
    return out;
}

void concat(set_p& s1, const set_p s2){
    s1.insert(s2.begin(), s2.end());
}

set_p traverse_helper1(const std::vector<std::vector<int>>& map, const int row, const int col, const int prev_value){
    
    if(col < 0 || col >= static_cast<int>(map[0].size()) || row < 0 || row >= static_cast<int>(map.size())){
        return {};
    }
    if(map[row][col] != (prev_value+1)){
        return {};
    }

    if(map[row][col] == 9){
        return {{row, col}};
    }
    
    auto s = traverse_helper1(map, row+1, col, prev_value+1);
    concat(s, traverse_helper1(map, row-1, col, prev_value+1));
    concat(s, traverse_helper1(map, row, col-1, prev_value+1));
    concat(s, traverse_helper1(map, row, col+1, prev_value+1));

    return s;
}


int traverse1(const std::vector<std::vector<int>>& map, const int row, const int col){
    auto s = traverse_helper1(map, row+1, col, 0);
    concat(s, traverse_helper1(map, row-1, col, 0));
    concat(s, traverse_helper1(map, row, col+1, 0));
    concat(s, traverse_helper1(map, row, col-1, 0));
    
    return s.size();
}

int traverse_helper2(const std::vector<std::vector<int>>& map, const int row, const int col, const int prev_value){
    
    if(col < 0 || col >= static_cast<int>(map[0].size()) || row < 0 || row >= static_cast<int>(map.size())){
        return 0;
    }
    if(map[row][col] != (prev_value+1)){
        return 0;
    }
    if(map[row][col] == 9){
        return 1;
    }
    

    return traverse_helper2(map, row+1, col, prev_value+1)+traverse_helper2(map, row-1, col, prev_value+1)+traverse_helper2(map, row, col-1, prev_value+1)+traverse_helper2(map, row, col+1, prev_value+1);
}


int traverse2(const std::vector<std::vector<int>>& map, const int row, const int col){
    
    return traverse_helper2(map, row+1, col, 0)+traverse_helper2(map, row-1, col, 0)+traverse_helper2(map, row, col+1, 0)+traverse_helper2(map, row, col-1, 0);
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> t("main ");

    if (argc != 2) {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const auto inp = getInput(argv[1]);

    std::vector<std::pair<int, int>> trailheads;

    for(const auto& [i, row]: std::ranges::views::enumerate(inp)){
        for(const auto& [j, ele]: std::ranges::views::enumerate(row)){
            if(ele == 0){
                trailheads.emplace_back(i, j);
            }
            std::cout << ele;
        }
        std::cout << std::endl;
    }

    int count1 = 0;
    int count2 = 0;
    for(const auto& [row, col]: trailheads){
        count1 += traverse1(inp, row, col);
        count2 += traverse2(inp, row, col); 
    }
    std::cout << count1 << std::endl;
    std::cout << count2 << std::endl;
}