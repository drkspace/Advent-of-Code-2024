#include <ranges>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <cmath>
#include "easyTimer.h"
#include <unordered_map>
#include <ranges>

using u64 = uint64_t;

std::list<u64> split(const std::string &inp, const char delim)
{
    std::string tmp;
    std::list<u64> r;
    std::istringstream s(inp);
    while (std::getline(s, tmp, delim))
    {
        r.push_back(std::stoul(tmp));
    }
    return r;
}

std::list<u64> getInput(const std::string &fp)
{
    std::ifstream file(fp);
    std::list<u64> out;
    while (file)
    {

        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            out = split(tmp, ' ');
        }
    }
    return out;
}

template <size_t DEPTH>
u64 run(std::array<std::unordered_map<u64, u64>, DEPTH>& shortcuts, size_t curDepth, u64 value){
    if(curDepth == DEPTH){
        return 1;
    }

    if(!shortcuts[curDepth].contains(value)){

        if(value == 0){
            shortcuts[curDepth][value] = run(shortcuts, curDepth+1, 1);
        }
        else
        {
            const u64 l = std::floor(std::log10(value) + 1);
            // std::cout << l << ":" << *it << std::endl;
            if (l % 2 == 0)
            {
                auto s = static_cast<int>(std::pow(10, l / 2));

                auto front = (value) / s;
                auto end = (value) % s;

                shortcuts[curDepth][value] = run(shortcuts, curDepth+1, front) + run(shortcuts, curDepth+1, end);
            }
            else
            {
                // std::cout << "r2024" << ":" << *it << std::endl;
                shortcuts[curDepth][value] = run(shortcuts, curDepth+1, value*2024);
            }
        }

    }

    return shortcuts[curDepth][value];
}

int main(const int argc, char *argv[])
{
    easyTimer<std::chrono::milliseconds> t("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    auto inp = getInput(argv[1]);

    const size_t N = 75;
    {
        easyTimer<std::chrono::milliseconds> t("t2 ");
        std::array<std::unordered_map<u64, u64>, N> scs;
        u64 count = 0;
        for(const auto& ele: inp){
            count += run(scs, 0, ele);
        }
        std::cout << count << std::endl;
    }
}