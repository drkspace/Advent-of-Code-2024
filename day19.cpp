
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

};

bool canbemade(const Node& head, std::span<char> wan, const std::unordered_set<std::string>& avail )
{
    std::optional<std::shared_ptr<Node>> curnode = std::make_shared<Node>(head);
    for (const auto& [i, c]: enumerate(wan))
    {
        auto prevNode = curnode;
        curnode = curnode.value()->has[ctoint(c)];

        if (!curnode.has_value())
        {
            if (!prevNode.value()->endpoint)
            {
                return false;
            }

            curnode = head.has[ctoint(c)];
            if (!curnode.has_value())
            {
                return false;
            }
        }
        else
        {
            if (curnode.value()->endpoint)
            {
                if (canbemade(head, wan.subspan(i+1), avail))
                {
                    return true;
                }
            }
        }

    }
    return curnode.value()->endpoint;
}

int matches(const std::string wan, const std::unordered_set<std::string>& avail)
{
    int count = 0;
    for (size_t i = 1; i<=wan.size(); ++i)
    {

        if (auto tmp = wan.substr(0, i); avail.contains(tmp))
        {
            if (i == (wan.size()))
            {
                return count+1;
            }
            else
            {
                count += matches({wan.begin()+i, wan.end()}, avail);
            }
        }
    }
    return count;
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

    int c = 0;
    for (auto wan : wanted)
    {
        auto v = canbemade(head, wan, avail);
        std::cout << wan << '-'<< (v?"true":"false") << std::endl;
        c += v;
    }
    std::cout << c << std::endl;
    return 0;
    c = 0;
    matches("gbbr", avail);
    for (auto wan : wanted)
    {
        auto v = matches(wan, avail);
        std::cout << wan << '-'<< v << std::endl;
        c += v;
    }
    std::cout << c << std::endl;

    return 0;
}