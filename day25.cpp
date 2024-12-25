//
// Created by daniel on 12/24/24.
//
#include <bitset>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "easyTimer.h"
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>
#include <cinttypes>
#include <deque>
#include <numeric>

#include "utils/utils.h"
#include "cppitertools/cppitertools/combinations.hpp"

#include <print>

using dt = uintmax_t;

constexpr dt INF = std::numeric_limits<dt>::max();

template <typename T>
bool vecsumeq(const std::vector<T>& v1, const std::vector<T>& v2, const T& val)
{
    if (v1.size() != v2.size())
    {
        return false;
    }
    for (size_t i = 0; i< v1.size(); i++)
    {
        if ((v1[i]+v2[i]) > val)
        {
            return false;
        }
    }
    return true;
}

std::tuple<std::vector<std::vector<int>>, std::vector<std::vector<int>>> getInput(const std::string &fp){
    std::ifstream file(fp);
    std::vector<std::vector<int>> locks;
    std::vector<std::vector<int>> keys;
    std::vector<std::string> tmp_m;

    while (file)
    {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            tmp_m.emplace_back(tmp);
        }
        else
        {
            bool forLock = tmp_m[0][0] == '#';
            std::vector<int> lens;
            if (forLock)
            {

                for (size_t j = 0; j<tmp_m[0].size(); j++)
                {
                    size_t c = 0;
                    for (c = 0; c<tmp_m.size() && tmp_m[c][j] == '#'; c++)
                    {}
                    lens.emplace_back(c-1);
                }
                locks.emplace_back(lens);
            }
            else
            {
                for (size_t j = 0; j<tmp_m[0].size(); j++)
                {
                    size_t c = 0;
                    for (c = 0; c<tmp_m.size() && tmp_m[tmp_m.size()-1-c][j] == '#'; c++)
                    {}
                    lens.emplace_back(c-1);
                }
                keys.emplace_back(lens);
            }
            tmp_m.clear();
        }

    }

    return {locks, keys};
}

int main(const int argc, char *argv[])
{
    easyTimer<std::chrono::milliseconds> _("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const auto [locks, keys] = getInput(argv[1]);

    std::println("{}, {}", locks.size(), keys.size());
    std::unordered_map<int, std::vector<std::vector<int>>> lock_map;
    for (const auto& l: locks)
    {
        lock_map[l[0]].emplace_back(l);
    }
    int count = 0;
    for (const auto& k: keys)
    {
        for (int i = 5; (i-k[0]) >= 0; i--)
        {
            auto poss = lock_map[i-k[0]];
            for (const auto& p: poss)
            {
                count += vecsumeq(p, k, 5);
            }
        }
    }
    std::println("{}", count);
}