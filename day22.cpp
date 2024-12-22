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
#include <cmath>
#include <queue>

using dt = uintmax_t;

constexpr dt INF = std::numeric_limits<dt>::max();

std::vector<dt> getInput(const std::string &fp){
    std::ifstream file(fp);
    std::vector<dt> tmp_m;
    while (file)
    {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            tmp_m.emplace_back(std::strtoumax(tmp.c_str(), nullptr, 10));
        }
    }

    return tmp_m;
}

dt rng(dt seed) {
    constexpr dt PRUNE = 16777216;
    auto a = seed*64;
    seed = a^seed;
    seed = seed%PRUNE;

    a = seed/32;
    seed = a^seed;
    seed = seed%PRUNE;

    a = seed*2048;
    seed = a^seed;
    seed = seed%PRUNE;

    return seed;
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> _("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const auto inp = getInput(argv[1]);
    dt sum = 0;
    std::unordered_map<std::tuple<int, int, int, int>, std::unordered_map<int, int>, hash_tuple> seqs;
    for (auto [idx, _i] : enumerate(inp)) {
        std::vector<int> queue;
        auto i = _i;
        auto prev = i;
        i = rng(i);
        sum += i;
        queue.emplace_back((i%10) - (prev%10));
        prev = i;
        i = rng(i);
        sum += i;
        queue.insert(queue.begin(), (i%10) - (prev%10));
        prev = i;
        i = rng(i);
        sum += i;
        queue.insert(queue.begin(), (i%10) - (prev%10));
        prev = i;
        i = rng(i);
        sum += i;
        queue.insert(queue.begin(), (i%10) - (prev%10));
        for (int c = 0; c < 2000-4; c++) {

            if (!seqs[{queue[0], queue[1], queue[2], queue[3]}].contains(idx)) {
                seqs[{queue[0], queue[1], queue[2], queue[3]}][idx] = i%10;
            }

            prev = i;
            i = rng(i);
            queue.pop_back();
            queue.insert(queue.begin(), (i%10) - (prev%10));



        }
        sum += i;
    }
    std::cout << sum << std::endl;

    dt best = 0;
    std::tuple<int, int, int, int> best_seq;
    for (const auto&[seq, sm]: seqs) {
        sum = 0;
        for (const auto&[k, v]: sm) {
            sum += v;
        }
        if (sum > best) {
            best = sum;
            best_seq = seq;
        }

    }
    //neq 2164
    //neq 2148
    std::cout << best << ';' << std::get<0>(best_seq) << ',' << std::get<1>(best_seq) << ',' << std::get<2>(best_seq) << ','<<std::get<3>(best_seq) << ',' << std::endl;
    return 0;
}