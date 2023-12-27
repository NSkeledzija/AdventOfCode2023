#pragma once

#include "utils.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>

class Day12 {
public:
  bool is_match(const std::string &diagram, size_t offset, size_t len) {
    if (offset + len > diagram.size()) {
      return false;
    }
    auto i = offset;
    for (; i < offset + len; ++i) {
      if (diagram.at(i) == '.') {
        return false;
      }
    }
    if (i < diagram.size() && diagram.at(i) == '#') {
      return false;
    }
    return true;
  }

  std::map<std::pair<std::string, std::vector<long long>>, long long> memo;

  size_t solve(const std::string &diagram, const std::vector<long long> &list) {
    if (memo.find({diagram, list}) != memo.end()) {
      return memo.at({diagram, list});
    }
    long long hash_position = diagram.find_first_of('#');
    if (list.empty()) {
      return hash_position == std::string::npos;
    }

    auto len = list.front();
    if (len > diagram.size()) {
      return 0;
    }

    size_t match_length = 0;
    std::vector<size_t> matches;

    int begin = 0;
    int end;
    if (hash_position == std::string::npos) {
      end = diagram.size() - len;
    } else {
      end = std::min(hash_position, (long long)diagram.size() - len);
    }

    for (int i = begin; i <= end; ++i) {
      if (is_match(diagram, i, len)) {
        matches.push_back(i);
      }
    }

    size_t count = 0;
    for (auto match : matches) {
      auto nd = std::string{diagram.begin() + match + len, diagram.end()};
      if (nd.size() > 0) {
        nd.at(0) = '.';
      }
      auto nl = std::vector<long long>{list.begin() + 1, list.end()};
      count += solve(nd, nl);
    }
    memo[{diagram, list}] = count;
    return count;
  }

  void part1(const std::string &input) {
    auto lines = split(input, "\n");

    size_t total = 0;
    for (const auto &l : lines) {
      auto tokens = split(l, " ");
      auto diagram = tokens.at(0);
      auto groups = tokens.at(1);
      std::vector<long long> list_ints;
      for (const auto &n : split(groups, ",")) {
        list_ints.push_back(std::stoll(n));
      }
      total += solve(diagram, list_ints);
    }

    std::cout << "Total: " << total << std::endl;
  }

  void part2(const std::string &input) {
    auto lines = split(input, "\n");

    size_t total = 0;
    for (const auto &l : lines) {
      auto tokens = split(l, " ");
      auto diagram = tokens.at(0);
      auto groups = tokens.at(1);

      auto extended_diagram = diagram;
      auto extended_groups = groups;

      for (int i = 0; i < 4; ++i) {
        extended_diagram.append("?" + diagram);
        extended_groups.append("," + groups);
      }
      auto list = split(extended_groups, ",");
      std::vector<long long> list_ints;
      for (const auto &n : list) {
        list_ints.push_back(std::stoull(n));
      }

      total += solve(extended_diagram, list_ints);
    }

    std::cout << "Total: " << total << std::endl;
  }
};