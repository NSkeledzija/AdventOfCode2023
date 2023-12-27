#pragma once

#include "utils.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>

class Day11 {
public:
  auto find_galaxies(const std::vector<std::string> &universe) {
    std::vector<std::pair<size_t, size_t>> out;
    for (size_t r = 0; r < universe.size(); ++r) {
      for (auto c = 0; c < universe.at(0).size(); ++c) {
        if (universe.at(r).at(c) == '#') {
          out.emplace_back(r, c);
        }
      }
    }
    return out;
  }

  std::vector<std::string> gap_mark(const std::vector<std::string> &universe) {
    std::vector<std::string> marked = universe;
    for (size_t row = 0; row < universe.size(); ++row) {
      bool contains_galaxy = false;
      for (size_t col = 0; col < universe.at(row).size(); ++col) {
        if (universe.at(row).at(col) == '#') {
          contains_galaxy = true;
        }
      }
      if (!contains_galaxy) {
        for (size_t col = 0; col < universe.at(row).size(); ++col) {
          marked.at(row).at(col) = 'X';
        }
      }
    }
    for (size_t col = 0; col < universe.at(0).size(); ++col) {
      bool contains_galaxy = false;
      for (size_t row = 0; row < universe.size(); ++row) {
        if (universe.at(row).at(col) == '#') {
          contains_galaxy = true;
        }
      }

      if (!contains_galaxy) {
        for (size_t row = 0; row < universe.size(); ++row) {
          marked.at(row).at(col) = 'X';
        }
      }
    }
    return marked;
  }

  size_t calculate_distance(const std::vector<std::string> &universe,
                            std::pair<size_t, size_t> first,
                            std::pair<size_t, size_t> second, size_t gap_size) {
    auto min_row = std::min(first.first, second.first);
    auto max_row = std::max(first.first, second.first);

    auto min_col = std::min(first.second, second.second);
    auto max_col = std::max(first.second, second.second);

    size_t distance = 0;
    size_t row = min_row;
    for (; row < max_row; ++row) {
      if (universe.at(row).at(min_col) == 'X') {
        distance += gap_size;
      } else {
        distance += 1;
      }
    }

    for (auto col = min_col; col < max_col; ++col) {
      if (universe.at(row).at(col) == 'X') {
        distance += gap_size;
      } else {
        distance += 1;
      }
    }
    return distance;
  }

  size_t solve(const std::string &input, size_t gap_size) {
    auto universe = split(input, "\n");
    universe = gap_mark(universe);
    auto galaxies = find_galaxies(universe);

    size_t distance = 0;
    for (size_t first = 0; first < galaxies.size() - 1; ++first) {
      for (size_t second = first + 1; second < galaxies.size(); ++second) {
        distance += calculate_distance(universe, galaxies.at(first),
                                       galaxies.at(second), gap_size);
      }
    }

    return distance;
  }

  void part1(const std::string &input) {
    auto distance = solve(input, 2);
    std::cout << "Distance: " << distance << std::endl;
  }

  void part2(const std::string &input) {
    auto distance = solve(input, 1000000);
    std::cout << "Distance: " << distance << std::endl;
  }
};