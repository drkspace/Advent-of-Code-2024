
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <limits>

using row = std::vector<int>;

struct Inp 
{
    std::vector<std::pair<size_t, size_t>> comp;
    std::vector<row> order;
};

row split(const std::string& inp, const char delim) {
    std::string tmp;
    row r;
    std::istringstream s(inp);
    while (std::getline(s, tmp, delim)) {
        r.push_back(std::stoi(tmp));
    }
    return r;
}

Inp getInput(const std::string& fp){
    std::ifstream file (fp);
    
    Inp inp;

    int mode = 0;
    size_t bi;
    while(file){
     std::string tmp;
     std::getline(file, tmp);
        if (tmp.empty()) {
            mode++;
            continue;
        }
        switch (mode)
        {
        case 0:
            bi = tmp.find_first_of('|');
            
            inp.comp.emplace_back(std::stoi(tmp.substr(0, bi)), std::stoi(tmp.substr(bi+1)));
            break;
        case 1:
            inp.order.emplace_back(split(tmp, ','));
            break;
        default:
            break;
        }

      }
      return inp;
}

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const auto inp = getInput(argv[1]);

    // for(const auto& [f,l]: inp.comp){
    //     std::cout << f << '|' << l << std::endl;
    // }

    // for(const auto& r: inp.order){
    //     for(const auto& ele: r){
    //         std::cout << ele << ',';
    //     }
    //     std::cout << std::endl;
    // }

    
    int sum1 = 0;
    int sum2 = 0;
    for(auto r: inp.order){
        std::unordered_map<int, int> poses;
        for(size_t i = 0; i < r.size(); i++){
            poses[r[i]] = i;
        }
        const auto middle = r[r.size()/2];

        bool good = true;
        for(const auto& [f,l]: inp.comp){
            if((poses.contains(f) && poses.contains(l)) && poses[f] > poses[l]){
                // std::cout << f << '|' << l << std::endl;
                good = false;
                break;
            }
        }
        if(good){
            sum1 += middle;
        }
        else{
            good = true;
            do{
                good=true;
                for(const auto& [f,l]: inp.comp){
                    if((poses.contains(f) && poses.contains(l)) && poses[f] > poses[l]){
                        // std::cout << f << '|' << l << std::endl;

                        r.erase(r.begin() + poses[l]);
                        r.insert(r.begin() + poses[f], static_cast<int>(l));
                        poses.clear();
                        for(size_t i = 0; i < r.size(); i++){
                            poses[r[i]] = i;
                        }
                        good = false;
                        break;
                    }
                }
            }while(!good);
            // for(const auto& ele: r){
            //     std::cout << ele << ',';
            // }
            // std::cout << std::endl;
            // std::cout << r[r.size()/2] << std::endl;
            sum2 += r[r.size()/2];
        }
        
    }
    // std::cout << sum1 << std::endl;
    std::cout << sum2 << std::endl;

    return 0;
}