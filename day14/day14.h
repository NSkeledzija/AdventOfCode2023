#pragma once

#include "utils.h"
#include <cmath>
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

class Day14 {
public:
  void roll_north(std::vector<std::string> &grid) {
    for (int row = 1; row < grid.size(); ++row) {
      for (int col = 0; col < grid.at(row).size(); ++col) {
        size_t swap_row = row;
        while (swap_row >= 1 && grid.at(swap_row - 1).at(col) == '.' &&
               grid.at(swap_row).at(col) == 'O') {
          auto &higher = grid.at(swap_row - 1).at(col);
          auto &lower = grid.at(swap_row).at(col);
          std::swap(higher, lower);
          --swap_row;
        }
      }
    }
  }

  void roll_east(std::vector<std::string> &grid) {
    for (int row = 0; row < grid.size(); ++row) {
      for (int col = grid.at(row).size() - 1; col >= 0; --col) {
        size_t swap_col = col;
        while (swap_col < grid.at(row).size() - 1 &&
               grid.at(row).at(swap_col + 1) == '.' &&
               grid.at(row).at(swap_col) == 'O') {
          auto &east = grid.at(row).at(swap_col + 1);
          auto &west = grid.at(row).at(swap_col);
          std::swap(east, west);
          ++swap_col;
        }
      }
    }
  }

  void roll_south(std::vector<std::string> &grid) {
    for (int row = grid.size() - 1; row >= 0; --row) {
      for (int col = 0; col < grid.at(row).size(); ++col) {
        size_t swap_row = row;
        while (swap_row < grid.size() - 1 &&
               grid.at(swap_row + 1).at(col) == '.' &&
               grid.at(swap_row).at(col) == 'O') {
          auto &lower = grid.at(swap_row + 1).at(col);
          auto &higher = grid.at(swap_row).at(col);
          std::swap(higher, lower);
          ++swap_row;
        }
      }
    }
  }

  void roll_west(std::vector<std::string> &grid) {
    for (int row = 0; row < grid.size(); ++row) {
      for (int col = 1; col < grid.size(); ++col) {
        size_t swap_col = col;
        while (swap_col >= 1 && grid.at(row).at(swap_col - 1) == '.' &&
               grid.at(row).at(swap_col) == 'O') {
          auto &west = grid.at(row).at(swap_col - 1);
          auto &east = grid.at(row).at(swap_col);
          std::swap(east, west);
          --swap_col;
        }
      }
    }
  }

  bool in_grid(const std::vector<std::string> &grid, int row, int col) {
    return row >= 0 && row < grid.size() && col >= 0 &&
           col < grid.at(row).size();
  }

  void part1(const std::string &input) {
    auto grid = split(input, "\n");
    roll_north(grid);
    size_t load = 0;
    for (int row = 0; row < grid.size(); ++row) {
      for (int col = 0; col < grid.at(row).size(); ++col) {
        if (grid.at(row).at(col) == 'O') {
          load += grid.size() - row;
        }
      }
    }
    std::cout << "Total: " << load << std::endl << std::endl;
    ;
  }

  std::string join(const std::vector<std::string> &in, std::string delim) {
    std::stringstream ss;

    for (int i = 0; i < in.size() - 1; ++i) {
      ss << in.at(i) << delim;
    }
    ss << in.back();
    return ss.str();
  }

  void print_grid(const std::vector<std::string> &grid) {
    size_t count = 0;
    for (int i = 0; i < grid.size(); ++i) {
      for (int j = 0; j < grid.at(i).size(); ++j) {
        std::cout << grid.at(i).at(j);
        if (grid.at(i).at(j) == 'O') {
          ++count;
        }
      }
      std::cout << std::endl;
    }
    std::cout << count << std::endl << std::endl;
    ;
  }

  void part2(const std::string &input) {
    auto grid = split(input, "\n");
    std::unordered_map<std::string, size_t> grids;
    size_t load = 0;
    size_t offset = 0;
    size_t period = 0;
    size_t total_cycles = 1000000000;

    for (int i = 0; i < 1000000000; ++i) {
      roll_north(grid);
      roll_west(grid);
      roll_south(grid);
      roll_east(grid);
      auto joined = join(grid, "\n");
      if (grids.find(joined) != grids.end()) {
        auto first_cycle = grids.at(joined);
        auto second_cycle = i;
        period = second_cycle - first_cycle;
        offset = first_cycle;
        break;
      } else {
        grids.insert({joined, i});
      }
    }

    size_t cycles_left = (total_cycles - offset + 1) % period;
    for (int i = 0; i < cycles_left; ++i) {
      roll_north(grid);
      roll_west(grid);
      roll_south(grid);
      roll_east(grid);
    }

    for (int row = 0; row < grid.size(); ++row) {
      for (int col = 0; col < grid.at(row).size(); ++col) {
        if (grid.at(row).at(col) == 'O') {
          load += grid.size() - row;
        }
      }
    }
    std::cout << "Total: " << load << std::endl;
  }
};