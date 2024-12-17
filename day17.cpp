#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "easyTimer.h"
#include <unordered_set>
#include <unordered_map>
#include <thread>

using u64 = uint64_t;
const auto& enumerate = std::ranges::views::enumerate;

constexpr u64 INF = std::numeric_limits<u64>::max()/2;

u64 exp2(u64 exp){
    if(exp == 0){
        return 1;
    }
    u64 res = 1;
    for(u64 i = 0; i<exp; i++){
        res*=2;
    }
    return res;
}

struct Program{
    u64 reg_a = 0;
    u64 reg_b = 0;
    u64 reg_c = 0;
    std::vector<uint16_t> tape;

    bool run(size_t& ip, std::vector<uint16_t>& other_tape){
        if((ip+1) >= tape.size()){
            return false;
        }
        auto opcode = tape[ip];
        auto operand = tape[ip+1];
        auto literal = operand;
        switch (operand)
        {
        case 4:
            operand = reg_a;
            break;
        case 5:
            operand = reg_b;
            break;
        case 6:
            operand = reg_c;
            break;
        
        default:
            break;
        }
        // std::cout << "op" << operand << std::endl;
        
        switch (opcode)
        {
        case 0:
            
            reg_a = reg_a >> operand;
            
            break;
        case 1:
            reg_b = reg_b ^ literal;
            break;
        case 2:
            reg_b = operand % 8;
            break;
        case 3:
            if(reg_a != 0){
                ip = literal-2;
            }
            break;
        case 4:
            reg_b = reg_b ^ reg_c;
            break;
        case 5:
            other_tape.emplace_back(operand%8);
            break;
        case 6:
            reg_b = reg_a >> operand;
            break;
        case 7:
            reg_c = reg_a >> operand;
            break;
        default:
            std::cout << "weewoo" << std::endl;
            return false;
            break;
        }

        
        ip+=2;
        
        return true;
    }

    bool operator==(const Program& other) const{
        return this->reg_a == other.reg_a &&
                this->reg_b == other.reg_b &&
                this->reg_c == other.reg_c &&
                std::ranges::equal(this->tape, other.tape);
    }
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

Program getInput(const std::string &fp){
    std::ifstream file(fp);

    std::string tmpa;
    std::getline(file, tmpa);
    std::string tmpb;
    std::getline(file, tmpb);
    std::string tmpc;
    std::getline(file, tmpc);

    auto a = std::stoul(split(tmpa, ':')[1]);
    auto b = std::stoul(split(tmpb, ':')[1]);
    auto c = std::stoul(split(tmpc, ':')[1]);

    std::getline(file, tmpa);
    std::getline(file, tmpa);
    auto tmp = split(split(tmpa, ':')[1], ',');
    std::vector<uint16_t> tape;
    for(const auto&ele: tmp){
        tape.emplace_back(std::stoi(ele));
    }
        
    
    
    return {a, b, c, tape};
};

struct hash_pair {
    // copied from https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c/
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        // Hash the first element
        size_t hash1 = std::hash<T1>{}(p.first);
        // Hash the second element
        size_t hash2 = std::hash<T2>{}(p.second);
        // Combine the two hash values
        return hash1
               ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
                  + (hash1 >> 2));
    }

    template <class T1, class T2, class T3>
    size_t operator()(const std::tuple<T1, T2, T3>& p) const
    {
        return hash_pair()(std::make_pair(hash_pair()(std::make_pair(std::get<0>(p), std::get<1>(p))), std::get<2>(p)));
    }
};

template <typename T>
bool veq(std::vector<T> v1, std::vector<T> v2){
    if(v1.size() != v2.size()){
        return false;
    }
    for(size_t i = 0; i< v1.size(); i++){
        if(v1[i] != v2[i]){
            return false;
        }
    }
    return true;
}

template <typename T>
bool par_veq(std::vector<T> v1, std::vector<T> v2){
    if(v1.size() > v2.size()){
        return false;
    }
    for(size_t i = 0; i< v1.size(); i++){
        if(v1[i] != v2[i]){
            return false;
        }
    }
    return true;
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
        auto inp_cpy = inp;
        size_t ip = 0;
        bool res = true;
        std::vector<uint16_t> out;
        while (res)
        {
            // std::cout << inp.reg_a << std::endl;
            res = inp_cpy.run(ip, out);
            // std::cout << inp.reg_a << ',' << inp.reg_b << ',' << inp.reg_c << std::endl;
        }
        for(const auto&ele: out){
            std::cout << ele << ',';
        }
        std::cout << std::endl;
    }
    
    // constexpr int N = 12;
    std::mutex m, m2;
    size_t maxlen = 0;
    // auto f = [&](u64 start){
    {

        auto inp_cpy = inp;
        const auto original = inp_cpy.tape;
        std::vector<uint16_t> out;

        u64 i = 21614684;
        i =0;
        u64 a = 0;
        do {
            inp_cpy = inp;
            a = i*(8*8*8*8*8*8*8*8)+7355407;
            inp_cpy.reg_a = a;
            size_t ip = 0;
            bool res = true;
            out.clear();

            while (res && par_veq(out, original))
            {
                res = inp_cpy.run(ip, out);
                if(out.size() > maxlen){
                
                    maxlen = out.size();
                   
                    std::cout << "|"<<maxlen <<',' <<a%8 << ','<<a%64<<','<<a%(64*8)<<','<<a%(64*8*8)<<','<<a%(64*8*8*8)<<','<<a%(64*8*8*8*8)<<','<<a%(64*8*8*8*8*8)<<','<<a%(64*8*8*8*8*8*8) <<','<<a%(64*8*8*8*8*8*8)<< "," << a << ":";
                    for(const auto&ele: out){
                        std::cout << ele << ',';
                    }
                    std::cout << std::endl;
                    
                
                }
            }
            
            i++;
            // a+=N*8;
            // for(const auto&ele: out){
            // std::cout << ele << ',';
            // }
            // std::cout << std::endl;
            // for(const auto&ele: original){
            //     std::cout << ele << ',';
            // }
            // std::cout << std::endl;
            // break;
            // if(a%2001111 == 0){
            //     {
            //         std::lock_guard<std::mutex> _(m2);
            //         std::cout << a << std::endl;
            //     }
            // }
        } while (!veq(original, out));
        
        if(veq(original, out)){
            std::cout << "\nSolution: " << a << std::endl;
        }
    };
    // std::vector<std::thread> threads;
    // for(int i = 0; i<N; i++){
    //     threads.emplace_back(f, 7+i*8);
    // }
    // for(int i = 0; i<N; i++){
    //     threads[i].join();
    // }
    return 0;
}