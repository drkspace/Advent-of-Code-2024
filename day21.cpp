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

using dt = uintmax_t;

constexpr dt INF = std::numeric_limits<dt>::max();
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

dt dochunk(const char c_start, const int level, const std::unordered_map<std::pair<char, char>, std::vector<std::string>, hash_tuple>& DIR_PATHS, std::unordered_map<std::tuple<char, char, int>, dt, hash_tuple>& cache, char start = A) {
    if (cache.contains({c_start,start, level})) {
        return cache[{c_start, start, level}];
    }
    // std::cout << seq << std::endl;
    dt sum = 0;
    // auto best = INF;
    char curpos = start;

    std::string seq2 = DIR_PATHS.find({curpos, c_start})->second[0]+'A';

    if (level==25) {
        return static_cast<dt>(seq2.size());
    }
    curpos = A;
    for (const auto& c: seq2) {
        sum += dochunk(c, level+1, DIR_PATHS, cache, curpos);
        curpos = c;
    }

    cache[{c_start, start, level}] = sum;
    return sum;
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> _("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    std::unordered_map<std::pair<char, char>, std::vector<std::string>, hash_tuple> DIR_PATHS;
    DIR_PATHS[{A, U}] = {"<"};
    DIR_PATHS[{U, A}] = {">"};
    DIR_PATHS[{A, R}] = {"v"};
    DIR_PATHS[{R, A}] = {"^"};
    DIR_PATHS[{A, D}] = {"<v"};
    DIR_PATHS[{D, A}] = {"^>"};
    DIR_PATHS[{A, L}] = {"v<<"};
    DIR_PATHS[{L, A}] = {">>^"};

    DIR_PATHS[{U, D}] = {"v"};
    DIR_PATHS[{D, U}] = {"^"};
    DIR_PATHS[{U, L}] = {"v<"};
    DIR_PATHS[{L, U}] = {">^"};
    DIR_PATHS[{U, R}] = {"v>"};
    DIR_PATHS[{R, U}] = {"<^"};

    DIR_PATHS[{D, L}] = {"<"};
    DIR_PATHS[{L, D}] = {">"};
    DIR_PATHS[{D, R}] = {">"};
    DIR_PATHS[{R, D}] = {"<"};

    DIR_PATHS[{L, R}] = {">>"};
    DIR_PATHS[{R, L}] = {"<<"};

    DIR_PATHS[{A,A}] = {""};
    DIR_PATHS[{U,U}] = {""};
    DIR_PATHS[{D,D}] = {""};
    DIR_PATHS[{R,R}] = {""};
    DIR_PATHS[{L,L}] = {""};



    std::unordered_map<std::pair<char, char>, std::string, hash_tuple> KEYPAD_PATHS1;
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

            if (r1 < r2) {
                for (int k = 0; k<(r2-r1); k++) {
                    tmp += "^";
                }
            }

            if (c1 < c2) {
                for (int k = 0; k<(c2-c1); k++) {
                    tmp += ">";
                }
            }



            KEYPAD_PATHS1[{itoc(i),itoc(j)}] = std::move(tmp);
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

            KEYPAD_PATHS1[{itoc(i), '0'}] = std::move(tmp1);
            KEYPAD_PATHS1[{'0', itoc(i)}] = std::move(tmp2);
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

            KEYPAD_PATHS1[{itoc(i), A}] = std::move(tmp1);
            KEYPAD_PATHS1[{A, itoc(i)}] = std::move(tmp2);
        }
    }

    KEYPAD_PATHS1[{A, '0'}] = "<";
    KEYPAD_PATHS1[{'0', A}] = ">";


    std::unordered_map<std::tuple<char, char, int>, dt, hash_tuple> cache;


    const auto inp = getInput(argv[1]);
    dt sum = 0;
    for (const auto& kp: inp) {

        std::cout << kp  << std::endl;
        dt n = std::stoi(kp);

        char curpos = A;
        std::string seq;
        dt tmp = 0;
        std::string seq1;
        for (const auto& c: kp) {
            seq1 += KEYPAD_PATHS1[{curpos, c}] + 'A';
            curpos = c;
        }

        curpos = A;
        for (const auto& c1: seq1) {
            tmp += dochunk(c1, 1, DIR_PATHS, cache, curpos);
            curpos = c1;
        }


        std::cout << tmp << '*' << n << '=' << tmp*n << std::endl;
        sum += tmp*n;
    }
    // gt 291239350580826
    // lt 719206954115056
    // gt 337244995283638
    // neq 838939274742794
    // neq 384041985863254
    // neq 368151407930732
    std::cout << sum << std::endl;
    std::cout << (sum > 291239350580826) << std::endl;
    std::cout << (sum > 337244995283638) << std::endl;
    std::cout << (sum < 719206954115056) << std::endl;
    std::cout << (sum != 838939274742794) << std::endl;
    std::cout << (sum != 384041985863254) << std::endl;
    std::cout << (sum != 368151407930732) << std::endl;
}

