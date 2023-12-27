#pragma once

#include "utils.h"
#include <cctype>
#include <cstddef>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

class Day3 {
public:
  bool is_symbol(size_t row, size_t col,
                 const std::vector<std::string> &lines) {
    if (!std::isdigit(lines[row][col]) && lines[row][col] != '.') {
      return true;
    }
    return false;
  }

  const std::vector<std::pair<int, int>> DIRECTIONS = {
      {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};

  bool if_digit_find_boundaries(
      std::pair<size_t, std::pair<size_t, size_t>> &boundaries, size_t row,
      size_t col, const std::vector<std::string> &lines,
      const std::pair<int, int> &direction) {
    int trow = row + direction.first;
    int tcol = col + direction.second;
    if (trow < 0 || trow > lines.size() - 1) {
      return false;
    }
    if (tcol < 0 || tcol > lines.at(0).size() - 1) {
      return false;
    }
    if (!std::isdigit(lines[trow][tcol])) {
      return false;
    }

    int begin_col = tcol;
    while (std::isdigit(lines[trow][begin_col]) && begin_col >= 0) {
      --begin_col;
    }

    int end_col = tcol;
    while (std::isdigit(lines[trow][end_col]) &&
           end_col < lines.at(trow).size()) {
      ++end_col;
    }

    boundaries = {trow, {begin_col + 1, end_col - 1}};
    return true;
  }

  std::vector<int> neighboring_numbers(size_t row, size_t col,
                                       const std::vector<std::string> &lines) {
    std::vector<int> numbers;
    std::set<std::pair<size_t, std::pair<size_t, size_t>>> unique_nrs;

    for (const auto &d : DIRECTIONS) {
      std::pair<size_t, std::pair<size_t, size_t>> boundaries;
      if (if_digit_find_boundaries(boundaries, row, col, lines, d)) {
        unique_nrs.insert(boundaries);
      }
    }

    for (auto &boundaries : unique_nrs) {
      size_t row = boundaries.first;
      size_t begin_col = boundaries.second.first;
      size_t end_col = boundaries.second.second;

      std::string current = "";
      while (begin_col <= end_col) {
        current += lines[row][begin_col];
        ++begin_col;
      }
      numbers.push_back(atoi(current.c_str()));
    }
    return numbers;
  }

  void part1(const std::string &input) {
    auto lines = split(input, "\n");

    long long sum = 0;
    for (size_t row = 0; row < lines.size(); ++row) {
      for (size_t col = 0; col < lines.at(row).size(); ++col) {
        if (is_symbol(row, col, lines)) {
          for (auto n : neighboring_numbers(row, col, lines)) {
            sum += n;
          }
        }
      }
    }
    std::cout << "Sum is: " << sum << std::endl;
  }

  bool if_gear_get_ratio(long long &gear_ratio, size_t row, size_t col,
                         const std::vector<std::string> &lines) {
    if (lines[row][col] != '*') {
      return false;
    }

    auto numbers = neighboring_numbers(row, col, lines);

    if (numbers.size() == 2) {
      gear_ratio = numbers[0] * numbers[1];
      return true;
    }
    return false;
  }

  void part2(const std::string &input) {
    auto lines = split(input, "\n");

    long long sum = 0;
    for (size_t row = 0; row < lines.size(); ++row) {
      auto line = lines.at(row);
      for (size_t col = 0; col < line.size(); ++col) {

        long long ratio;
        if (if_gear_get_ratio(ratio, row, col, lines)) {
          sum += ratio;
        }
      }
    }
    std::cout << "Sum is: " << sum << std::endl;
  }
};