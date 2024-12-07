#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <sstream>
#include <limits>
#include <ranges>
#include <unordered_map>

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

struct hash_triple {
    // copied from https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c/
    template <class T1, class T2, class T3>
    size_t operator()(const std::tuple<T1, T2, T3>& p) const
    {
        size_t hash1 = std::hash<T1>{}(std::get<0>(p));
        // Hash the second element
        size_t hash2 = std::hash<T2>{}(std::get<1>(p));
        size_t hash3 = std::hash<T3>{}(std::get<2>(p));
        // Combine the two hash values
        hash1 = hash1
               ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
                  + (hash1 >> 2));
        return hash1
               ^ (hash3 + 0x9e3779b9 + (hash1 << 6)
                  + (hash1 >> 2));
    }
};


using Map = std::vector<std::vector<char>>;
using pos = std::pair<int, int>;
using tracker = std::unordered_set<pos, hash_pair>;
using BoxLocations = std::unordered_set<pos, hash_pair>;

constexpr char BOX = '#';
constexpr char GUARD = '^';

[[nodiscard]] int sign(const int v) {
    if (v>0) {
        return 1;
    }else if (v < 0) {
        return -1;
    }
    return 0;
}

class Direction{
    public:
    enum D{
        NORTH,
        SOUTH,
        EAST,
        WEST
    };
    static D next(D cur){
        switch (cur)
        {
        case D::NORTH:
            return D::EAST;
            break;
        case D::EAST:
            return D::SOUTH;
            break;
        case D::SOUTH:
            return D::WEST;
            break;
        case D::WEST:
            return D::NORTH;
            break;
        }
        return D::NORTH;
    }
};

class GuardState{
    using BoxHash = std::unordered_map<int, std::vector<int>>;
    public:
        GuardState(pos g_pos, BoxLocations bl, int grid_size_x, int grid_size_y): init_pos(g_pos), box_locations(std::move(bl)), grid_size_x(grid_size_x), grid_size_y(grid_size_y), t({}) {
            for(const auto& [x, y]: box_locations){
                row_locations[x].push_back(y);
                col_locations[y].push_back(x);
            }
            for(auto& [key, vec]: row_locations){
                std::ranges::sort(vec);
            }
            for(auto& [key, vec]: col_locations){
                std::ranges::sort(vec);
            }
        }
        int run(){

            Direction::D d = Direction::D::NORTH;
            auto cur_pos = init_pos;
            int count = 1; // Include the guard's starting position
            while(true){

                switch (d)
                {
                case Direction::D::NORTH:
                    if(col_locations.contains(cur_pos.second)){
                        auto lb = std::ranges::lower_bound(col_locations[cur_pos.second], cur_pos.first);
                        auto next = std::distance(col_locations[cur_pos.second].begin(), lb) - 1;
                        if(next < 0){
                            visit_mult(cur_pos, std::make_pair(-1, cur_pos.second));
                            return t.size();
                        }
                        count += std::abs(cur_pos.first - col_locations[cur_pos.second].at(next))-1;
                        auto tmp_pos = std::make_pair(col_locations[cur_pos.second][next]+1, cur_pos.second);
                        visit_mult(cur_pos, tmp_pos);
                        cur_pos = tmp_pos;
                    }else{
                        return t.size() + cur_pos.first;
                    }
                    break;
                case Direction::D::EAST:
                    if(row_locations.contains(cur_pos.first)){
                        auto ub = std::ranges::upper_bound(row_locations[cur_pos.first], cur_pos.second);
                        auto next = std::distance(row_locations[cur_pos.first].begin(), ub);
                        if(next >= static_cast<int>(row_locations[cur_pos.first].size())){
                            visit_mult(cur_pos, std::make_pair(cur_pos.first, grid_size_y));
                            return t.size();
                        }
                        count += std::abs(cur_pos.second - row_locations[cur_pos.first].at(next))-1;
                        auto tmp_pos = std::make_pair(cur_pos.first, row_locations[cur_pos.first][next]-1);
                        visit_mult(cur_pos, tmp_pos);
                        cur_pos = tmp_pos;
                    }else{
                        return t.size() + (grid_size_y-cur_pos.second);
                    }
                    break;
                case Direction::D::SOUTH:
                    if(col_locations.contains(cur_pos.second)){
                        auto ub = std::ranges::upper_bound(col_locations[cur_pos.second], cur_pos.first);
                        auto next = std::distance(col_locations[cur_pos.second].begin(), ub);
                        if(next >= static_cast<int>(col_locations[cur_pos.second].size())){
                            visit_mult(cur_pos, std::make_pair(grid_size_x, cur_pos.second));
                            return t.size();
                        }
                        count += std::abs(cur_pos.first - col_locations[cur_pos.second].at(next))-1;
                        auto tmp_pos = std::make_pair(col_locations[cur_pos.second][next]-1, cur_pos.second);
                        visit_mult(cur_pos, tmp_pos);
                        cur_pos = tmp_pos;
                    }else{
                        return t.size() + (grid_size_x - cur_pos.first);
                    }
                    break;
                case Direction::D::WEST:
                    if(row_locations.contains(cur_pos.first)){
                        auto lb = std::ranges::lower_bound(row_locations[cur_pos.first], cur_pos.second);
                        auto next = std::distance(row_locations[cur_pos.first].begin(), lb) - 1;
                        if(next < 0){
                            visit_mult(cur_pos, std::make_pair(cur_pos.first, -1));
                            return t.size();
                        }
                        count += std::abs(cur_pos.second - row_locations[cur_pos.first].at(next))-1;
                        auto tmp_pos = std::make_pair(cur_pos.first, row_locations[cur_pos.first][next]+1);
                        visit_mult(cur_pos, tmp_pos);
                        cur_pos = tmp_pos;
                    }else{
                        return t.size() + cur_pos.second;
                    }
                    break;
                }

                d = Direction::next(d);
            }
            return count;
        }
    private:
    const pos init_pos;
    const BoxLocations box_locations;
    BoxHash row_locations;
    BoxHash col_locations;
    const int grid_size_x;
    const int grid_size_y;

    tracker t;

    void visit_mult(const pos& pos1, const pos& pos2) {
        if (pos1.first != pos2.first) {
            for (int i = pos1.first; i != pos2.first; i+= sign(pos2.first-pos1.first)) {
                t.emplace(i, pos1.second);
            }
        }
        else {
            for (int i = pos1.second; i != pos2.second; i+= sign(pos2.second-pos1.second)) {
                t.emplace(pos1.first, i);
            }
        }
    };

};

Map getInput(const std::string& fp){
    std::ifstream file (fp);
    Map out;
    while(file){
     std::string tmp;
     std::getline(file, tmp);
        if (!tmp.empty()) {
            out.emplace_back(tmp.begin(), tmp.end());
        }
      }
      return out;
}

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const auto inp = getInput(argv[1]);
    BoxLocations bl;
    pos g_pos;
    for(const auto& [i, row]: std::ranges::views::enumerate(inp)){
        for(const auto& [j, c]: std::ranges::views::enumerate(row)){
            std::cout << c;
            if(c == BOX){
                bl.emplace(i, j);
            }
            else if(c == GUARD){
                g_pos = std::make_pair(i, j);
            }
        }
        std::cout << std::endl;
    }
    // std::cout << g_pos.first <<',' << g_pos.second << std::endl;
    GuardState s(g_pos, bl, inp.size(), inp[0].size());
    auto c = s.run();
    std::cout << c << std::endl;

    // for(const auto& [i, row]: std::ranges::views::enumerate(inp)){
    //     for(const auto& [j, c]: std::ranges::views::enumerate(row)){
    //         if (c == GUARD || c == BOX) {
    //             std::cout << c;
    //         }
    //         else if (s.t.contains(std::make_pair(i, j))) {
    //             std::cout << "+";
    //         }
    //         else{
    //             std::cout << c;
    //         }
    //
    //     }
    //     std::cout << std::endl;
    // }

    //6,3
    //7,6
    //7,7
    //8,1
    //8,3
    //9,7

    //gt 867
    //gt 866
    // auto c = s.loops(inp);
    // std::cout << c << std::endl;
    return 0;
}