//
// Created by daniel on 12/23/24.
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

[[nodiscard]] TD findParents(const TD& td, const std::string& child)
{
    TD parents;
    std::deque<std::string> to_find{{child}};

    while (!to_find.empty())
    {
        auto c = to_find[0];
        to_find.pop_front();
        for (const auto&ele:td)
        {
            if (std::get<3>(ele) == c)
            {
                parents.emplace_back(ele);
                to_find.emplace_back(std::get<0>(ele));
                to_find.emplace_back(std::get<2>(ele));
                break;
            }
        }
    }
    return parents;
}

// [[nodiscard]] bool equiv_to(const TD& td, const TD& match)
// {
//     std::vector<std::string> inputs;
//     for (const auto&[i1, op, i2, out]: td)
//     {
//
//     }
// }
[[nodiscard]] std::optional<dt> calcz(WM wm, TD td)
{

    WM zs;
    size_t prev_size = 0;
    while (!td.empty() && prev_size != td.size())
    {
        prev_size = td.size();
        for (auto it = td.begin(); it != td.end();)
        {
            auto[i1, op, i2,v] = *it;
            if (wm.contains(i1) && wm.contains(i2))
            {
                auto tmp = doop(wm[i1], wm[i2], op);
                wm[v] = tmp;
                if (v[0] == 'z')
                {
                    zs[v] = tmp;
                }

                it = td.erase(it);
            }else
            {
                it++;
            }

        }
    }

    if (!td.empty()) {
        return std::nullopt;
    }
    dt N = 0;
    for (const auto&[k,v]: std::ranges::views::reverse(zs))
    {
        N = N << 1 | v;
    }
    return N;

}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
int main(const int argc, char *argv[])
{
    easyTimer<std::chrono::milliseconds> _("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    {
        auto [wm, td] = getInput(argv[1]);

        std::println("{}", calcz(wm, td).value());
    }
    std::print("\n\n\n");
    {
        auto [wm, td] = getInput(argv[1]);

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


        std::vector<std::pair<std::string, std::string>> bad = {{"qfs", "rcr"},
                                                                    {"cgn", "spp"}};
        bad.clear();

        std::vector<std::pair<std::string, std::string>> pre = {{"z05", "bpf"}, {"hcc","z11"}, {"hqc", "qcw"}, {"fdw", "z35"}};
        std::set<std::string> swapped;

        for (auto&[v1, v2]: pre)
        {
            std::vector<size_t> to_swap;
            for (const auto&[i,ele]: enumerate(td))
            {
                if (std::get<3>(ele)==v1 || std::get<3>(ele)==v2)
                {
                    to_swap.emplace_back(i);
                }
            }
            auto a = to_swap.at(0);
            auto b = to_swap.at(1);
            auto tmp = std::get<3>(td[a]);
            std::get<3>(td[a]) = std::get<3>(td[b]);
            std::get<3>(td[b]) = tmp;
            swapped.emplace(v1);
            swapped.emplace(v2);
        }


        auto Nz = calcz(wm, td).value();
        std::println("Nx+Ny={}", Nx+Ny);
        std::println("Nz={}", Nz);
        std::bitset<64> bitz(Nz);
        std::bitset<64> bita(Nx+Ny);

        for (size_t i = 0; i<bita.size();++i)
        {
            if (bitz[i] == bita[i])
            {
                continue;
            }
            std::println("{}", i);

            TD parents;
            for (const auto&[i1,op,i2,v]:td)
            {
                if (v == std::format("z{:02d}", i))
                {
                    parents = findParents(td, v);
                    break;
                }
            }

            // Add a breakpoint here to see what the error is
            // The output bit "zXX" should be an XOR of [xXX XOR yXX] XOR [the carry bit from zXX-1]
            std::vector<size_t> idxs(parents.size());
            std::iota(idxs.begin(), idxs.end(), 0);

            for (const auto& tmp1: iter::combinations(idxs, 4)) {
                auto a = tmp1[0];
                auto b = tmp1[1];
                auto c = tmp1[0];
                auto d = tmp1[1];

                if (a >= b || c >= d)
                {
                    continue;
                }

                WM wm_cpy = wm;
                TD par_cpy = parents;
                auto tmp = std::get<3>(par_cpy[a]);
                std::get<3>(par_cpy[a]) = std::get<3>(par_cpy[b]);
                std::get<3>(par_cpy[b]) = tmp;
                tmp = std::get<3>(par_cpy[c]);
                std::get<3>(par_cpy[c]) = std::get<3>(par_cpy[d]);
                std::get<3>(par_cpy[d]) = tmp;


                std::vector<std::pair<std::string, std::string>> pairs = {{std::get<3>(par_cpy[a]), std::get<3>(par_cpy[b])},
                                                                        {std::get<3>(par_cpy[c]),
                                                                        std::get<3>(par_cpy[d])}};

                const std::set<std::string> swapped_int = {std::get<3>(par_cpy[a]),
                                                        std::get<3>(par_cpy[b]),
                                                        std::get<3>(par_cpy[c]),
                                                            std::get<3>(par_cpy[d])};

                bool skip = false;
                for (const auto&[v1, v2]: bad)
                {
                    if (swapped_int.contains(v1) && swapped_int.contains(v2))
                    {
                        skip = true;
                        break;
                    }
                }
                if (skip)
                {
                    continue;
                }

                auto N_calc = calcz(wm_cpy, par_cpy);
                if (N_calc.has_value() && N_calc.value() == bita[i]) {

                    std::vector<size_t> to_swap;
                    for (const auto& [ele2, ele3]: pairs)
                    {
                        for (const auto&[i,ele]: enumerate(td))
                        {

                            if (std::get<3>(ele) == ele2 || std::get<3>(ele) == ele3)
                            {
                                to_swap.emplace_back(i);
                                break;
                            }
                        }
                    }
                    TD td_cpy = td;

                    // check that lower bits haven't been flipped
                    a = to_swap.at(0);
                    b = to_swap.at(1);
                    c = to_swap.at(2);
                    d = to_swap.at(3);
                    tmp = std::get<3>(td_cpy[a]);
                    std::get<3>(td_cpy[a]) = std::get<3>(td_cpy[b]);
                    std::get<3>(td_cpy[b]) = tmp;
                    tmp = std::get<3>(td_cpy[c]);
                    std::get<3>(td_cpy[c]) = std::get<3>(td_cpy[d]);
                    std::get<3>(td_cpy[d]) = tmp;

                    auto tmp_N = calcz(wm, td);
                    if (!tmp_N.has_value())
                    {
                        std::println("WEEWOO");
                    }
                    auto tmpn = tmp_N.value();
                    std::bitset<64> tmp_bits(tmpn);
                    bool good = true;
                    for (size_t j = 0; j<=i; j++)
                    {
                        if (bita[j] != tmp_bits[j])
                        {
                            good = false;
                            break;
                        }
                    }
                    if (good || true)
                    {
                        swapped.insert(swapped_int.begin(), swapped_int.end());
                        td = td_cpy;
                        break;
                    }
                }
            }
            auto tmp = calcz(wm, td);
            if (!tmp.has_value())
            {
                std::println("WEEWOO");
            }
            Nz = tmp.value();
            bitz = Nz;
            std::print("{}: ", Nz);
            for (const auto& ele: swapped) {
                std::print("{},", ele);
            }
            std::println("");
        }
        for (const auto& ele: swapped) {
            std::print("{},", ele);
        }
        std::println("");
    }

}
#pragma GCC diagnostic pop