#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "easyTimer.h"
#include <unordered_set>
#include <unordered_map>
#include <cinttypes>
#include "utils/utils.h"
#include <optional>
#include <pstl/execution_defs.h>
using dt = intmax_t;

constexpr dt INF = std::numeric_limits<dt>::max()/2;
constexpr char U='^';
constexpr char D='v';
constexpr char L='<';
constexpr char R='>';
constexpr char A='A';

std::vector<std::string> getInput(const std::string &fp){
    std::ifstream file(fp);
    std::vector<std::string> tmp_m;
    while (file)
    {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            tmp_m.emplace_back(tmp);
        }
    }

    return tmp_m;
}

char itoc(const int& i) {
    switch (i) {
        case 0:
            return '0';
            break;
        case 1:
            return '1';
        break;
        case 2:
            return '2';
        break;
        case 3:
            return '3';
        break;
        case 4:
            return '4';
        break;
        case 5:
            return '5';
        break;
        case 6:
            return '6';
        break;
        case 7:
            return '7';
        break;
        case 8:
            return '8';
        break;
        case 9:
            return '9';
        break;
        default:
            return 'P';
    }
}

void simkp(const std::string& seq) {
    int py=3;
    int px = 2;

    for (const auto& c: seq) {
        switch (c) {
            case '<':
                px--;
            break;
            case '>':
                px++;
            break;
            case '^':
                py--;
            break;
            case 'v':
                py++;
            break;
            case 'A':
                if (py <3) {
                    std::cout << 7-(3*py) + px;
                }
                else {
                    if (px == 1) {
                        std::cout << 0;
                    }
                    else if (px == 2) {
                        std::cout << 'A';
                    }
                    else {
                        std::cout << "E";
                    }
            }
            break;
            default:
                std::cout <<'p';
                break;
        }

        if (px < 0 || px >2|| py < 0 || py>4) {
            std::cout << "E";
        }
    }
    std::cout << std::endl;
}

void simdir(const std::string& seq) {
    int px = 2;
    int py = 0;
    int i =0;
    for (const auto& c: seq) {
        switch (c) {
            case '<':
                px--;
            break;
            case '>':
                px++;
            break;
            case '^':
                py--;
            break;
            case 'v':
                py++;
            break;
            case 'A':
                if (px==0 && py ==0) {
                    std::cout << "E" << std::flush;
                }
                if (px==1 && py ==0) {
                    std::cout << "^" << std::flush;
                }
                if (px==2 && py ==0) {
                    std::cout << "A" << std::flush;
                }
                if (px==0 && py ==1) {
                    std::cout << "<" << std::flush;
                }
                if (px==1 && py ==1) {
                    std::cout << "v" << std::flush;
                }
                if (px==2 && py ==1) {
                    std::cout << ">" << std::flush;
                }
            break;
            default:
                std::cout <<'p';
            break;
        }

        if (px < 0 || px >2|| py < 0 || py>1) {
            std::cout << "E";
        }
        i++;
    }
    std::cout << std::endl;
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> _("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    std::unordered_map<std::pair<char, char>, std::string, hash_tuple> DIR_PATHS;
    DIR_PATHS[{A, U}] = "<";
    DIR_PATHS[{U, A}] = ">";
    DIR_PATHS[{A, R}] = "v";
    DIR_PATHS[{R, A}] = "^";
    DIR_PATHS[{A, D}] = "<v";
    DIR_PATHS[{D, A}] = "^>";
    DIR_PATHS[{A, L}] = "v<<";
    DIR_PATHS[{L, A}] = "^>>";

    DIR_PATHS[{U, D}] = "v";
    DIR_PATHS[{D, U}] = "^";
    DIR_PATHS[{U, L}] = "<v";
    DIR_PATHS[{L, U}] = "^>";
    DIR_PATHS[{U, R}] = "v>";
    DIR_PATHS[{R, U}] = "<^";

    DIR_PATHS[{D, L}] = "<";
    DIR_PATHS[{L, D}] = ">";
    DIR_PATHS[{D, R}] = ">";
    DIR_PATHS[{R, D}] = "<";

    DIR_PATHS[{L, R}] = ">>";
    DIR_PATHS[{R, L}] = "<<";

    DIR_PATHS[{A,A}] = "";
    DIR_PATHS[{U,U}] = "";
    DIR_PATHS[{D,D}] = "";
    DIR_PATHS[{R,R}] = "";
    DIR_PATHS[{L,L}] = "";



    std::unordered_map<std::pair<char, char>, std::string, hash_tuple> KEYPAD_PATHS;
    for (int i = 1; i<=9;i++) {
        const auto r1 = (i-1)/3;
        const auto c1 = (i-1)%3;
        for (int j = 1; j<=9;j++) {
            const auto r2 = (j-1)/3;
            const auto c2 = (j-1)%3;
            std::string tmp;


            if (c1 > c2) {
                for (int k = 0; k<(c1-c2); k++) {
                    tmp += "<";
                }
            }
            if (r1 > r2) {
                for (int k = 0; k<(r1-r2); k++) {
                    tmp += "v";
                }
            }

            if (c1 < c2) {
                for (int k = 0; k<(c2-c1); k++) {
                    tmp += ">";
                }
            }

            if (r1 < r2) {
                for (int k = 0; k<(r2-r1); k++) {
                    tmp += "^";
                }
            }

            KEYPAD_PATHS[{itoc(i),itoc(j)}] = std::move(tmp);
        }
        {
            std::string tmp1;
            std::string tmp2;
            if (c1 == 0) {
                for (int k = 0; k<(r1+1); k++) {
                    tmp2 += "^";
                }
                tmp1 += ">";
                tmp2 += "<";
                for (int k = 0; k<(r1+1); k++) {
                    tmp1 += "v";
                }
            } else if (c1 == 2) {

                tmp1 += "<";
                tmp2 += ">";
                for (int k = 0; k<(r1+1); k++) {
                    tmp1 += "v";
                    tmp2 += "^";
                }
            }else {
                for (int k = 0; k<(r1+1); k++) {
                    tmp1 += "v";
                    tmp2 += "^";
                }
            }

            KEYPAD_PATHS[{itoc(i), '0'}] = std::move(tmp1);
            KEYPAD_PATHS[{'0', itoc(i)}] = std::move(tmp2);
        }


        {
            std::string tmp1;
            std::string tmp2;
            // for (int k = 0; k<(r1+1); k++) {
            //     tmp1 += "v";
            //     tmp2 += "^";
            // }
            if (c1 == 0) {
                for (int k = 0; k<(r1+1); k++) {
                    tmp2 += "^";
                }

                tmp1 += ">>";
                tmp2 += "<<";
                for (int k = 0; k<(r1+1); k++) {

                    tmp1 += "v";
                }
            } else if (c1 == 1) {
                for (int k = 0; k<(r1+1); k++) {
                    tmp1 += "v";
                }
                tmp1 += ">";
                tmp2 += "<";
                for (int k = 0; k<(r1+1); k++) {

                    tmp2 += "^";
                }
            }
            else {
                for (int k = 0; k<(r1+1); k++) {
                        tmp1 += "v";
                        tmp2 += "^";
                }
            }

            KEYPAD_PATHS[{itoc(i), A}] = std::move(tmp1);
            KEYPAD_PATHS[{A, itoc(i)}] = std::move(tmp2);
        }
    }

    KEYPAD_PATHS[{A, '0'}] = "<";
    KEYPAD_PATHS[{'0', A}] = ">";

    std::unordered_map<char, int> t = {{'<', 3}, {'v', 2}, {'>',1}, {'^', 1}};

    for (const auto&[k, s]: KEYPAD_PATHS) {
        std::cout << k << s << std::endl;
        simkp(KEYPAD_PATHS[{A, k.first}]+"A"+s+"A");
        if (s.size()>1) {
            for (size_t i = 0; i<(s.size()-1); i++) {
                if (t[s[i]] < t[s[i+1]]) {
                    std::cout << "invalid" << std::endl;
                }
            }
        }
    }
    std::cout << "==============" << std::endl;
    for (const auto&[k, s]: DIR_PATHS) {
        std::cout << k << s << std::endl;
        simdir(DIR_PATHS[{A, k.first}]+"A"+s+"A");
        if (s.size()>1) {
            for (size_t i = 0; i<(s.size()-1); i++) {
                if (t[s[i]] < t[s[i+1]]) {
                    std::cout << "invalid" << std::endl;
                }
            }
        }
    }

    const auto inp = getInput(argv[1]);
    dt sum = 0;
    for (const auto& kp: inp) {
        // break;
        std::cout << kp  << std::endl;
        dt n = std::stoi(kp);
        char curpos = A;
        std::string seq, seq2, seq3;
        for (const auto& c: kp) {
            if (!KEYPAD_PATHS.contains({curpos, c})) {
                std::cout << "kp"<<curpos<<c<<std::endl;
            }
            seq += KEYPAD_PATHS[{curpos, c}] + 'A';
            curpos = c;
        }
        simkp(seq);
        std::cout << seq << std::endl;
        // simkp(seq);
        // std::cout << "<A^A>^^AvvvA\n" << std::endl;

        curpos = A;
        for (const auto& c: seq) {
            if (!DIR_PATHS.contains({curpos, c})) {
                std::cout << "dp1"<<curpos<<c<<std::endl;
            }

            // if (tmp.size()==2) {
            //     if (seq2[seq2.size()-2] == tmp[1][0]) {
            //         pick = tmp[1];
            //     }
            // }
            seq2 += DIR_PATHS[{curpos, c}] + 'A';
            curpos = c;
        }
        simdir(seq2);
        std::cout << seq2 << std::endl;

        // std::cout << "v<<A>>^A<A>AvA<^AA>A<vAAA>^A\n" << std::endl;

        curpos = A;
        for (const auto& c: seq2) {
            if (!DIR_PATHS.contains({curpos, c})) {
                std::cout << "dp2"<<curpos<<c<<std::endl;
            }
            seq3 += DIR_PATHS[{curpos, c}] + 'A';
            curpos = c;
        }
        simdir(seq3);
        std::cout << seq3 << std::endl;

        // std::cout << "<vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A\n"  << std::endl;
        std::cout << seq3.size() << '*' << n << '=' << seq3.size()*n << std::endl;
        sum += static_cast<dt>(seq3.size())*n;
    }
    // lt 289060
    // gt 270372
    // neq 275428
    // neq 282520
    // neq 276912
    // new 285840
    std::cout << sum << std::endl;
    std::cout << (sum < 289060) << std::endl;
    std::cout << (sum > 270372) << std::endl;
    std::cout << (sum != 275428) << std::endl;
    std::cout << (sum != 282520) << std::endl;
    std::cout << (sum != 276912) << std::endl;
    std::cout << (sum != 285840) << std::endl;
}

