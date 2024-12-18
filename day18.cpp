#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "easyTimer.h"
#include <unordered_set>
#include <unordered_map>
#include <cinttypes>

using dt = intmax_t;
const auto& enumerate = std::ranges::views::enumerate;

constexpr dt INF = std::numeric_limits<dt>::max()/2;
// constexpr dt N = 6;
// constexpr dt N_BYTES = 12;
constexpr dt N = 70;
constexpr dt N_BYTES = 1024;

auto split(const std::string& inp, const char delim) {
    std::string tmp;
    std::vector<std::string> r;
    std::istringstream s(inp);
    while (std::getline(s, tmp, delim)) {
        r.push_back(tmp);
    }
    return r;
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

    template <class T1, class T2, class T3>
    size_t operator()(const std::tuple<T1, T2, T3>& p) const
    {
        return hash_pair()(std::make_pair(hash_pair()(std::make_pair(std::get<0>(p), std::get<1>(p))), std::get<2>(p)));
    }
};

using Locs = std::array<std::array<bool, N+1>, N+1>;

Locs getInput(const std::string &fp, const size_t n_bytes=N_BYTES){
    std::ifstream file(fp);
    Locs tmp_m;
    for(dt i = 0; i<= N; i++){
        for(dt j = 0; j<= N; j++){
            tmp_m[i][j] = false;
        }
    }
    size_t c = 0;
    while (file)
    {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            auto l = split(tmp,',');
            tmp_m[std::strtoimax(l[1].c_str(), nullptr, 10)][std::strtoimax(l[0].c_str(), nullptr, 10)] = true;
            c++;
        }
        if(c >= n_bytes){
            break;
        }
    }
    
    return tmp_m;
}

void searchHelper(const Locs &walls, const std::pair<dt, dt> pos, std::unordered_set<std::pair<dt, dt>, hash_pair>& best_visited, std::array<std::array<dt, N+1>, N+1>& min_score_at_place, dt& min_score, dt curScore) {
    
    if ((curScore + (N-pos.first) + (N-pos.second)) > min_score) {
        // std::cout << "here-1" << std::endl;
        return;
    }
    if(pos.first > N || pos.first < 0 || pos.second > N || pos.second < 0){
        // std::cout << "here-2" << ':' << pos.first << ',' << pos.second << std::endl;
        return;
    }
    

    if (pos == std::make_pair(N,N)) {
        // std::cout << "here0" << std::endl;
        if (curScore == min_score) {
            // best_visited.insert(visited.begin(), visited.end());
        }
        else if (curScore < min_score) {
            // std::cout <<  "cur best: " <<curScore << std::endl;
            // best_visited.clear();
            // best_visited.insert(visited.begin(), visited.end());
            std::cout << curScore << std::endl;
            min_score = curScore;
        }
        return;
    }

    if (curScore >= min_score_at_place[pos.first][pos.second]) {
        // std::cout << "hit" << min_score_at_place.size() << std::endl;
        return;
    }
    
    min_score_at_place[pos.first][pos.second] = curScore;
    std::cout << curScore << ',' << pos.first << ',' << pos.second << std::endl;
    // visited.emplace(pos);
    // std::cout << "here1" << std::endl;
    // if (min_score_at_place.size()%128 == 0) {
    //     std::cout << "visited size: " << min_score_at_place.size() << std::endl;
    // }
    // std::cout << visited.size() << ':' << pos.first << ',' << pos.second << std::endl;
    std::pair<dt, dt> tmp_pos;
    // std::cout << "here1" << std::endl;
    
    // std::cout << "here3" << std::endl;
    tmp_pos = std::make_pair(pos.first, pos.second+1);
    if (! (walls[tmp_pos.first][tmp_pos.second]) ) {
        searchHelper(walls, tmp_pos, best_visited, min_score_at_place, min_score, curScore+1);
    }
    // std::cout << "here4" << std::endl;
    tmp_pos = std::make_pair(pos.first+1, pos.second);
    if (! (walls[tmp_pos.first][tmp_pos.second] )) {
        searchHelper(walls, tmp_pos, best_visited, min_score_at_place, min_score, curScore+1);
    }
    tmp_pos = std::make_pair(pos.first-1, pos.second);
    if (! (walls[tmp_pos.first][tmp_pos.second] )) {
        // std::cout << "here2" << std::endl;
        searchHelper(walls, tmp_pos, best_visited, min_score_at_place, min_score, curScore+1);
    }
    // std::cout << "here2" << std::endl;
    tmp_pos = std::make_pair(pos.first, pos.second-1);
    if (! (walls[tmp_pos.first][tmp_pos.second] )) {
        searchHelper(walls, tmp_pos, best_visited, min_score_at_place, min_score, curScore+1);
    }
    // std::cout << "here-end" << std::endl;

}

[[nodiscard]] std::tuple<dt, dt> search(const Locs &walls, const std::pair<dt, dt> init_pos) {
    std::array<std::array<dt, N+1>, N+1> min_score_at_place = {INF};
    for(dt i = 0; i<= N; i++){
        for(dt j = 0; j<= N; j++){
            min_score_at_place[i][j] = INF;
        }
    }

    std::unordered_set<std::pair<dt, dt>, hash_pair> visited;
    std::unordered_set<std::pair<dt, dt>, hash_pair> best_visited;
    dt min_score = INF;

    if (!walls[init_pos.first][init_pos.second+1]) {
        searchHelper(walls, {init_pos.first, init_pos.second+1}, best_visited, min_score_at_place, min_score, 1);
    }
    if (!walls[init_pos.first+1][init_pos.second]) {
        searchHelper(walls, {init_pos.first+1, init_pos.second}, best_visited, min_score_at_place, min_score, 1);
    }

    return {min_score, best_visited.size()+2};
}

template <typename T,typename U>                                                   
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first+r.first,l.second+r.second};                                    
}   

bool searchHelper2(const Locs &walls, const std::pair<dt, dt> pos, std::array<std::array<dt, N+1>, N+1>& min_score_at_place, const dt x_target, const dt y_target, dt curScore){
    
    if(pos.first > N || pos.first < 0 || pos.second > N || pos.second < 0){
        return false;
    }
    
    if(!walls[pos.first][pos.second]){
        return false;
    }
    if(pos.first == x_target || pos.second == y_target){
        return true;
    }

    if (curScore >= min_score_at_place[pos.first][pos.second]) {
        return false;
    }
    
    min_score_at_place[pos.first][pos.second] = curScore;
    for(const auto&v: std::initializer_list<std::pair<dt, dt>>{{0, 1}, {1,0}, {-1, 0}, {0, -1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}}){
        auto tmp = searchHelper2(walls, pos+v, min_score_at_place, x_target, y_target, curScore+1);
        if(tmp){
            return true;
        }
    }

    return false;
}

[[nodiscard]] bool search2(const Locs &walls) {
    std::array<std::array<dt, N+1>, N+1> min_score_at_place = {INF};
    for(dt i = 0; i<= N; i++){
        for(dt j = 0; j<= N; j++){
            min_score_at_place[i][j] = INF;
        }
    }

    for(dt i = 0; i<= N; i++){
        auto found = searchHelper2(walls, {i, 0}, min_score_at_place, 0, N, 1);
        
        if(found){
            return true;
        }
    }

    for(dt i = 0; i<= N; i++){
        auto found = searchHelper2(walls, {i, N}, min_score_at_place, N, 0, 1);
        if(found){
            return true;
        }
    }

    return false;
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> t("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    
    {
        const auto inp = getInput(argv[1]);
        for(dt i = 0; i<= N; i++){
            for(dt j = 0; j<= N; j++){
                if(inp[i][j]){
                    std::cout << '#';
                }else{
                    std::cout << '.';
                }
            }
            std::cout << std::endl;
        }
        // for(const auto& [x,y]: inp){
        //     std::cout << x << ',' << y << std::endl;
        // }
        std::pair<dt, dt> init_pos = {0,0};
        auto [res1, res2] = search(inp, init_pos);
        std::cout << res1 << ',' << res2 << std::endl;
    }

    {
        int i = 1;
        bool res = false;
        Locs inp;
        while(!res){
            inp = getInput(argv[1], i);
            std::stringstream tmp;
            tmp << "\033[2J";
            for(dt i = 0; i<= N; i++){
            for(dt j = 0; j<= N; j++){
                if(inp[i][j]){
                    tmp << '#';
                }else{
                    tmp << '.';
                }
            }
            tmp << std::endl;
        }
            std::cout << tmp.str();
            res = search2(inp);
            i++;
        }
        std::cout << i-1 << std::endl;
    }
    return 0;
}