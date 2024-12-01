//
// Created by daniel on 12/1/24.
//



#include <vector>
#include <algorithm>
#include <cstdint>
#include <tuple>
#include <string>
#include <fstream>
#include <iostream>
#include <span>
#include <unordered_map>

using inp = std::vector<int>;
using map = std::unordered_map<int, uint64_t>;

class Counter {
public:
    Counter()= default;
    void add(const int val) {
        if (!counter.contains(val)) {
            counter[val] = 0;
        }
        counter[val]++;
    }
    uint64_t get(const int val) {
        if (!counter.contains(val)) {
            return 0;
        }
        return counter[val];
    }

    void add(const std::span<int>& vals) {
        for (const auto & ele: vals) {
            this->add(ele);
        }
    }
private:
    map counter;
};

std::tuple<inp, inp> getInput(const std::string& fp){
    inp c1;
    inp c2;
    std::ifstream file (fp);
    while (file) {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty()) {
            auto fs = tmp.find_first_of(' ');
            auto ls = tmp.find_last_of(' ');
            c1.push_back(std::stoi(tmp.substr(0, fs)));
            c2.push_back(std::stoi(tmp.substr(ls+1, tmp.size())));
        }

    }
    return std::make_tuple(c1, c2);
}

int main() {
    auto [inp1, inp2] = getInput("/IdeaProjects/avoc2024/inputs/input1");

    if (inp1.size() != inp2.size()) {
        std::cerr << "The 2 inputs are not the same sizes" << std::endl;
        return 1;
    }

    std::ranges::sort(inp1);
    std::ranges::sort(inp2);
    uint64_t sum = 0;
    for (size_t i = 0; i < inp1.size(); ++i) {
        sum += std::abs(inp1[i] - inp2[i]);
    }
    std::cout << "Part 1 Answer: " << sum << std::endl;

    Counter c;
    sum = 0;
    c.add(inp2);
    for (const auto& val: inp1) {
        sum += c.get(val) * val;
    }
    std::cout << "Part 2 Answer: " << sum << std::endl;

    return 0;
}