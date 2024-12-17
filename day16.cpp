#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "easyTimer.h"
#include <unordered_set>
#include <unordered_map>

using u64 = int64_t;
const auto& enumerate = std::ranges::views::enumerate;

constexpr u64 INF = std::numeric_limits<u64>::max()/2;

enum class Dir {
    UP, DOWN, LEFT, RIGHT
};

std::vector<std::vector<char>> getInput(const std::string &fp){
    std::ifstream file(fp);
    std::vector<std::vector<char>> tmp_m;
    while (file)
    {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            std::vector<char> tmp_v(tmp.begin(), tmp.end());
            tmp_m.emplace_back(tmp_v);
        }
    }
    
    return tmp_m;
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

void searchHelper(const std::vector<std::vector<char>> &input, Dir dir, const std::pair<u64, u64> pos, std::unordered_set<std::pair<u64, u64>, hash_pair> visited, std::unordered_set<std::pair<u64, u64>, hash_pair>& best_visited, std::unordered_map<std::tuple<u64, u64, Dir>, u64, hash_pair>& min_score_at_place, u64& min_score, u64 curScore) {
    if (curScore > min_score) {
        return;
    }
    if (input[pos.first][pos.second] == 'E') {
        if (curScore == min_score) {
            best_visited.insert(visited.begin(), visited.end());
        }
        else if (curScore < min_score) {
            std::cout <<  "cur best: " <<curScore << std::endl;
            best_visited.clear();
            best_visited.insert(visited.begin(), visited.end());
            min_score = curScore;
        }
    }
    Dir tmpd = Dir::UP;
    switch (dir) {
        case Dir::UP:
        case Dir::DOWN:
            tmpd = Dir::UP;
            break;
        case Dir::RIGHT:
        case Dir::LEFT:
            tmpd = Dir::RIGHT;
            break;
    }

    if (const auto s = min_score_at_place.find(std::make_tuple(pos.first, pos.second, tmpd)); s != min_score_at_place.end()) {
        if (s->second < curScore) {
            // std::cout << "hit" << std::endl;
            return;
        }
    }
    min_score_at_place[std::make_tuple(pos.first, pos.second, tmpd)] = curScore;
    visited.emplace(pos);
    if (min_score_at_place.size()%128 == 0) {
        std::cout << "visited size: " << min_score_at_place.size() << std::endl;
    }
    // std::cout << visited.size() << ':' << pos.first << ',' << pos.second << std::endl;
    std::pair<u64, u64> tmp_pos;
    switch (dir) {

        case Dir::UP:
            tmp_pos = std::make_pair(pos.first-1, pos.second);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::UP, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1);
            }
            tmp_pos = std::make_pair(pos.first, pos.second-1);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::LEFT, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1+1000);
            }
            tmp_pos = std::make_pair(pos.first, pos.second+1);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::RIGHT, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1+1000);
            }
            break;
        case Dir::DOWN:
            tmp_pos = std::make_pair(pos.first+1, pos.second);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::DOWN, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1);
            }
            tmp_pos = std::make_pair(pos.first, pos.second-1);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::LEFT, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1+1000);
            }
            tmp_pos = std::make_pair(pos.first, pos.second+1);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::RIGHT, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1+1000);
            }
            break;
        case Dir::LEFT:
            tmp_pos = std::make_pair(pos.first+1, pos.second);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::DOWN, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1+1000);
            }
            tmp_pos = std::make_pair(pos.first, pos.second-1);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::LEFT, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1);
            }
            tmp_pos = std::make_pair(pos.first-1, pos.second);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::UP, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1+1000);
            }

            break;
        case Dir::RIGHT:
            tmp_pos = std::make_pair(pos.first+1, pos.second);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::DOWN, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1+1000);
            }
            tmp_pos = std::make_pair(pos.first, pos.second+1);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::RIGHT, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1);
            }
            tmp_pos = std::make_pair(pos.first-1, pos.second);
            if (! (input[tmp_pos.first][tmp_pos.second] == '#' || visited.contains(tmp_pos))) {
                searchHelper(input, Dir::UP, tmp_pos, visited, best_visited, min_score_at_place, min_score, curScore+1+1000);
            }

            break;
    }

}

[[nodiscard]] std::tuple<u64, u64> search(const std::vector<std::vector<char>> &input, const std::pair<u64, u64> init_pos) {
    std::unordered_map<std::tuple<u64, u64, Dir>, u64, hash_pair> min_score_at_place;
    std::unordered_set<std::pair<u64, u64>, hash_pair> visited;
    std::unordered_set<std::pair<u64, u64>, hash_pair> best_visited;
    u64 min_score = INF;

    if (input[init_pos.first][init_pos.second-1] != '#') {
        searchHelper(input, Dir::LEFT, {init_pos.first, init_pos.second-1}, visited, best_visited, min_score_at_place, min_score, 1);
    }
    if (input[init_pos.first-1][init_pos.second] != '#') {
        searchHelper(input, Dir::UP, {init_pos.first-1, init_pos.second}, visited, best_visited, min_score_at_place, min_score, 1+1000);
    }
    if (input[init_pos.first+1][init_pos.second] != '#') {
        searchHelper(input, Dir::DOWN, {init_pos.first+1, init_pos.second}, visited, best_visited, min_score_at_place, min_score, 1+1000);
    }

    return {min_score, best_visited.size()+2};
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> t("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    auto inp = getInput(argv[1]);

    std::pair<u64, u64> init_pos;
    u64 count = 0;
    for (const auto&[i, r]: enumerate(inp)) {
        for (const auto&[j, c]: enumerate(r)) {
            if (c=='S') {
                init_pos = {i, j};
            }
            count += c=='.';
        }
    }
    std::cout << count << std::endl;
    auto [res1, res2] = search(inp, init_pos);
    std::cout << res1 << ',' << res2 << std::endl;
    return 0;
}