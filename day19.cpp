
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
using dt = intmax_t;


std::tuple<std::unordered_set<std::string>, std::vector<std::string>> getInput(const std::string &fp){
    std::ifstream file(fp);
    std::unordered_set<std::string> avail;
    std::vector<std::string> wanted;
    std::string tmp;
    std::getline(file, tmp);
    auto s = split(tmp, ',');
    for (auto& w: s)
    {
        ltrim(w);
        avail.emplace(w);
    }
    avail.insert(s.begin(), s.end());
    while (file)
    {
        std::getline(file, tmp);
        if (!tmp.empty())
        {
            ltrim(tmp);
            wanted.emplace_back(tmp);
        }
    }

    return {avail, wanted};
}

enum class COLORS:size_t
{
    W, U, B, R, G
};

inline size_t ctoint(const char& c)
{
    switch (c)
    {
    case 'w':
        return static_cast<size_t>(COLORS::W);
        break;

    case 'u':
        return static_cast<size_t>(COLORS::U);
        break;

    case 'b':
        return static_cast<size_t>(COLORS::B);
        break;

    case 'r':
        return static_cast<size_t>(COLORS::R);
        break;

    case 'g':
        return static_cast<size_t>(COLORS::G);
        break;
    default:
        return 0;
    }
    return 0;
}

struct Node
{
    Node() = default;
    Node(const char& c): c(c)
    {};

    char c = '\0';
    bool endpoint = false;
    std::array<std::optional<std::shared_ptr<Node>>, 5> has;

    void add(const std::string& wrd)
    {
        if (wrd.empty())
        {
            endpoint = true;
            return;
        }

        if (!has[ctoint(wrd[0])].has_value())
        {
            has[ctoint(wrd[0])] = std::make_shared<Node>(wrd[0]);
        }
        has[ctoint(wrd[0])].value()->add(wrd.substr(1));
    }

    [[nodiscard]] bool contains(const std::span<char>& wrd) const {
        if (wrd.empty())
        {
            return endpoint;
        }

        if (!has[ctoint(wrd[0])].has_value())
        {
            return false;
        }
        return has[ctoint(wrd[0])].value()->contains(wrd.subspan(1));
    }

};

void canbemade(const Node& head, const std::span<char>& wan, dt& count, std::unordered_map<std::string, dt>& cache, bool tl = false)
{
    if (cache.contains({wan.begin(), wan.end()})) {
        count += cache[{wan.begin(), wan.end()}];
        return;
    }
    std::optional<std::shared_ptr<Node>> curnode = std::make_shared<Node>(head);
    for (const auto& [i, c]: enumerate(wan))
    {

        if (tl) {
            std::cout << i << '\n';
        }
        auto prevNode = curnode;
        curnode = curnode.value()->has[ctoint(c)];

        if (!curnode.has_value())
        {
            if (!prevNode.value()->endpoint)
            {
                return;
            }

            curnode = head.has[ctoint(c)];
            if (!curnode.has_value())
            {
                return;
            }
        }
        if (curnode.value()->endpoint)
        {

            if (curnode.value()->has[ctoint(wan[i+1])].has_value()) {
                auto init = count;
                canbemade(head, wan.subspan(i+1), count, cache);
                cache[{wan.begin()+i+1, wan.end()}] = count - init;
            }
        }

    }
    count += curnode.value()->endpoint;
}

int main(const int argc, char *argv[]) {
    easyTimer<std::chrono::milliseconds> _("main ");

    if (argc != 2)
    {
        std::cerr << "Invalid args" << std::endl;
        return 1;
    }

    const auto [avail, wanted] = getInput(argv[1]);

    Node head;

    for (auto w: avail)
    {
        ltrim(w);
        head.add(w);
    }

    dt c1 = 0;
    dt c2 = 0;
    std::unordered_map<std::string, dt> cache;
    for (auto wan : wanted)
    {
        dt tmp = 0;
        canbemade(head, wan, tmp, cache, false);
        std::cout << wan << '-'<< tmp << std::endl;

        c1 += tmp>0;
        c2 += tmp;
    }
    std::cout << c1 << " : " << c2 << std::endl;

    return 0;
}