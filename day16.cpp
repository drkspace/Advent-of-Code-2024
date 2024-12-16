#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include "easyTimer.h"
#include <unordered_set>
#include <unordered_map>

using u64 = uint64_t;

constexpr size_t N = 7;

using Matrix = std::array<std::array<u64, N>, N>;

constexpr u64 INF = std::numeric_limits<u64>::max()/2;

enum class Dir {
    UP, DOWN, LEFT, RIGHT
};

Matrix input(const std::string &fp){
    std::ifstream file(fp);
    std::vector<std::vector<char>> tmp_m;
    Matrix out;
    while (file)
    {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            tmp_m.emplace_back(tmp);
        }
    }
    
    for(size_t i = 0; i< N; ++i){
        for(size_t j = 0; j< N; ++j){
            out[i][j] = INF;
        }
    }

    u64 node_idx = 1;
    std::unordered_map<std::tuple<u64, u64, Dir>, u64> idx_to_node;
    for(size_t i = 0; i<tmp_m.size(); i++){
        for(size_t j = 0; j<tmp_m.size(); j++){
            if(tmp_m[i][j] == '#'){
                continue;
            }
            if(tmp_m[i][j] == 'S'){
                idx_to_node[std::make_tuple(i,j, Dir::LEFT)] = 0;
            }else if(tmp_m[i][j] == 'E'){
                idx_to_node[std::make_tuple(i,j, Dir::UP)] = N;
            } else{
                if(tmp_m[i+1][j] == '.' || tmp_m[i-1][j] == '.'){
                    idx_to_node[std::make_tuple(i,j)] = node_idx;
                    node_idx++;
                }
            }
        }
    }

    for(const auto&[idx, node]: idx_to_node){
        const auto&[i,j,d] = idx;


    }

}

Matrix adj_matrix(){
           // 1 2 3 4 5 6 7 
    return  {{{INF,4,6,8,INF,INF,INF},
             {INF,INF,1,INF,7,INF,INF},
             {INF,INF,INF,2,5,4,INF},
             {INF,INF,INF,INF,INF,5,INF},
             {INF,INF,INF,INF,INF,INF,6},
             {INF,INF,INF,INF,1,INF,8},
             {INF,INF,INF,INF,INF,INF,INF}}};
}

void disp(const std::unordered_set<u64>& P, const std::unordered_set<u64>& T, const std::array<u64, N>& L, const std::array<u64, N>& L_prime){
    std::cout << "P: ";
    for(const auto&ele: P){
        std::cout << ele << ',';
    }
    std::cout << std::endl;
    std::cout << "T: ";
    for(const auto&ele: T){
        std::cout << ele << ',';
    }
    std::cout << std::endl;
    std::cout << "L: ";
    for(const auto&i: P){
        std::cout << i << '=' << L[i-1] << ',';
    }
    std::cout << std::endl;
    std::cout << "L': ";
    for(const auto&i: T){
        if(L_prime[i-1] == INF){
            std::cout << i << "=-,";
        }else{
            std::cout << i << '=' << L_prime[i-1] << ',';
        }
    }
    std::cout << std::endl;
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> t("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    auto inp = getInput(argv[1]);

    const auto adj = adj_matrix();

    std::unordered_set<u64> P = {{1}};
    std::unordered_set<u64> T;
    std::array<u64, N> L({0});
    std::array<u64, N> L_prime = adj[0];
    L[1-1] = 0;

    {
        auto tmp = std::views::iota(static_cast<u64>(1), N+1);
        std::set_difference(tmp.begin(), tmp.end(), 
                            P.begin(), P.end(), 
                            std::inserter(T, T.begin()));
    }

    


    while (!T.empty())
    {
        std::cout << "===============" << std::endl;
        disp(P, T, L, L_prime);
        u64 k = INF;
        u64 min_v = INF;
        for(const auto& idx: T){
            if(L_prime[idx-1] < min_v){
                min_v = L_prime[idx-1];
                k = idx;
            }
        }
        if(k == INF){
            break;
        }
        
        P.emplace(k);
        auto k_itr = T.find(k);
        if(k_itr == T.end()){
            std::cout << "weewoo" << std::endl;
        }
        L[k-1] = L_prime[k-1];
        T.erase(k_itr);
        if(T.empty()){
            break;
        }
        
        for(const auto&j: T){
            L_prime[j-1] = std::min({L_prime[j-1], L[k-1]+adj[k-1][j-1]});
        }
    }
    std::cout << "===============" << std::endl;
    disp(P, T, L, L_prime);

    return 0;
}