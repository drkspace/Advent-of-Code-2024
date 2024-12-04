//
// Created by vscode on 12/4/24.
//

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

constexpr auto MATCH = "XMAS";
constexpr size_t NCHARS = 4;

std::vector<std::string> getInput(const std::string& fp){
    std::ifstream file (fp);
    std::vector<std::string> out;
    while(file){
     std::string tmp;
     std::getline(file, tmp);
        if (!tmp.empty()) {
            out.push_back(tmp);
        }

      }
      return out;
}

int forwards(const std::vector<std::string>& inp) {
    int count = 0;
    for (const auto& row: inp) {
        for (size_t i = 0; i < row.size()-NCHARS-1; ++i) {
            count += row.substr(i, NCHARS) == MATCH;
        }
    }

    return count;
}

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const auto inp = getInput(argv[1]);

    for(const auto& row : inp){
        std::cout << row << std::endl;
    }
    std::cout << forwards(inp) << std::endl;
}