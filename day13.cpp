
#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include "easyTimer.h"
#include <unordered_set>

using u64 = uint64_t;

struct ClawMachine{
    u64 a_x;
    u64 a_y;
    u64 b_x;
    u64 b_y;
    u64 X;
    u64 Y;
};

auto split(const std::string& inp, const char delim) {
    std::string tmp;
    std::vector<std::string> r;
    std::istringstream s(inp);
    while (std::getline(s, tmp, delim)) {
        r.push_back(tmp);
    }
    return r;
}

std::vector<ClawMachine> getInput(const std::string &fp)
{
    std::ifstream file(fp);
    std::vector<ClawMachine> out;
    while (file)
    {

        std::string tmp1, tmp2, tmp3;
        std::getline(file, tmp1);
        std::getline(file, tmp2);
        std::getline(file, tmp3);

        if (!tmp1.empty())
        {
            auto l1 = split(tmp1, ':');
            l1 = split(l1.at(1), ',');

            auto a_x = std::stoul(split(l1.at(0), '+').at(1));
            auto a_y = std::stoul(split(l1.at(1), '+').at(1));

            auto l2 = split(tmp2, ':');
            l2 = split(l2.at(1), ',');

            auto b_x = std::stoul(split(l2.at(0), '+').at(1));
            auto b_y = std::stoul(split(l2.at(1), '+').at(1));

            auto l3 = split(tmp3, ':');
            l3 = split(l3.at(1), ',');

            auto X = std::stoul(split(l3.at(0), '=').at(1));
            auto Y = std::stoul(split(l3.at(1), '=').at(1));

            out.emplace_back(a_x, a_y, b_x, b_y, X, Y);

        }

        //eat the blank line
        std::getline(file, tmp1);
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

    auto inp = getInput(argv[1]);

    {
        u64 count = 0;
        for(const auto& ele: inp){
            // std::cout << ele.a_x << ',' << ele.a_y<< ',' << ele.b_x<< ',' << ele.b_y<< ',' << ele.X<< ',' << ele.Y << std::endl;

            auto n1 = (ele.Y*ele.a_x);
            auto n2 = (ele.X*ele.a_y);
            auto n3 = (ele.b_y*ele.a_x);
            auto n4 = (ele.b_x*ele.a_y);
            if(n3 == n4){
                continue;
            }
            if(n2>n1 && n4>n3){
                std::swap(n1, n2);
                std::swap(n3, n4);
            }

            auto numer = n1-n2;
            auto denom = n3-n4;
            auto M = numer/denom;
            auto r = numer % denom;
            std::cout <<"M" << M << '|' << r << std::endl;
            if(r==0 && M <= 100){
                if(ele.b_x*M > ele.X){
                    continue;
                }
                numer = ele.X - ele.b_x*M;
                denom = ele.a_x;

                auto N = numer / denom;
                r = numer % denom;
                    std::cout <<"N"<< N << '|' << r << std::endl;

                if(r==0 && N <= 100){
                    count += 3*N +1*M;
                }
            }
        }
        std::cout << count << std::endl;
    }

    {
        u64 count = 0;
    for(auto& ele: inp){
        // std::cout << ele.a_x << ',' << ele.a_y<< ',' << ele.b_x<< ',' << ele.b_y<< ',' << ele.X<< ',' << ele.Y << std::endl;
        ele.X += 10000000000000;
        ele.Y += 10000000000000;
        auto n1 = (ele.Y*ele.a_x);
        auto n2 = (ele.X*ele.a_y);
        auto n3 = (ele.b_y*ele.a_x);
        auto n4 = (ele.b_x*ele.a_y);
        if(n3 == n4){
            continue;
        }
        if(n2>n1 && n4>n3){
            std::swap(n1, n2);
            std::swap(n3, n4);
        }

        auto numer = n1-n2;
        auto denom = n3-n4;
        auto M = numer/denom;
        auto r = numer % denom;
        std::cout <<"M" << M << '|' << r << std::endl;
        if(r==0){
            if(ele.b_x*M > ele.X){
                continue;
            }
            numer = ele.X - ele.b_x*M;
            denom = ele.a_x;

            auto N = numer / denom;
            r = numer % denom;
                std::cout <<"N"<< N << '|' << r << std::endl;

            if(r==0){
                count += 3*N +1*M;
            }
        }
    }
    std::cout << count << std::endl;
    }
    return 0;
}