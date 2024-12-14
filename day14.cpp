//
// Created by vscode on 12/14/24.
//

#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "easyTimer.h"
#include <unordered_set>

using u64 = int64_t;

struct Robot{
    u64 x;
    u64 y;
    const u64 vx;
    const u64 vy;

    void step(const u64 width, const u64 height) {
        x = (width + ((x+vx) % width)) % width ;
        y = (height + ((y+vy) % height)) % height;
    }
};

std::vector<std::string> split(const std::string& inp, const char delim) {
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
};

std::vector<Robot> getInput(const std::string &fp)
{
    std::ifstream file(fp);
    std::vector<Robot> out;
    while (file)
    {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            auto l = split(tmp, ' ');
            auto l1 = split(l.at(0), '=');
            l1 = split(l1.at(1), ',');
            auto x = std::stoul(l1.at(0));
            auto y = std::stoul(l1.at(1));
            l1 = split(l.at(1), '=');
            l1 = split(l1.at(1), ',');
            auto vx = std::stoul(l1.at(0));
            auto vy = std::stoul(l1.at(1));
            out.emplace_back(x, y, vx, vy);
        }
    }
    return out;
}


int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> t("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }
    constexpr u64 W = 101;
    constexpr u64 H = 103;

    const auto inp = getInput(argv[1]);
    {
        auto robots = inp;
        for (int i = 0; i < 100; ++i) {
            for (auto& r: robots) {
                r.step(W, H);
            }
        }
        std::array<int, 4> quad_counts = {0,0,0,0};
        for (auto& r: robots) {
            if (r.x > W/2 && r.y > H/2) {
                quad_counts[3]++;
            }else if (r.x < W/2 && r.y > H/2) {
                quad_counts[2]++;
            } else if (r.x > W/2 && r.y < H/2) {
                quad_counts[1]++;
            } else if (r.x < W/2 && r.y < H/2) {
                quad_counts[0]++;
            }
        }

        std::cout << quad_counts[0]*quad_counts[1]*quad_counts[2]*quad_counts[3] << std::endl;
    }

    {
        auto robots = inp;
        std::unordered_set<std::pair<int, int>, hash_pair> poses;
        u64 count = 0;
        while (poses.size() != robots.size()) {
            poses.clear();
            for (auto& r: robots) {
                r.step(W, H);
                poses.emplace(r.x, r.y);
            }
            ++count;
        }
        // for (u64 i = 0; i < H; ++i) {
        //     for (u64 j = 0; j < W; ++j) {
        //         if (poses.contains(std::make_pair(i, j))) {
        //             std::cout << "1";
        //         }else {
        //             std::cout << '.';
        //         }
        //     }
        //     std::cout << std::endl;
        // }
        std::cout << count << std::endl;
    }
}