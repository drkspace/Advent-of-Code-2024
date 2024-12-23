//
// Created by daniel on 12/22/24.
//
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "easyTimer.h"
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <cinttypes>
#include "utils/utils.h"

#include <print>

using dt = uintmax_t;

constexpr dt INF = std::numeric_limits<dt>::max();
using P = Point2d<dt>;

std::vector<std::pair<std::string, std::string>> getInput(const std::string &fp){
    std::ifstream file(fp);
    std::vector<std::pair<std::string, std::string>> tmp_m;
    while (file)
    {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            auto l = split(tmp, '-');
            std::string a = l[0];
            std::string b = l[1];
            if (a < b)
            {
                std::swap(a, b);
            }
            tmp_m.emplace_back(a, b);
        }
    }

    return tmp_m;
}


int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> _("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const auto inp = getInput(argv[1]);
    std::println("{}", inp.size());

    std::unordered_map<std::string, std::set<std::string>> m;
    for (const auto&[a,b]: inp)
    {
        m[a].emplace(b);
        m[b].emplace(a);
    }

    int count = 0;
    for (const auto&[k,v]: m)
    {
        if (v.size()>=2)
        {
            for (const auto&s:v)
            {
                for (const auto& s1: m[s])
                {
                    if (s1 == k)
                    {
                        continue;
                    }
                    if (m[s1].contains(k))
                    {
                        if (k[0]=='t'||s[0]=='t'||s1[0]=='t')
                        {
                            count++;
                        }
                    }
                }
            }

        }
    }
    std::println("{}", count/6);
    size_t best = 0;
    std::set<std::string> best_comp;
    for (auto[k,v]: m)
    {
        v.emplace(k);
        std::set<std::string> comb(v.begin(), v.end());
        for (const auto& ele : v)
        {
            if (!comb.contains(ele))
            {
                continue;
            }
            auto tmp = m[ele];
            tmp.emplace(ele);

            std::vector<std::string> inter, tmp2;
            std::set_intersection(v.begin(), v.end(), tmp.begin(), tmp.end(),  std::back_inserter(inter));
            std::sort(inter.begin(), inter.end());
            std::set_intersection(inter.begin(), inter.end(), comb.begin(), comb.end(),  std::back_inserter(tmp2));
            comb.clear();
            comb.insert(tmp2.begin(), tmp2.end());


        }
        if (comb.size() > best)
        {
            best = comb.size();
            best_comp.clear();
            best_comp.insert(comb.begin(), comb.end());
        }
    }
    std::println("{}", best);
    for (const auto&ele:best_comp)
    {
        std::print("{},", ele);
    }
    std::println("");
}