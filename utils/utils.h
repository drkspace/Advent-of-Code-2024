//
// Created by daniel on 12/18/24.
//

#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <ranges>
#include <format>
#include <print>

inline const auto& enumerate = std::ranges::views::enumerate;

template <typename T>
struct Point
{
    T x;
    T y;

    Point() = default;
    Point(T x, T y): x(x), y(y){};
    Point(const Point& other) = default;

    [[nodiscard]] bool operator==(const Point&) const = default;
    [[nodiscard]] Point operator+(const Point& other) const
    {
        return {this->x+other.x, this->y+other.y};
    }
    [[nodiscard]] Point operator-(const Point& other) const
    {
        return {this->x-other.x, this->y-other.y};
    }
    [[nodiscard]] Point operator*(const Point& other) const
    {
        return {this->x*other.x, this->y*other.y};
    }
    [[nodiscard]] Point operator*(const T& other) const
    {
        return {this->x*other, this->y*other};
    }
    [[nodiscard]] Point operator/(const Point& other) const
    {
        return {this->x*other.x, this->y*other.y};
    }
    [[nodiscard]] Point operator/(const T& other) const
    {
        return {this->x/other, this->y/other};
    }
};

template<typename T>
struct std::hash<Point<T>>
{
    std::size_t operator()(const Point<T>& p) const noexcept
    {
        const std::size_t h1 = std::hash<T>{}(p.x);
        const std::size_t h2 = std::hash<T>{}(p.y);
        return h1 ^ (h2 << 1);
    }
};

template<typename T>
struct std::formatter<Point<T>> : std::formatter<std::string> {
    auto format(Point<T> p, format_context& ctx) const {
        return formatter<string>::format(
          std::format("({}, {})", p.x, p.y), ctx);
    }
};

inline auto split(const std::string& inp, const char delim) {
    std::string tmp;
    std::vector<std::string> r;
    std::istringstream s(inp);
    while (std::getline(s, tmp, delim)) {
        r.push_back(tmp);
    }
    return r;
};

struct hash_tuple {
    // copied from https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c/
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        // Hash the first element
        const size_t hash1 = std::hash<T1>{}(p.first);
        // Hash the second element
        const size_t hash2 = std::hash<T2>{}(p.second);
        // Combine the two hash values
        return hash1
               ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
                  + (hash1 >> 2));
    }

    template <class T1, class T2, class T3>
    size_t operator()(const std::tuple<T1, T2, T3>& p) const
    {
        return hash_tuple()(std::make_pair(hash_tuple()(std::make_pair(std::get<0>(p), std::get<1>(p))), std::get<2>(p)));
    }

    template <class T1, class T2, class T3, class T4>
    size_t operator()(const std::tuple<T1, T2, T3, T4>& p) const
    {
        return hash_tuple()(std::make_pair(hash_tuple()(std::make_pair(hash_tuple()(std::make_pair(std::get<0>(p), std::get<1>(p))), std::get<2>(p))), std::get<3>(p)));
    }

};

template <class T1, class T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
    os << "(" << p.first << "," << p.second << ")";
    return os;
}

enum class Dir{
    UP,DOWN,LEFT,RIGHT
};

template <typename T,typename U>
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r) {
    return {l.first+r.first,l.second+r.second};
}

inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

#endif //UTILS_H
