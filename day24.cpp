//
// Created by daniel on 12/23/24.
//
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
#include <numeric>

#include "utils/utils.h"
#include "cppitertools/cppitertools/combinations.hpp"

#include <print>

using dt = uintmax_t;

constexpr dt INF = std::numeric_limits<dt>::max();

enum class OP
{
    XOR,OR,AND
};

OP stoop(const std::string& op)
{

    if (op=="XOR")
    {
        return OP::XOR;
    }else if (op=="OR")
    {
        return OP::OR;
    }else if (op=="AND")
    {
        return OP::AND;
    }else
    {
        return OP::AND;
    }

}

using WM = std::map<std::string, int>;
using TD = std::vector<std::tuple<std::string, OP, std::string, std::string>>;

std::tuple<WM, TD> getInput(const std::string &fp){
    std::ifstream file(fp);
    std::vector<std::string> tmp_m;
    WM wm;
    TD td;
    while (file)
    {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            auto l = split(tmp, ':');
            wm[l[0]] = std::stoi(l[1]);
        }else
        {
            break;
        }
    }
    while (file)
    {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            auto l = split(tmp, ' ');
            auto op = stoop(l[1]);
            td.emplace_back(l[0], op, l[2], l[4]);
        }
    }

    return {wm, td};
}

[[nodiscard]] int doop(const int v1, const int v2, const OP op)
{
    switch (op)
    {
    case OP::OR:
        return v1 | v2;
    case OP::XOR:
        return v1^v2;
    case OP::AND:
        return v1 & v2;
    default:
        return 0;
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(const int argc, char *argv[])
{
    easyTimer<std::chrono::milliseconds> _("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    // {
    //     auto [wm, td] = getInput(argv[1]);
    //
    //     WM zs;
    //     while (!td.empty())
    //     {
    //         for (auto it = td.begin(); it != td.end();)
    //         {
    //             auto[k,vs] = *it;
    //             if (const auto& [i1, op, i2] = k; wm.contains(i1) && wm.contains(i2))
    //             {
    //                 for (const auto& v: vs)
    //                 {
    //                     auto tmp = doop(wm[i1], wm[i2], op);
    //                     wm[v] = tmp;
    //                     if (v[0] == 'z')
    //                     {
    //                         zs[v] = tmp;
    //                     }
    //
    //                 }
    //                 it = td.erase(it);
    //             }else
    //             {
    //                 it++;
    //             }
    //
    //         }
    //     }
    //
    //     dt N = 0;
    //     for (const auto&[k,v]: std::ranges::views::reverse(zs))
    //     {
    //         std::println("{}: {}", k, v);
    //         N = N << 1 | v;
    //     }
    //     std::println("{}", N);
    // }

    {
        const auto [wm, td] = getInput(argv[1]);

        dt Nx = 0;
        dt Ny = 0;
        for (const auto&[k,v]: std::ranges::views::reverse(wm))
        {
            if (k[0] == 'x')
            {
                Nx = Nx << 1 | v;
            }else if (k[0] == 'y')
            {
                Ny = Ny << 1 | v;
            }
        }

        // std::unordered_set<std::unordered_set<size_t>> done = {{0}};
        std::vector<size_t> idxs(td.size());
        std::iota(idxs.begin(), idxs.end(), 0);
        for (const auto& tmp1: iter::combinations(idxs, 8)){
            auto a = tmp1[0];
            auto b = tmp1[1];
            auto c = tmp1[2];
            auto d = tmp1[3];
            auto e = tmp1[4];
            auto f = tmp1[5];
            auto g = tmp1[6];
            auto h = tmp1[7];
            // std::println("{},{},{},{},{},{},{},{}", a, b, c, d, e, f, g, h);
            if (a >= b || c>=d || e>=f || g>=h)
            {
                continue;
            }

            if (g%25==0)
            {
                std::println("{},{},{},{},{},{},{},{}", a, b, c, d, e, f, g, h);
            }
            WM zs;
            WM wm_cpy = wm;
            TD td_cpy = td;
            auto tmp = std::get<3>(td_cpy[a]);
            std::get<3>(td_cpy[a]) = std::get<3>(td_cpy[b]);
            std::get<3>(td_cpy[b]) = tmp;
            tmp = std::get<3>(td_cpy[c]);
            std::get<3>(td_cpy[c]) = std::get<3>(td_cpy[d]);
            std::get<3>(td_cpy[d]) = tmp;
            tmp = std::get<3>(td_cpy[e]);
            std::get<3>(td_cpy[e]) = std::get<3>(td_cpy[f]);
            std::get<3>(td_cpy[f]) = tmp;
            tmp = std::get<3>(td_cpy[g]);
            std::get<3>(td_cpy[g]) = std::get<3>(td_cpy[h]);
            std::get<3>(td_cpy[h]) = tmp;

            const std::set<std::string> swapped = {std::get<3>(td_cpy[a]),
                                                        std::get<3>(td_cpy[b]),
                                                        std::get<3>(td_cpy[c]),
                                                        std::get<3>(td_cpy[d]),
                                                            std::get<3>(td_cpy[e]),
                                                            std::get<3>(td_cpy[f]),
                                                            std::get<3>(td_cpy[g]),
                                                            std::get<3>(td_cpy[h])};

            size_t prev_length = 0;
            while (!td_cpy.empty() && td_cpy.size() != prev_length)
            {
                prev_length = td_cpy.size();
                for (auto it = td_cpy.begin(); it != td_cpy.end();)
                {
                    if (auto[i1, op, i2, v] = *it; wm_cpy.contains(i1) && wm_cpy.contains(i2))
                    {
                            auto tmp = doop(wm_cpy[i1], wm_cpy[i2], op);
                            wm_cpy[v] = tmp;
                            if (v[0] == 'z')
                            {
                                zs[v] = tmp;
                            }
                        it = td_cpy.erase(it);
                    }else
                    {
                        it++;
                    }

                }
            }

            dt N = 0;
            for (const auto&[k,v]: std::ranges::views::reverse(zs))
            {
                // std::println("{}: {}", k, v);
                N = N << 1 | v;
            }
            // std::println("{} == {}", N, Nx&Ny);
            if (N == (Nx&Ny))
            {
                for (const auto& ele: swapped)
                {
                    std::print("{},", ele);
                }
                std::println("");
                break;
            }
        }
    }
}
#pragma GCC diagnostic pop