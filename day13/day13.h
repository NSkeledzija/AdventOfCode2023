#pragma once

#include <cstddef>
#include <iostream>
#include <utils.h>

class Day13 {
public:
  bool is_row_mirrored(const std::string &row, int left) {
    int right = left + 1;
    while (left >= 0 && right < row.size()) {
      if (row.at(left) != row.at(right)) {
        return false;
      }
      --left;
      ++right;
    }
    return true;
  }

  bool is_vertical_mirror(const std::vector<std::string> &lines, int left) {
    int right = left + 1;
    for (const auto &row : lines) {
      if (!is_row_mirrored(row, left)) {
        return false;
      }
    }
    return true;
  }

  bool is_col_mirrored(const std::vector<std::string> &lines, int col,
                       int top) {
    int bottom = top + 1;
    while (top >= 0 && bottom < lines.size()) {
      if (lines.at(top).at(col) != lines.at(bottom).at(col)) {
        return false;
      }
      --top;
      ++bottom;
    }
    return true;
  }

  bool is_horizontal_mirror(const std::vector<std::string> &lines, int top) {
    int bottom = top + 1;
    for (size_t col = 0; col < lines.at(0).size(); ++col) {

      if (!is_col_mirrored(lines, col, top)) {
        return false;
      }
    }
    return true;
  }

  std::vector<size_t> find_mirrors(const std::vector<std::string> &lines) {
    int left = 0;
    int right = 1;
    std::vector<size_t> mirrors;
    while (right < lines.at(0).size()) {
      if (is_vertical_mirror(lines, left)) {
        mirrors.push_back(left + 1);
      }
      ++left;
      ++right;
    }
    int top = 0;
    int bottom = 1;

    while (bottom < lines.size()) {
      if (is_horizontal_mirror(lines, top)) {
        mirrors.push_back((top + 1) * 100);
      }
      ++top;
      ++bottom;
    }

    return mirrors;
  }

  void part1(const std::string &input) {
    auto grids = split(input, "\n\n");

    size_t total = 0;
    for (const auto &g : grids) {
      auto lines = split(g, "\n");
      total += find_mirrors(lines).at(0);
    }
    std::cout << "Total: " << total << std::endl;
  }

  size_t find_smudged_mirror(const std::vector<std::string> &lines) {
    auto tmp_lines = lines;
    auto mirror_old = find_mirrors(lines).at(0);

    for (int i = 0; i < tmp_lines.size(); ++i) {
      for (int j = 0; j < tmp_lines.at(i).size(); ++j) {
        tmp_lines.at(i).at(j) = tmp_lines.at(i).at(j) == '#' ? '.' : '#';
        auto mirrors_new = find_mirrors(tmp_lines);
        for (auto m : mirrors_new) {
          if (mirror_old != m) {
            return m;
          }
        }
        tmp_lines.at(i).at(j) = tmp_lines.at(i).at(j) == '#' ? '.' : '#';
      }
    }
    return 0;
  }

  void part2(const std::string &input) {
    auto grids = split(input, "\n\n");

    size_t total = 0;
    for (const auto &g : grids) {
      auto lines = split(g, "\n");
      total += find_smudged_mirror(lines);
    }
    std::cout << "Total: " << total << std::endl;
  }
};