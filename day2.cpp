
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using row = std::vector<int>;
using grid = std::vector<row>;

row split(const std::string& inp, const char delim) {
    std::string tmp;
    row r;
    std::istringstream s(inp);
    while (std::getline(s, tmp, delim)) {
        r.push_back(std::stoi(tmp));
    }
    return r;
}

grid getInput(const std::string& fp){
    grid g;
    std::ifstream file (fp);
    while (file) {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty()) {
            g.push_back(split(tmp, ' '));
        }
    }
    return g;
}

row diff1d(const row& r) {
    row diff;
    for (std::size_t i = 0; i < r.size()-1; ++i) {
        diff.push_back(r[i]-r[i+1]);
    }
    return diff;
}

bool good(const row& r) {

    auto diff = diff1d(r);

    if (std::abs(diff.at(0)) == 0) {
        return false;
    }

    if (diff.at(0) > 0) {
        auto [min, max] = std::ranges::minmax_element(diff);
        return *max <=3 && *min >= 1;
    }
    else {
        auto [min, max] = std::ranges::minmax_element(diff);
        return *min >= -3 && *max <= -1;
    }
}

int main() {

    auto g = getInput("/IdeaProjects/avoc2024/inputs/input2");

    int count = 0;

    for (const auto& r : g) {
        count += good(r);
    }

    std::cout << "Part 1 Answer: " << count << std::endl;

    count = 0;
    for (const auto & r: g) {
        if (good(r)) {
            count += 1;
            continue;
        }

        for (std::size_t i = 0; i < r.size(); ++i) {
            auto cpy(r);
            cpy.erase(cpy.begin() + i);
            if (good(cpy)) {
                count += 1;
                break;
            }
        }
    }

    std::cout << "Part 2 Answer: " << count << std::endl;


    return 0;
}