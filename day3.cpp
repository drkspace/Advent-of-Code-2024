
#include <regex>
#include <iostream>
int main(){
    std::regex r("(mul\\(([0-9]{1,3}),([0-9]{1,3})\\))");

    std::string tmp("xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))");

    std::smatch ip_result;

    u_int64_t sum = 0;

    while (std::regex_search(tmp, ip_result, r)){ //loop

        sum += std::stoi(ip_result[2]) * std::stoi(ip_result[3]);
        // std::cout <<  ip_result[2] << ',' << ip_result[3] << std::endl;

        tmp = ip_result.suffix(); //remove "192", then "168" ...
    }
    std::cout << sum << std::endl;
}