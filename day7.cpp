

#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <sstream>
#include <limits>
#include <ranges>
#include <unordered_map>
#include <math.h>

using dtype = uint64_t;

std::vector<std::string> split(const std::string& inp, const char delim) {
    std::string tmp;
    std::vector<std::string> r;
    std::istringstream s(inp);
    while (std::getline(s, tmp, delim)) {
        r.push_back(tmp);
    }
    return r;
}

struct Equation {
    dtype test_value;
    std::vector<dtype> numbers;

    explicit Equation(const std::string& line) {
        const auto tmp = split(line, ':');
        test_value = std::stoul(tmp[0]);
        for (const auto& ele: split(tmp[1], ' ')) {

            if (!ele.empty()) {
                numbers.emplace_back(std::stoul(ele));
            }
        }

    }
};

std::vector<Equation> getInput(const std::string& fp){
    std::ifstream file (fp);
    std::vector<Equation> out;
    while(file){
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty()) {
            out.emplace_back(tmp);
        }
    }
    return out;
}

dtype add(dtype a, dtype b) {
    return a+b;
}

dtype mult(dtype a, dtype b) {
    return a*b;
}

bool is_equ_helper(const dtype target, const dtype current, const std::span<dtype>& remaining) {
    if (current > target) {
        return false;
    }
    if (current == target && remaining.empty()) {
        return true;
    }
    if (remaining.empty()) {
        return false;
    }
    auto tmp1 = current+remaining[0];
    auto tmp2 = current*remaining[0];
    const auto next = std::span{remaining}.subspan(1);

    if (is_equ_helper(target, tmp1, next)) {
        return true;
    }
    if (is_equ_helper(target, tmp2, next)) {
        return true;
    }
    return false;
}

bool is_equ(Equation& equ) {

    const auto next = std::span{equ.numbers}.subspan(1);
    return is_equ_helper(equ.test_value, equ.numbers[0], next);
}

bool is_equ_helper2(const dtype target, const dtype current, const std::span<dtype>& remaining) {
    if (current > target) {
        return false;
    }
    if (current == target && remaining.empty()) {
        return true;
    }
    if (remaining.empty()) {
        return false;
    }
    auto tmp1 = current+remaining[0];
    auto tmp2 = current*remaining[0];
    auto tmp3 = std::stoul(std::to_string(current) + std::to_string(remaining[0]));
    const auto next = std::span{remaining}.subspan(1);

    if (is_equ_helper2(target, tmp1, next)) {
        return true;
    }
    if (is_equ_helper2(target, tmp2, next)) {
        return true;
    }
    if (is_equ_helper2(target, tmp3, next)) {
        return true;
    }
    return false;
}

bool is_equ2(Equation& equ) {
    auto tmp = equ.numbers[0];
    const auto next = std::span{equ.numbers}.subspan(1);
    return is_equ_helper2(equ.test_value, tmp, next);
}

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

       auto inp = getInput(argv[1]);

    dtype sum = 0;
    for (auto equation: inp) {
        if (is_equ(equation)) {
            // std::cout << equation.test_value << std::endl;
            sum += equation.test_value;
        }
        // break;
    }
    // Not 662526278095
    // Not 662526277245
    std::cout << sum << std::endl;

    sum = 0;
    for (auto equation: inp) {
        if (is_equ2(equation)) {
            std::cout << equation.test_value << std::endl;
            sum += equation.test_value;
        }
        // break;
    }
    std::cout << sum << std::endl;
    return 0;

}