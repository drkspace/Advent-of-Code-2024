#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <sstream>
#include <limits>
#include <ranges>

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

constexpr char BOX = '#';
constexpr char VALID = '.';
constexpr char GUARD = '^';


class GuardState{
    
    public:
        GuardState(int x, int y): visited({}), visited_dir({}), cur_pos(std::make_pair(x,y)), inital(cur_pos){
            visit(x, y);
            visit_dir(std::make_pair(x, y), GuardState::Facing::NORTH);
        };
        GuardState(const pos& p): visited({}), visited_dir({}), cur_pos(p), inital(p){
            visit(p);
            visit_dir(p, GuardState::Facing::NORTH);
        };
        
        void move(const Map& map){
            pos tmp;
            switch (facing)
            {
            case Facing::NORTH:
                tmp = std::make_pair(cur_pos.first-1, cur_pos.second);
                if(tmp.first >= 0 && map[tmp.first][tmp.second] == BOX){
                    facing = Facing::EAST;
                }else{
                    cur_pos = tmp;
                }
                break;
            case Facing::EAST:
                tmp = std::make_pair(cur_pos.first, cur_pos.second+1);
                if(tmp.second < static_cast<int>(map[0].size()) && map[tmp.first][tmp.second] == BOX){
                    facing = Facing::SOUTH;
                }else{
                    cur_pos = tmp;
                }
                break;
            case Facing::SOUTH:
                tmp = std::make_pair(cur_pos.first+1, cur_pos.second);
                if(tmp.first < static_cast<int>(map.size()) && map[tmp.first][tmp.second] == BOX){
                    facing = Facing::WEST;
                }else{
                    cur_pos = tmp;
                }
                break;
            case Facing::WEST:
                tmp = std::make_pair(cur_pos.first, cur_pos.second-1);
                if(tmp.second >= 0 && map[tmp.first][tmp.second] == BOX){
                    facing = Facing::NORTH;
                }else{
                    cur_pos = tmp;
                }
                break;
            default:
                break;
            }
            if(in_map(map)){
                visit(cur_pos);
                // std::cout << cur_pos.first << ',' <<cur_pos.second << std::endl;
                visit_dir(cur_pos, facing);
            }else{
            }
        }

        int loops(const Map& map){

            tracker locations; 

            for(const auto& [idx, tmp]: std::ranges::views::enumerate(visited_dir)){
                const auto& [x, y, dir] = tmp;
                // continue;
                auto nf = next(dir);
                switch (nf)
                {
                case Facing::NORTH:
                    for(int i = x; i>=0; i--){
                        if(std::find(visited_dir.begin(), visited_dir.begin()+idx, std::make_tuple(i, y, nf)) != (visited_dir.begin()+idx)){
                            // std::cout << x << ',' << y-1 << "WEST" << std::endl;
                            if(x == 3 && y-1 == 4){
                                std::cout << i << ',' << y << " W" << std::endl;
                            }
                            locations.emplace(x, y-1);
                            break;
                        }
                    }
                    break;
                case Facing::SOUTH:
                    for(size_t i = x; i< map.size(); i++){
                        if(std::find(visited_dir.begin(), visited_dir.begin()+idx, std::make_tuple(i, y, nf)) != (visited_dir.begin()+idx)){
                            // std::cout << x << ',' << y+1 << "EAST"<<  std::endl;
                            if(x == 3 && y+1 == 4){
                                std::cout << i << ',' << y << " E" << std::endl;
                            }
                            locations.emplace(x, y+1);
                            break;
                        }
                    }
                    break;
                case Facing::EAST:
                    for(size_t i = y; i<map[0].size(); i++){
                        if(std::find(visited_dir.begin(), visited_dir.begin()+idx, std::make_tuple(x, i, nf)) != (visited_dir.begin()+idx)){
                            // std::cout << x-1 << ',' << y << "NORTH"<< std::endl;
                            if(x-1 == 3 && y == 4){
                                std::cout << i << ',' << y << " N" << std::endl;
                            }
                            locations.emplace(x-1, y);
                            break;
                        }
                    }
                    break;
                case Facing::WEST:
                    for(int i = y; i>=0; i--){
                        if(std::find(visited_dir.begin(), visited_dir.begin()+idx, std::make_tuple(x, i, nf)) != (visited_dir.begin()+idx)){
                            //  std::cout << x+1 << ',' << y << "SOUTH"<< std::endl;
                            if(x+1 == 3 && y == 4){
                                std::cout << i << ',' << y << " W" << std::endl;
                            }
                            locations.emplace(x+1, y);
                            break;
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            
            for(const auto& [x, y]: locations){
                std::cout << x << ',' << y << std::endl;
            }
            return locations.size();
        }

        [[nodiscard]] bool in_map(const Map& map){
            // std::cout << "im" << std::endl;
            return cur_pos.first >= 0 && cur_pos.second >= 0 && cur_pos.second < static_cast<int>(map[0].size()) && cur_pos.first < static_cast<int>(map.size());
        }

        [[nodiscard]] int count(){
            return visited.size();
        }

        int loop_count = 0;

    private:
        enum Facing{
            NORTH,
            EAST,
            SOUTH,
            WEST
        };

        Facing facing = Facing::NORTH;
        tracker visited;
        std::vector<std::tuple<int, int, GuardState::Facing>> visited_dir; 

        pos cur_pos;
        const pos inital;

        void visit(int x, int y){
            visited.emplace(x, y);
        }

        void visit(pos p){
            visited.emplace(p);
        }

        void visit_dir(pos p, GuardState::Facing facing){
            visited_dir.emplace_back(std::make_tuple(p.first, p.second, facing));
        }

        Facing next(Facing f){
            switch (f)
            {
            case Facing::NORTH:
                return Facing::EAST;
                break;
            case Facing::EAST:
                return Facing::SOUTH;
                break;
            case Facing::SOUTH:
                return Facing::WEST;
                break;
            case Facing::WEST:
                return Facing::NORTH;
                break;
            }
            return Facing::NORTH;
        }
};

Map getInput(const std::string& fp){
    std::ifstream file (fp);
    Map out;
    while(file){
     std::string tmp;
     std::getline(file, tmp);
        if (!tmp.empty()) {
            out.push_back(std::vector<char>(tmp.begin(), tmp.end()));
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

    pos g_pos;
    for(const auto& [i, row]: std::ranges::views::enumerate(inp)){
        for(const auto& [j, c]: std::ranges::views::enumerate(row)){
            std::cout << c;
            
            if(c == GUARD){
                g_pos = std::make_pair(i, j);
            }
        }
        std::cout << std::endl;
    }
    // std::cout << g_pos.first <<',' << g_pos.second << std::endl;
    GuardState s(g_pos);
    while(s.in_map(inp)){
        // std::cout << "moving" << std::endl;
        s.move(inp);
        // std::cout << "done moving" << std::endl;
    }
    std::cout << s.count() << std::endl;

    //6,3
    //7,6
    //7,7
    //8,1
    //8,3
    //9,7

    //gt 867
    //gt 866
    auto c = s.loops(inp);
    std::cout << c << std::endl;
    return 0;
}