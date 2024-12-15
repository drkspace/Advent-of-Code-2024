

#include <algorithm>
#include <ranges>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "easyTimer.h"
#include <unordered_set>
#include <set>

using u64 = int64_t;

std::vector<std::string> split(const std::string& inp, const char delim) {
    std::string tmp;
    std::vector<std::string> r;
    std::istringstream s(inp);
    while (std::getline(s, tmp, delim)) {
        r.push_back(tmp);
    }
    return r;
}

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

    // template <class T1, class T2>
    // size_t operator>(const std::pair<T1, T2>& p1, const std::pair<T1, T2>& p2) const
    // {
    //     if (p1.first == p2.first) {
    //         return p2.second < p2.first;
    //     }else {
    //         return p2.first < p1.first;
    //     }
    // }
};

enum class Dir {
    UP, DOWN, LEFT, RIGHT
};

using Walls = std::unordered_set<std::pair<u64, u64>, hash_pair>;
using BoxLocs = std::unordered_set<std::pair<u64, u64>, hash_pair>;
using Moves = std::vector<Dir>;
auto getInput(const std::string &fp)
{
    std::ifstream file(fp);
    Walls walls;
    BoxLocs boxLocs;
    std::pair<u64, u64> botpos;
    Moves moves;
    u64 row = 0;
    u64 cols = 0;
    while (file)
    {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            cols = tmp.size();
            for (size_t i = 0; i < tmp.size(); ++i) {
                if (tmp[i] == '#') {
                    walls.emplace(row, i);
                }else if (tmp[i] == 'O') {
                    boxLocs.emplace(row, i);
                }else if (tmp[i] == '@') {
                    botpos = {row, i};
                }
            }
            row++;
        }else {
            break;
        }
    }
    while (file) {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty()) {
            for (const auto& c: tmp) {
                switch (c) {
                    case '^':
                        moves.emplace_back(Dir::UP);
                    break;
                    case '>':
                        moves.emplace_back(Dir::RIGHT);
                    break;
                    case '<':
                        moves.emplace_back(Dir::LEFT);
                    break;
                    case 'v':
                        moves.emplace_back(Dir::DOWN);
                    break;
                }
            }
        }
    }

    return std::make_tuple(walls, boxLocs, botpos, moves, row, cols);
}

auto getInput2(const std::string &fp)
{
    std::ifstream file(fp);
    Walls walls;
    BoxLocs boxLocs_left;
    BoxLocs boxLocs_right;
    std::pair<u64, u64> botpos;
    Moves moves;
    std::vector<std::string> tmpgrid;
    while (file)
    {
        std::string line;
        std::getline(file, line);
        if (!line.empty())
        {
            std::stringstream tmp;
            for (const auto c: line) {
                switch (c) {
                    case '#':
                        tmp << "##";
                        break;
                    case 'O':
                        tmp << "[]";
                        break;
                    case '.':
                        tmp << "..";
                        break;
                    case '@':
                        tmp << "@.";
                        break;
                }
            }
            tmpgrid.emplace_back(tmp.str());
        }else {
            break;
        }
    }

    for (const auto& [i, r]: std::ranges::views::enumerate(tmpgrid)) {
        for (const auto& [j, c]: std::ranges::views::enumerate(r)) {
                if (c == '#') {
                    walls.emplace(i, j);
                }else if (c == '[') {
                    boxLocs_left.emplace(i, j);
                    boxLocs_right.emplace(i, j+1);
                }else if (c == '@') {
                    botpos = {i, j};
                }
        }
    }

    while (file) {
        std::string tmp;
        std::getline(file, tmp);
        if (!tmp.empty()) {
            for (const auto& c: tmp) {
                switch (c) {
                    case '^':
                        moves.emplace_back(Dir::UP);
                    break;
                    case '>':
                        moves.emplace_back(Dir::RIGHT);
                    break;
                    case '<':
                        moves.emplace_back(Dir::LEFT);
                    break;
                    case 'v':
                        moves.emplace_back(Dir::DOWN);
                    break;
                }
            }
        }
    }

    return std::make_tuple(walls, boxLocs_left, boxLocs_right, botpos, moves, tmpgrid.size(), tmpgrid[0].size());
}

u64 nOpen(const Walls& walls, const BoxLocs& box_locs, const std::pair<u64, u64>& pos, Dir d) {
    u64 count = 0;
    u64 i;
    switch (d) {
        case Dir::UP:
            i = pos.first-1;
            while (true) {
                if (!walls.contains(std::make_pair(i, pos.second)) &&
                    !box_locs.contains(std::make_pair(i, pos.second))) {
                    count++;
                }
                else if (walls.contains(std::make_pair(i, pos.second))){
                    break;
                }
                --i;
            }
            break;
        case Dir::DOWN:
            i = pos.first+1;
            while (true) {
                if (!walls.contains(std::make_pair(i, pos.second)) &&
                    !box_locs.contains(std::make_pair(i, pos.second))) {
                    count++;
                    }
                else if (walls.contains(std::make_pair(i, pos.second))){
                    break;
                }
                i++;
            }
            break;
        case Dir::LEFT:
            i = pos.second-1;
            while (true) {
                if (!walls.contains(std::make_pair(pos.first, i)) &&
                    !box_locs.contains(std::make_pair(pos.first, i))) {
                    count++;
                    }
                else if (walls.contains(std::make_pair(pos.first, i))){
                    break;
                }
                --i;
            }
            break;
        case Dir::RIGHT:
            i = pos.second+1;
            while (true) {
                if (!walls.contains(std::make_pair(pos.first, i)) &&
                    !box_locs.contains(std::make_pair(pos.first, i))) {
                    count++;
                    }
                else if (walls.contains(std::make_pair(pos.first, i))){
                    break;
                }
                i++;
            }
            break;

    }
    return count;
}
u64 nOpen2(const Walls& walls, const BoxLocs& box_locs_left, const BoxLocs& box_locs_right, const std::pair<u64, u64>& pos, Dir d) {
    u64 count = 0;
    u64 i;
    switch (d) {
        case Dir::UP:
            i = pos.first-1;
        while (true) {
            if (!walls.contains(std::make_pair(i, pos.second)) &&
                (!box_locs_left.contains(std::make_pair(i, pos.second)) &&
                 !box_locs_right.contains(std::make_pair(i, pos.second))   )) {
                count++;
                }
            else if (walls.contains(std::make_pair(i, pos.second))){
                break;
            }
            --i;
        }
        break;
        case Dir::DOWN:
            i = pos.first+1;
        while (true) {
            if (!walls.contains(std::make_pair(i, pos.second)) &&
            (!box_locs_left.contains(std::make_pair(i, pos.second)) &&
             !box_locs_right.contains(std::make_pair(i, pos.second))   )) {
                count++;
                }
            else if (walls.contains(std::make_pair(i, pos.second))){
                break;
            }
            i++;
        }
        break;
        case Dir::LEFT:
            i = pos.second-1;
        while (true) {
            if (!walls.contains(std::make_pair(pos.first, i)) &&
            (!box_locs_left.contains(std::make_pair(pos.first, i)) &&
             !box_locs_right.contains(std::make_pair(pos.first, i))   )) {
                count++;
                }
            else if (walls.contains(std::make_pair(pos.first, i))){
                break;
            }
            --i;
        }
        break;
        case Dir::RIGHT:
            i = pos.second+1;
        while (true) {
            if (!walls.contains(std::make_pair(pos.first, i)) &&
            (!box_locs_left.contains(std::make_pair(pos.first, i)) &&
         !box_locs_right.contains(std::make_pair(pos.first, i)))) {
                count++;
                }
            else if (walls.contains(std::make_pair(pos.first, i))){
                break;
            }
            i++;
        }
        break;

    }
    return count;
}

void printConfig(const Walls& walls, const BoxLocs& box_locs, const std::pair<u64, u64>& pos, u64 rows, u64 cols) {
    for (u64 i = 0; i < rows; ++i) {
        for (u64 j = 0; j < cols; ++j) {
            if (walls.contains(std::make_pair(i, j))) {
                std::cout << "#";
            } else if (box_locs.contains(std::make_pair(i, j))) {
                std::cout << 'O';
            } else if (pos == std::make_pair(i, j)) {
                std::cout << '@';
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}

void printConfig(const Walls& walls, const BoxLocs& box_locs_left, const BoxLocs& box_locs_right, const std::pair<u64, u64>& pos, u64 rows, u64 cols) {
    for (u64 i = 0; i < rows; ++i) {
        for (u64 j = 0; j < cols; ++j) {
            if (walls.contains(std::make_pair(i, j))) {
                std::cout << "#";
            } else if (box_locs_left.contains(std::make_pair(i, j))) {
                std::cout << '[';
            }else if (box_locs_right.contains(std::make_pair(i, j))) {
                std::cout << ']';
            }
            else if (pos == std::make_pair(i, j)) {
                std::cout << '@';
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}

bool isClip(const Walls& walls, const BoxLocs& box_locs, const std::pair<u64, u64>& pos) {
    if (walls.contains(pos)) {
        return true;
    }
    if (box_locs.contains(pos)) {
        return true;
    }
    for (const auto& p: walls) {
        if (box_locs.contains(p)) {
            return true;
        }
    }
    for (const auto& p: box_locs) {
        if (walls.contains(p)) {
            return true;
        }
    }
    return false;
}

bool canMove(const Walls& walls, const BoxLocs& boxLocs_left, const BoxLocs& boxLocs_right, std::pair<u64, u64> pos, const Dir d, const bool first=false) {
    if (walls.contains(pos)) {
        return false;
    }
    if (!first && !boxLocs_left.contains(pos) && !boxLocs_right.contains(pos)) {
        return true;
    }
    switch (d) {
        case Dir::UP:
            if (walls.contains(std::make_pair(pos.first-1, pos.second))) {
                return false;
            }
            if (boxLocs_right.contains(std::make_pair(pos.first-1, pos.second))) {
                return canMove(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first-1, pos.second), d) && canMove(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first-1, pos.second-1), d);
            } else if (boxLocs_left.contains(std::make_pair(pos.first-1, pos.second))){
                return canMove(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first-1, pos.second), d) && canMove(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first-1, pos.second+1), d);
            } else {
                return true;
            }
            break;
        case Dir::DOWN:
            if (walls.contains(std::make_pair(pos.first+1, pos.second))) {
                return false;
            }
            if (boxLocs_right.contains(std::make_pair(pos.first+1, pos.second))) {
                return canMove(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first+1, pos.second), d) && canMove(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first+1, pos.second-1), d);
            } else if (boxLocs_left.contains(std::make_pair(pos.first+1, pos.second))){
                return canMove(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first+1, pos.second), d) && canMove(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first+1, pos.second+1), d);
            } else {
                return true;
            }
            break;
        case Dir::LEFT:
            if (walls.contains(std::make_pair(pos.first, pos.second-1))) {
                return false;
            }
            return canMove(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first, pos.second-1), d);
            break;
        case Dir::RIGHT:
            if (walls.contains(std::make_pair(pos.first, pos.second+1))) {
                return false;
            }
            return canMove(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first, pos.second+1), d);
            break;
    }
    return false;
}

void stepHelper(const Walls& walls, BoxLocs& boxLocs_left, BoxLocs& boxLocs_right, std::pair<u64, u64> pos, Dir d, BoxLocs& hasmoved, BoxLocs& toMove) {

    decltype(pos) last_pos;
    if (pos.first == 3 && (pos.second == 5 || pos.second == 6)) {
        std::cout << "";
    }
    // if (!boxLocs_left.contains(pos) && !boxLocs_right.contains(pos)) {
    //     return;
    // }

    switch (d) {
        case Dir::UP:
            if (boxLocs_right.contains(std::make_pair(pos.first-1, pos.second))) {
                stepHelper(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first-1, pos.second), d, hasmoved, toMove);
                stepHelper(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first-1, pos.second-1), d, hasmoved, toMove);
            } else if (boxLocs_left.contains(std::make_pair(pos.first-1, pos.second))){
                stepHelper(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first-1, pos.second), d, hasmoved, toMove);
                stepHelper(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first-1, pos.second+1), d, hasmoved, toMove);
            }

            if (auto search = boxLocs_left.find(pos); search != boxLocs_left.end()) {
                toMove.emplace(pos);
            } else if (auto search = boxLocs_right.find(pos); search != boxLocs_right.end()){
                toMove.emplace(std::make_pair(pos.first, pos.second-1));
            }
            break;
        case Dir::DOWN:
            if (boxLocs_right.contains(std::make_pair(pos.first+1, pos.second))) {
                stepHelper(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first+1, pos.second), d, hasmoved, toMove);
                stepHelper(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first+1, pos.second-1), d, hasmoved, toMove);
            } else if (boxLocs_left.contains(std::make_pair(pos.first+1, pos.second))){
                stepHelper(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first+1, pos.second), d, hasmoved, toMove);
                stepHelper(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first+1, pos.second+1), d, hasmoved, toMove);
            }

        if (auto search = boxLocs_left.find(pos); search != boxLocs_left.end()) {
            toMove.emplace(pos);
        } else if (auto search = boxLocs_right.find(pos); search != boxLocs_right.end()){
            toMove.emplace(std::make_pair(pos.first, pos.second-1));
        }
        break;
        case Dir::LEFT:
            last_pos = std::make_pair(pos.first, pos.second-1);
            while (boxLocs_left.contains(last_pos)) {
                toMove.emplace(last_pos);
                last_pos.second-=2;
            }
            break;
        case Dir::RIGHT:
            last_pos = std::make_pair(pos.first, pos.second);
            while (boxLocs_left.contains(last_pos)) {
                toMove.emplace(last_pos);
                last_pos.second+=2;
            }
            break;
    }
}

void step(const Walls& walls, BoxLocs& boxLocs_left, BoxLocs& boxLocs_right, std::pair<u64, u64>& pos, Dir d) {

    printConfig(walls, boxLocs_left, boxLocs_right, pos, 10, 10*2);
    if (!canMove(walls, boxLocs_left, boxLocs_right, pos, d, true)) {
        return;
    }
    BoxLocs hasmoved;
    BoxLocs toMove;
    switch (d) {
        case Dir::UP:
            stepHelper(walls, boxLocs_left, boxLocs_right, pos, d, hasmoved, toMove);
            pos.first--;
            for (const auto& ele: toMove) {
                boxLocs_left.erase(boxLocs_left.find(ele));
                boxLocs_right.erase(boxLocs_right.find(std::make_pair(ele.first, ele.second+1)));
            }
            for (const auto& ele: toMove) {
                boxLocs_left.emplace(ele.first-1, ele.second);
                boxLocs_right.emplace(ele.first-1, ele.second+1);
            }
            break;
        case Dir::DOWN:
            stepHelper(walls, boxLocs_left, boxLocs_right, pos, d, hasmoved, toMove);
            pos.first++;
            for (const auto& ele: toMove) {
                boxLocs_left.erase(boxLocs_left.find(ele));
                boxLocs_right.erase(boxLocs_right.find(std::make_pair(ele.first, ele.second+1)));
            }
            for (const auto& ele: toMove) {
                boxLocs_left.emplace(ele.first+1, ele.second);
                boxLocs_right.emplace(ele.first+1, ele.second+1);
            }
        break;
        case Dir::LEFT:
            stepHelper(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first, pos.second-1), d, hasmoved, toMove);
            pos.second--;
            for (const auto& ele: toMove) {
                boxLocs_left.erase(boxLocs_left.find(ele));
                boxLocs_right.erase(boxLocs_right.find(std::make_pair(ele.first, ele.second+1)));
            }
            for (const auto& ele: toMove) {
                boxLocs_left.emplace(ele.first, ele.second-1);
                boxLocs_right.emplace(ele.first, ele.second);
            }
            break;
        case Dir::RIGHT:
            stepHelper(walls, boxLocs_left, boxLocs_right, std::make_pair(pos.first, pos.second+1), d, hasmoved, toMove);
            pos.second++;
            for (const auto& ele: toMove) {
                boxLocs_left.erase(boxLocs_left.find(ele));
                boxLocs_right.erase(boxLocs_right.find(std::make_pair(ele.first, ele.second+1)));
            }
            for (const auto& ele: toMove) {
                boxLocs_left.emplace(ele.first, ele.second+1);
                boxLocs_right.emplace(ele.first, ele.second+2);
            }
            break;
    }
    std::cout << "";
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> t("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    {
        auto [walls, boxLocs, botpos, moves, rows, cols] = getInput(argv[1]);

        decltype(botpos) last_box;
        for (const auto& move: moves) {

            if (nOpen(walls, boxLocs, botpos, move) == 0) {
                continue;
            }
            switch (move) {
                case Dir::UP:
                    botpos.first--;
                if (walls.contains(botpos)) {
                    botpos.first++;
                    break;
                }
                last_box = botpos;
                while (true) {

                    if (auto search = boxLocs.find(last_box); search != boxLocs.end()) {
                        last_box.first--;
                    }else {
                        break;
                    }
                }
                if (last_box.first != botpos.first) {
                    auto search = boxLocs.find(botpos);
                    boxLocs.erase(search);
                    boxLocs.emplace(last_box);
                }
                break;
                case Dir::DOWN:
                    botpos.first++;
                if (walls.contains(botpos)) {
                    botpos.first--;
                    break;
                }
                last_box = botpos;
                while (true) {

                    if (auto search = boxLocs.find(last_box); search != boxLocs.end()) {
                        last_box.first++;
                    }else {
                        break;
                    }
                }
                if (last_box.first != botpos.first) {
                    auto search = boxLocs.find(botpos);
                    boxLocs.erase(search);
                    boxLocs.emplace(last_box);
                }
                break;
                case Dir::LEFT:
                    botpos.second--;
                if (walls.contains(botpos)) {
                    botpos.second++;
                    break;
                }
                last_box = botpos;
                while (true) {

                    if (auto search = boxLocs.find(last_box); search != boxLocs.end()) {
                        last_box.second--;
                    }else {
                        break;
                    }
                }
                if (last_box.second != botpos.second) {
                    auto search = boxLocs.find(botpos);
                    boxLocs.erase(search);
                    boxLocs.emplace(last_box);
                }
                break;
                case Dir::RIGHT:
                    botpos.second++;
                if (walls.contains(botpos)) {
                    botpos.second--;
                    break;
                }
                last_box = botpos;
                while (true) {

                    if (auto search = boxLocs.find(last_box); search != boxLocs.end()) {
                        last_box.second++;
                    }else {
                        break;
                    }
                }
                if (last_box.second != botpos.second) {
                    auto search = boxLocs.find(botpos);
                    boxLocs.erase(search);
                    boxLocs.emplace(last_box);
                }
                break;

            }
        }
        printConfig(walls, boxLocs, botpos, rows, cols);
        // std::cout << botpos.first << ',' << botpos.second << std::endl;
        u64 sum = 0;
        for (const auto& b: boxLocs) {
            sum += b.first*100 + b.second;
        }
        std::cout << sum << std::endl;
    }

    {
        auto [walls, boxLocs_left, boxLocs_right, botpos, moves, rows, cols] = getInput2(argv[1]);
        decltype(botpos) last_box;
        for (const auto& move: moves) {
            // std::cout << static_cast<int>(move) <<std::endl;
            step(walls, boxLocs_left, boxLocs_right, botpos, move);
        }
        printConfig(walls, boxLocs_left, boxLocs_right, botpos, 10, 10*2);
        // std::cout << botpos.first << ',' << botpos.second << std::endl;
        std::cout << boxLocs_left.size() << std::endl;
        u64 sum = 0;
        for (const auto& b: boxLocs_left) {
            sum += b.first*100 + b.second;
        }
        std::cout << sum << std::endl;
    }
}