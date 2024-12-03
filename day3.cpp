
#include <regex>
#include <iostream>
#include <fstream>

std::string getInput(const std::string& fp){
    std::ifstream file (fp);
    std::stringstream s;
    s << file.rdbuf();
    s.seekg(0);
    return s.str();
}

int main(){
    

    std::string inp = getInput("/workspaces/avoc2024/inputs/input3");

    std::smatch ip_result;

    std::regex r("(mul\\(([0-9]{1,3}),([0-9]{1,3})\\))");
    
    {
        u_int64_t sum = 0;
        std::string cpy(inp);
        
        while (std::regex_search(cpy, ip_result, r)){ //loop

            sum += std::stoi(ip_result[2]) * std::stoi(ip_result[3]);
            // std::cout <<  ip_result[0] << std::endl;

            cpy = ip_result.suffix();
        }
        std::cout << sum << std::endl;
    }
    

    {
        u_int64_t sum = 0;
        std::string cpy(inp);
        std::replace(cpy.begin(), cpy.end(), '\n', '|');

        std::regex r2("don't\\(\\).*?($|do\\(\\))");

        std::string out = cpy;
        while (std::regex_search(out, r2)) {
            out = std::regex_replace(out, r2, "");
        }
        // std::cout << out << std::endl;
        while (std::regex_search(out, ip_result, r)){ //loop

            sum += std::stoi(ip_result[2]) * std::stoi(ip_result[3]);
            // std::cout <<  ip_result[0] << std::endl;

            out = ip_result.suffix();
        }
        // gt 25059239
        // gt 83524390
        // lt 173558055
        std::cout << sum << std::endl;
    }

    
}