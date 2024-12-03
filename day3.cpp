
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

int main(const int argc, char *argv[]){

    if (argc != 2) {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const std::string inp = getInput(argv[1]);

    std::smatch ip_result;

    const std::regex r("(mul\\(([0-9]{1,3}),([0-9]{1,3})\\))");

    {
        u_int64_t sum = 0;
        std::string cpy(inp);

        while (std::regex_search(cpy, ip_result, r)){

            sum += std::stoi(ip_result[2]) * std::stoi(ip_result[3]);

            cpy = ip_result.suffix();
        }
        std::cout << "Part 1 Solution: " << sum << std::endl;
    }

    {
        u_int64_t sum = 0;
        std::string cpy(inp);
        std::replace(cpy.begin(), cpy.end(), '\n', '|');

        // Remove parts of the string where the mult would be disabled
        const std::regex r2(R"(don't\(\).*?($|do\(\)))");

        std::string out = cpy;
        while (std::regex_search(out, r2)) {
            out = std::regex_replace(out, r2, "");
        }
        while (std::regex_search(out, ip_result, r)){ //loop
            sum += std::stoi(ip_result[2]) * std::stoi(ip_result[3]);
            out = ip_result.suffix();
        }
        std::cout << "Part 2 solution: " << sum << std::endl;
    }


}