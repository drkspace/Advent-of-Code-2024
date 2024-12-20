#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "easyTimer.h"
#include <unordered_set>
#include <unordered_map>
#include <cinttypes>
#include "utils/utils.h"
#include <optional>
using dt = intmax_t;

constexpr dt INF = std::numeric_limits<dt>::max()/2;

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

void searchHelper(const std::vector<std::vector<char>> &input, const std::pair<dt, dt> pos, std::vector<std::vector<dt>>& min_score_at_place, dt& min_score, dt curScore) {
    if (curScore > min_score) {
        return;
    }
    if (input[pos.first][pos.second] == 'E') {

        if (curScore < min_score) {
            std::cout <<  "cur best: " <<curScore << std::endl;
            min_score = curScore;
            min_score_at_place[pos.first][pos.second] = min_score;
            return;
        }
    }

    if (curScore >= min_score_at_place[pos.first][pos.second]) {
        return;
    }

    min_score_at_place[pos.first][pos.second] = curScore;

    // if (min_score_at_place.size()%128 == 0) {
    //     std::cout << "visited size: " << min_score_at_place.size() << std::endl;
    // }
    // std::cout << visited.size() << ':' << pos.first << ',' << pos.second << std::endl;
    std::pair<dt, dt> tmp_pos;
    for(const auto&v: std::initializer_list<std::pair<dt, dt>>{{0, 1}, {1,0}, {-1, 0}, {0, -1}}) {
        tmp_pos = pos + v;
        if (! (input[tmp_pos.first][tmp_pos.second] == '#')) {
            searchHelper(input, tmp_pos, min_score_at_place, min_score, curScore+1);
        }
    }


}

[[nodiscard]] std::tuple<dt, dt> search(const std::vector<std::vector<char>> &input, const std::pair<dt, dt> init_pos) {
    std::vector<std::vector<dt>> min_score_at_place;
    for(size_t i = 0; i< input.size(); i++){
        min_score_at_place.emplace_back(input[0].size(), INF);
    }
    // std::unordered_set<std::pair<dt, dt>, hash_tuple> visited;
    // std::unordered_set<std::pair<dt, dt>, hash_tuple> best_visited;
    dt min_score = INF;

    for(const auto&v: std::initializer_list<std::pair<dt, dt>>{{0, 1}, {1,0}, {-1, 0}, {0, -1}}) {
        auto tmp_pos = init_pos + v;
        if (! (input[tmp_pos.first][tmp_pos.second] == '#')) {
            searchHelper(input, tmp_pos, min_score_at_place, min_score, 1);
        }
    }

    std::unordered_map<std::tuple<dt,dt, dt,dt>,dt, hash_tuple> se;
    for (size_t i = 0; i< input.size(); i++) {
        for (size_t j = 0; j<input[0].size(); j++) {
            if (input[i][j] != '#' && input[i][j] != 'E') {
                std::pair<dt, dt> pos{i, j};
                auto init_score = min_score-min_score_at_place[pos.first][pos.second];
                for(const auto&[v, is]: std::initializer_list<std::pair<std::pair<dt, dt>, std::pair<dt, dt>>>{{{0, 2}, {0,1}},
                                                                                                                                                        {{2,0}, {1,0}}, {{-2,0}, {-1,0}},
                                                                                                                                                        {{0,-2}, {0,-1}}, {{1, 1}, {1, 0}},
                                                                                                                                                        {{1, 1}, {0, 1}}, {{1, -1}, {1, 0}},
                                                                                                                                                        {{1, -1}, {0, -1}}, {{-1, 1}, {-1, 0}},
                                                                                                                                                        {{-1, 1}, {0, 1}}, {{-1, -1}, {-1, 0}},
                                                                                                                                                        {{-1, -1}, {0, -1}}}) {
                    auto tmp_pos = pos + v;
                    auto fw = pos+is;
                    if (tmp_pos.first < 0 || tmp_pos.first >= static_cast<dt>(input.size()) || tmp_pos.second < 0 || tmp_pos.second >= static_cast<dt>(input[0].size())) {
                        continue;
                    }

                    if (input[tmp_pos.first][tmp_pos.second] != '#') {
                        if (input[fw.first][fw.second] != '#') {
                            continue;
                        }

                        auto new_score = min_score-min_score_at_place[tmp_pos.first][tmp_pos.second];
                        if ((init_score-new_score-2) >= 100) {
                            // std::cout << pos << "=" << init_score<< "|" << tmp_pos << "=" << new_score << std::endl;
                            se[{pos.first, pos.second, tmp_pos.first, tmp_pos.second}] = (init_score-new_score+2);
                        }

                    }
                }
            }
        }
    }

    return {min_score, se.size()};
}

[[nodiscard]] std::tuple<dt, dt> search2(const std::vector<std::vector<char>> &input, const std::pair<dt, dt> init_pos) {
    std::vector<std::vector<dt>> min_score_at_place;
    for(size_t i = 0; i< input.size(); i++){
        min_score_at_place.emplace_back(input[0].size(), INF);
    }
    dt min_score = INF;

    for(const auto&v: std::initializer_list<std::pair<dt, dt>>{{0, 1}, {1,0}, {-1, 0}, {0, -1}}) {
        auto tmp_pos = init_pos + v;
        if (! (input[tmp_pos.first][tmp_pos.second] == '#')) {
            searchHelper(input, tmp_pos, min_score_at_place, min_score, 1);
        }
    }
    min_score_at_place[init_pos.first][init_pos.second] = 0;
    std::unordered_map<std::tuple<dt,dt, dt,dt>,dt, hash_tuple> se;
    for (size_t i = 0; i< input.size(); i++) {
        for (size_t j = 0; j<input[0].size(); j++) {
            if (input[i][j] != '#' && input[i][j] != 'E') {
                std::pair<dt, dt> pos{i, j};
                auto init_score = min_score-min_score_at_place[pos.first][pos.second];
                for(const auto&v: std::initializer_list<std::pair<dt, dt>>{{0, 1}, {1,0}, {-1, 0}, {0, -1}}){
                    auto fw = pos+v;
                    if (input[fw.first][fw.second] != '#' && false) {
                        continue;
                    }
                    for (dt ii = -19; ii <= (20-1); ++ii) {
                        for (dt jj = -19; jj <= (20-1); ++jj) {
                            auto tcd = std::abs(ii)+std::abs(jj);
                            if (tcd > 19) {
                                continue;
                            }
                            auto tmp_pos = fw + std::make_pair(ii, jj);

                            if (tmp_pos.first < 0 || tmp_pos.first >= static_cast<dt>(input.size()) || tmp_pos.second < 0 || tmp_pos.second >= static_cast<dt>(input[0].size())) {
                                continue;
                            }

                            if (input[tmp_pos.first][tmp_pos.second] != '#') {

                                auto new_score = min_score-min_score_at_place[tmp_pos.first][tmp_pos.second];
                                if ((init_score-new_score-1-tcd) >= 100) {
                                    se[{pos.first, pos.second, tmp_pos.first, tmp_pos.second}] = (init_score-new_score-1-tcd);
                                }

                            }
                        }
                    }

                }
            }
        }
    }

    return {min_score, se.size()};
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> _("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const auto inp = getInput(argv[1]);
    std::pair<dt, dt> init_pos;
    for (size_t i = 0; i < inp.size(); ++i) {
        for (size_t j = 0; j < inp[i].size(); ++j) {
            if (inp[i][j] == 'S') {
                init_pos = {i,j};
                break;
            }
        }
    }
    auto [r, r1] = search(inp, init_pos);
    std::cout << r1 << std::endl;
    auto [rr, r2] = search2(inp, init_pos);
    // lt 1232055
    std::cout << r2 << std::endl;
}