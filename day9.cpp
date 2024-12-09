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
        if(s[i] == SENTINAL){
            continue;
        }
        // std::cout << i << '*' << s[i]<<std::endl;
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

        auto start = d_cpy.begin();
        // for(const auto& ele : d_cpy){
        //         if(ele == SENTINAL){
        //             std::cout << '.';
        //         }else{
        //             std::cout <<'[' << ele << ']';
        //         }
        //     }
        //     std::cout << std::endl;
        //     std::cout << std::endl;
        auto end = d_cpy.end()-1;
        std::unordered_set<u_int64_t> moved;
        while(start < end){

            while(*start != SENTINAL){
                start++;
            }   

            auto start_cpy = start;

            // end is at the end of the next block
            while(*end == SENTINAL){
                end--;
            }
            auto last_start = end;
            while(*end == *last_start){
                last_start--;
            }
            last_start++;
            if(start_cpy >= last_start){
                break;
            }
            // std::cout << *last_start << std::endl;
            if(!moved.contains(*last_start)){
                moved.emplace(*last_start);
                // std::cout << *last_start << std::endl;
                const auto len = std::distance(last_start, end)+1;

                for(auto it = start_cpy; it<last_start;){
                    
                    if(*it != SENTINAL){
                        ++it;
                        continue;
                    }
                    
                    auto blk_end = it;
                    while (*blk_end == SENTINAL)
                    {
                        blk_end++;
                    }
                    // std::cout << std::distance(d_cpy.begin(), it) << '-' <<std::distance(d_cpy.begin(), last_start) << *it << ',' << *last_start << std::endl;
                    if(std::distance(it, blk_end) >= len){
                        std::swap_ranges(it, it+len, last_start);
                        break;
                    }

                    it = blk_end+1;
                    
                }
            }
            end = last_start-1;

        }

        // for(const auto& ele : d_cpy){
        //         if(ele == SENTINAL){
        //             std::cout << '.';
        //         }else{
        //             std::cout <<'[' << ele << ']';
        //         }
        //     }
        //     std::cout << std::endl;

        //lt 6469637179774
        //gt 6450047797159
        auto tmp = checksum(d_cpy);
        std::cout << tmp << std::endl;
    }
    return 0;
}