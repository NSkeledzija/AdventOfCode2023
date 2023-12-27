#pragma once

#include "utils.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <sys/types.h>

class Day9 {
public:
  std::vector<std::vector<int>> parse_input(const std::string &input) {
    auto histories = split(input, "\n");
    std::vector<std::vector<int>> out;
    for (const auto &line : histories) {
      std::vector<int> values;
      auto value_strings = split(line);
      for (const auto &vstr : value_strings) {
        values.push_back(std::stoul(vstr));
      }
      out.push_back(values);
    }
    return out;
  }

  size_t solve_history(const std::vector<int> &history) {
    const static auto zero_foo = [](int i) { return i == 0; };
    std::vector<std::vector<int>> value_lines;
    value_lines.push_back(history);
    std::vector<int> new_values;
    do {
      auto values = value_lines.back();
      new_values.clear();
      for (size_t index = 1; index < values.size(); ++index) {
        new_values.push_back(values.at(index) - values.at(index - 1));
      }
      value_lines.push_back(new_values);
    } while (!std::all_of(new_values.begin(), new_values.end(), zero_foo));

    value_lines.at(value_lines.size() - 1).push_back(0);
    for (int index = value_lines.size() - 2; index >= 0; --index) {
      auto first_val = value_lines.at(index).back();
      auto second_val = value_lines.at(index + 1).back();
      value_lines.at(index).push_back(first_val + second_val);
    }

    // for (const auto &l : value_lines) {
    //   std::cout << std::endl;
    //   for (const auto &v : l) {
    //     std::cout << " " << v;
    //   }
    // }

    return value_lines.at(0).back();
  }

  size_t solve_history_reverse(const std::vector<int> &history) {
    const static auto zero_foo = [](int i) { return i == 0; };
    std::vector<std::vector<int>> value_lines;
    value_lines.push_back(history);
    std::vector<int> new_values;
    do {
      auto values = value_lines.back();
      new_values.clear();
      for (size_t index = 1; index < values.size(); ++index) {
        new_values.push_back(values.at(index) - values.at(index - 1));
      }
      value_lines.push_back(new_values);
    } while (!std::all_of(new_values.begin(), new_values.end(), zero_foo));

    for (auto &l : value_lines) {
      std::reverse(l.begin(), l.end());
    }

    value_lines.at(value_lines.size() - 1).push_back(0);
    for (int index = value_lines.size() - 2; index >= 0; --index) {
      auto first_val = value_lines.at(index).back();
      auto second_val = value_lines.at(index + 1).back();
      value_lines.at(index).push_back(first_val - second_val);
    }
    return value_lines.at(0).back();
  }

  void part1(const std::string &input) {
    auto histories = parse_input(input);
    auto sum = 0;
    for (const auto &h : histories) {
      sum += solve_history(h);
    }

    std::cout << "Solution: " << sum << std::endl;
  }

  void part2(const std::string &input) {
    auto histories = parse_input(input);
    auto sum = 0;
    for (const auto &h : histories) {
      sum += solve_history_reverse(h);
    }

    std::cout << "Solution: " << sum << std::endl;
  }
};