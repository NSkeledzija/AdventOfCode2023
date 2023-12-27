#pragma once

#include "utils.h"
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

inline std::pair<long long, long long>
operator+(std::pair<long long, long long> xy, std::pair<char, long long> move) {

  if (move.first == 'R') {
    return {xy.first, xy.second + move.second};
  }
  if (move.first == 'D') {
    return {xy.first + move.second, xy.second};
  }
  if (move.first == 'L') {
    return {xy.first, xy.second - move.second};
  }
  if (move.first == 'U') {
    return {xy.first - move.second, xy.second};
  }
  throw std::runtime_error("Invalid move");
}

class Day18 {
public:
  void part1(const std::string &input) {
    auto lines = split(input, "\n");
    std::pair<long long, long long> xy1 = {0, 0};
    long long area = 0;
    long long diameter = 0;
    for (const auto &line : lines) {
      auto tokens = split(line);
      auto xy2 = xy1 + std::pair<char, long long>{tokens.at(0).at(0),
                                                  stoi(tokens.at(1))};

      area += xy1.first * xy2.second - xy2.first * xy1.second;
      diameter += abs(xy1.first - xy2.first) + abs(xy1.second - xy2.second);
      xy1 = xy2;
    }
    area = std::abs(area / 2) + diameter / 2 + 1;

    std::cout << "Area: " << area << std::endl;
  }

  std::pair<char, long long> decode(const std::string &hex) {
    std::array<char, 4> chars = {'R', 'D', 'L', 'U'};
    auto c = chars[hex.at(hex.size() - 2) - '0'];

    std::stringstream ss;
    for (long long i = 2; i < hex.size() - 2; ++i) {
      ss << std::hex << hex[i];
    }

    long long x;
    ss >> x;

    return {c, x};
  }

  void part2(const std::string &input) {
    auto lines = split(input, "\n");
    std::pair<long long, long long> xy1 = {0, 0};
    long long area = 0;
    long long diameter = 0;
    for (const auto &line : lines) {
      auto tokens = split(line);
      auto decoded = decode(tokens.at(2));
      auto xy2 = xy1 + decoded;

      area += xy1.first * xy2.second - xy2.first * xy1.second;
      diameter += abs(xy1.first - xy2.first) + abs(xy1.second - xy2.second);
      xy1 = xy2;
    }
    area = std::abs(area / 2) + diameter / 2 + 1;

    std::cout << "Area: " << area << std::endl;
  }
};