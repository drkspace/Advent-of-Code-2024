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

constexpr int64_t SENTINAL = -1;

std::vector<int> getInput(const std::string& fp){
    std::ifstream file (fp);
    std::vector<int> out;
    while(file){
     std::string tmp;
     std::getline(file, tmp);
        if (!tmp.empty()) {
            for(const auto& c: tmp){
                out.emplace_back(std::stoi(std::string(1, c)));
            }
        }
      }
      return out;
}

u_int64_t checksum(const std::vector<int64_t>& s){
    u_int64_t sum = 0;
    for(size_t i = 0; i< s.size(); i++){
        if(s[i] ==SENTINAL){
            continue;
        }

        sum += i*s[i];
    }
    return sum;
}

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const auto inp = getInput(argv[1]);
    
    bool isFile = true;
    int id = 0;
    std::vector<int64_t> data;
    for(const auto& ele: inp){
        
        for(int i = 0; i<ele; i++){
            if(isFile){
                data.emplace_back(id);
            }
            else{
                data.emplace_back(SENTINAL);
            }
        }

        id += isFile;
        isFile = !isFile;
        
    }

    {
        auto d_cpy = data;
    
        // std::cout << 0 << std::endl;
        auto fptr = d_cpy.begin();
        auto bptr = d_cpy.end()-1;
        // std::cout << 1 << std::endl;
        while (fptr != bptr)
        {
            while(*fptr != SENTINAL && fptr != bptr){
                // std::cout << 2 <<':' << *fptr << std::endl;
                ++fptr;
            }
            while(*bptr == SENTINAL && fptr != bptr){
                // std::cout << 3 << std::endl;
                --bptr;
            }
            // std::cout << 4 << std::endl;
            std::iter_swap(fptr, bptr);
        }
        
        //gt 91649267962
        //eq 6435922584968
        std::cout << checksum(d_cpy) << std::endl;
    }

    {
        auto d_cpy = data;
        
    }
    return 0;
}