#pragma once

#include "utils.h"
#include <array>
#include <cmath>
#include <cstddef>
#include <deque>
#include <iostream>
#include <set>
#include <stdexcept>

class Day21 {
public:
  position find_start(const std::vector<std::string> &grid) {
    for (int i = 0; i < grid.size(); ++i) {
      for (int j = 0; j < grid.at(0).size(); ++j) {
        if (grid.at(i).at(j) == 'S') {
          return {i, j};
        }
      }
    }
    throw std::runtime_error("No start in grid");
  }

  const std::array<Direction, 4> DIRS = {Direction::NORTH, Direction::EAST,
                                         Direction::WEST, Direction::SOUTH};

  size_t solve_input_multiplied(const std::string &input,
                                size_t extra_periods) {
    auto input_grid = split(input, "\n");
    const auto input_rows = input_grid.size();
    const auto input_cols = input_grid.at(0).size();

    std::vector<std::string> grid(input_grid.size() +
                                  input_grid.size() * 2 * extra_periods);
    for (int i = 0; i < grid.size(); ++i) {
      auto row = input_grid[i % input_grid.size()];
      for (int j = 0; j < 2 * extra_periods; ++j) {
        row += input_grid[i % input_grid.size()];
      }
      grid[i] = row;
    }

    for (int i = 0; i < grid.size(); ++i) {
      for (int j = 0; j < grid.size(); ++j) {
        if (grid.at(i).at(j) == 'S') {
          grid.at(i).at(j) = '.';
        }
      }
    }

    std::deque<position> next;
    std::deque<position> prev;

    prev.push_back({grid.size() / 2, grid.size() / 2});

    for (int i = 0; i < input_rows / 2 + input_rows * extra_periods; ++i) {
      std::set<position> visited;

      while (!prev.empty()) {
        auto current = prev.front();
        prev.pop_front();
        for (auto d : DIRS) {
          auto np = current + d;
          if (position_in_grid(np, grid.size(), grid.size()) &&
              (grid.at(np.first).at(np.second) == '.' ||
               grid.at(np.first).at(np.second) == 'S') &&
              visited.find(np) == visited.end()) {
            visited.insert(np);
            next.push_back(np);
          }
        }
      }
      prev = next;
      next = {};
    }
    // print_grid_and_steps(grid, prev);
    return prev.size();
  }

  void part1(const std::string &input) {
    auto result = solve_input_multiplied(input, 0);
    std::cout << "Total: " << result << std::endl;
  }

  void print_grid_and_steps(const std::vector<std::string> &grid,
                            const std::deque<position> &steps) {
    auto new_grid = grid;
    for (auto p : steps) {
      new_grid.at(p.first).at(p.second) = 'O';
    }
    for (int i = 0; i < new_grid.size(); ++i) {
      for (int j = 0; j < new_grid.at(0).size(); ++j) {
        std::cout << new_grid.at(i).at(j);
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  void part2(const std::string &input) {
    const auto NR_STEPS = 26501365;
    auto x0 = 0;
    auto x1 = 1;
    auto x2 = 2;
    auto y0 = solve_input_multiplied(input, x0);
    auto y1 = solve_input_multiplied(input, x1);
    auto y2 = solve_input_multiplied(input, x2);

    auto a = (double)(y0 + y2) / 2 - y1;
    auto b = -3 * ((double)y0 / 2) + 2 * y1 - (double)y2 / 2;
    auto c = y0;

    auto x = ((NR_STEPS - 65) / 131);
    unsigned long long solution = std::round(a * x * x + b * x + c);
    std::cout << "Total: " << solution << std::endl;
  }
};