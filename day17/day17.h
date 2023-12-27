#pragma once

#include "utils.h"
#include <bits/stdc++.h>
#include <cstddef>
#include <functional>
#include <limits>
#include <stdexcept>

using namespace std::chrono;

class Day17 {
public:
  struct State {
    position p;
    Direction d;
    int straight_length;
    int heatloss;
  };

  struct cmp {
    bool operator()(State const &s1, State const &s2) {
      return s1.heatloss >= s2.heatloss;
    }
  };

  std::vector<State> possible_moves(const State &current,
                                    const std::vector<std::vector<int>> &grid,
                                    size_t min, size_t max) {
    std::vector<State> moves;
    auto tcposition = current.p + tc(current.d);
    const auto rows = grid.size();
    const auto cols = grid.at(0).size();
    if (position_in_grid(tcposition, rows, cols) &&
        current.straight_length >= min) {
      auto heat = grid[tcposition.first][tcposition.second];
      moves.push_back({tcposition, tc(current.d), 1, current.heatloss + heat});
    }
    auto tccposition = current.p + tcc(current.d);
    if (position_in_grid(tccposition, rows, cols) &&
        current.straight_length >= min) {
      auto heat = grid[tccposition.first][tccposition.second];
      moves.push_back(
          {tccposition, tcc(current.d), 1, current.heatloss + heat});
    }

    auto sposition = current.p + current.d;
    if (current.straight_length < max &&
        position_in_grid(sposition, rows, cols)) {
      auto heat = grid[sposition.first][sposition.second];
      moves.push_back({sposition, current.d, current.straight_length + 1,
                       current.heatloss + heat});
    }
    return moves;
  }

  auto init_heatmap(const std::vector<std::vector<int>> &grid,
                    size_t max_straight) {
    std::vector<std::vector<std::vector<std::array<int, 4>>>> heatmap;

    for (int i = 0; i < grid.size(); ++i) {
      std::vector<std::vector<std::array<int, 4>>> row;
      for (int j = 0; j < grid.at(0).size(); ++j) {
        std::vector<std::array<int, 4>> directions;
        for (int straight_length = 0; straight_length < max_straight;
             ++straight_length) {
          const auto val = std::numeric_limits<int>::max();
          directions.push_back({val, val, val, val});
        }
        row.push_back(directions);
      }
      heatmap.push_back(row);
    }

    return heatmap;
  }

  int least_heat(const std::string &input, size_t min, size_t max) {
    auto lines = split(input, "\n");
    std::vector<std::vector<int>> grid;
    for (const auto &line : lines) {
      std::vector<int> row;
      for (const auto c : line) {
        row.push_back(c - '0');
      }
      grid.push_back(row);
    }

    std::priority_queue<State, std::vector<State>, cmp> queue;
    auto heatmap = init_heatmap(grid, max);

    queue.push(State{{0, 0}, Direction::EAST, 0, 0});
    queue.push(State{{0, 0}, Direction::SOUTH, 0, 0});
    while (!queue.empty()) {
      auto current = queue.top();
      queue.pop();

      if (current.p.first == grid.size() - 1 &&
          current.p.second == grid.at(0).size() - 1) {
        return current.heatloss;
      }

      for (const auto &m : possible_moves(current, grid, min, max)) {
        auto old =
            heatmap[m.p.first][m.p.second][m.straight_length - 1][(size_t)m.d];

        if (old == std::numeric_limits<int>::max()) {
          heatmap[m.p.first][m.p.second][m.straight_length - 1][(size_t)m.d] =
              m.heatloss;
          queue.push(m);
        }
      }
    }
    throw std::runtime_error("No path through graph!");
  }

  void part1(const std::string &input) {
    std::cout << "Total: " << least_heat(input, 0, 3) << std::endl;
  }

  void part2(const std::string &input) {
    std::cout << "Total: " << least_heat(input, 4, 10) << std::endl;
  }
};