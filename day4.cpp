//
// Created by vscode on 12/4/24.
//

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

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

int forwards(const std::vector<std::string>& inp, const std::string& word = MATCH) {
    int count = 0;

    // Forwards
    for (const auto& row: inp) {
        for (size_t i = 0; i <= row.size()-NCHARS; ++i) {
            count += row.substr(i, NCHARS) == word;
        }
    }

    // Vertical
    for(size_t i = 0; i<= inp.size()-NCHARS; ++i){
        for(size_t j = 0; j <= inp[i].size(); ++j){
            bool good = true;
            for(size_t k = 0; k < NCHARS; ++k){
                if(inp[i+k][j] != word[k] ){
                    good = false;
                    break;
                }
            }

            count += good;
        }
    }


    // \ diagninoal
    for(size_t i = 0; i<= inp.size()-NCHARS; ++i){
        for(size_t j = 0; j <= inp[i].size()-NCHARS; ++j){
            bool good = true;

            for(size_t k = 0; k < NCHARS; ++k){
                if(inp[i+k][j+k] != word[k] ){
                    good = false;
                    break;
                }
            }
            count += good;
        }
    }

    // / diagninoal
    for(size_t i = 0; i<= inp.size()-NCHARS; ++i){
        for(size_t j = NCHARS-1; j < inp[i].size(); ++j){
            bool good = true;
            for(size_t k = 0; k < NCHARS; ++k){
                if(inp[i+k][j-k] != word[k] ){
                    good = false;
                    break;
                }
            }
            count += good;
        }
    }

    return count;
}

int backwards(const std::vector<std::string>& inp){

    std::string back_match(MATCH);
    std::reverse(back_match.begin(), back_match.end());
    return forwards(inp, back_match);
}

int part2(const std::vector<std::string>& inp){
    int count = 0;
    for(size_t i = 1; i < inp.size()-1; i++){
        for(size_t j = 1; j < inp[i].size()-1; j++){
            if(inp[i][j] == 'A'){
                if(((inp[i-1][j-1] == 'M' && inp[i+1][j+1] == 'S') ||
                   (inp[i-1][j-1] == 'S' && inp[i+1][j+1] == 'M') ) &&
                   ((inp[i-1][j+1] == 'M' && inp[i+1][j-1] == 'S') ||
                   (inp[i-1][j+1] == 'S' && inp[i+1][j-1] == 'M') )){
                    count ++;
                   }
            }
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

    std::cout << "Part 1 solution: " << forwards(inp) + backwards(inp) << std::endl;
    std::cout << "Part 2 solution: " << part2(inp) << std::endl;
}