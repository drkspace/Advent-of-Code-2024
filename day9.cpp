#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include "easyTimer.h"

constexpr int64_t SENTINAL = -1;

struct locs{
    uint64_t value;
    size_t idx;
    size_t count;
};


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
        if(s[i] == SENTINAL){
            continue;
        }
        // std::cout << i << '*' << s[i]<<std::endl;
        sum += i*s[i];
    }
    return sum;
}

u_int64_t checksum(const std::vector<locs>& s){
    u_int64_t sum = 0;
    for(const auto& ele: s){
        for (size_t i = ele.idx; i<(ele.idx+ele.count); ++i) {
            sum += i*ele.value;
        }
    }
    return sum;
}


int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> t("main ");

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


        std::map<size_t, std::set<size_t>> open_spaces;
        std::vector<locs> locations(1000);
        isFile = true;
        id = 0;
        size_t idx = 0;
        {

            for(const auto& ele: inp){
                if (isFile) {
                    locations.emplace_back(id, idx, ele);
                }else {
                    if (ele != 0) {
                        open_spaces[ele].emplace(idx);
                    }
                }
                idx += ele;

                id += isFile;
                isFile = !isFile;
            }
        }

        for (auto& l : std::ranges::reverse_view(locations)) {

            const auto lb = open_spaces.lower_bound(l.count);
            size_t min = l.idx;
            size_t min_key;
            for (auto it = lb; it != open_spaces.end(); ++it) {
                if (*(it->second.begin()) < min) {
                    min = *(it->second.begin());
                    min_key = it->first;
                }
            }
            if (l.idx != min) {
                l.idx = min;
                open_spaces[min_key].erase(open_spaces[min_key].begin());
                if (open_spaces[min_key].empty()) {
                    open_spaces.erase(min_key);
                }
                if (l.count < min_key) {
                    open_spaces[min_key-l.count].emplace(l.idx+l.count);
                }
            }

        }

        std::cout << checksum(locations) << std::endl;

    return 0;
}