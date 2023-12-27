#pragma once

#include "utils.h"
#include <cstddef>
#include <deque>
#include <iostream>
#include <numeric>
#include <set>

class Day16 {
public:
  struct Beam {
    Beam(const std::pair<int, int> &pos, Direction dir)
        : position(pos), dir(dir) {}
    std::pair<int, int> position{0, 0};
    Direction dir{Direction::EAST};
  };

  bool pass_through(Beam b, char segment) {
    if (segment == '.') {
      return true;
    }
    if (segment == '|' && b.dir == Direction::NORTH ||
        b.dir == Direction::SOUTH) {
      return true;
    }
    if (segment == '-' && b.dir == Direction::EAST ||
        b.dir == Direction::WEST) {
      return true;
    }
    return false;
  }

  size_t count_energized(const Beam &beam,
                         const std::vector<std::string> &grid) {
    std::deque<Beam> beams;
    beams.emplace_back(beam);
    std::set<std::pair<std::pair<size_t, size_t>, Direction>> visited;
    std::set<std::pair<size_t, size_t>> visited_locs;
    auto rows = grid.size();
    auto cols = grid.at(0).size();
    while (!beams.empty()) {
      auto b = beams.front();
      beams.pop_front();
      if (position_in_grid(b.position, rows, cols) &&
          visited.find({b.position, b.dir}) == visited.end()) {
        visited.insert({b.position, b.dir});
        visited_locs.insert(b.position);
        auto segment = grid.at(b.position.first).at(b.position.second);
        if (segment == '.') {
          beams.push_back(Beam{b.position + b.dir, b.dir});
        } else if (segment == '|') {
          if (b.dir == Direction::NORTH || b.dir == Direction::SOUTH) {
            beams.emplace_back(Beam{b.position + b.dir, b.dir});
          } else if (b.dir == Direction::EAST || b.dir == Direction::WEST) {
            beams.emplace_back(Beam{b.position + tc(b.dir), tc(b.dir)});
            beams.emplace_back(Beam{b.position + tcc(b.dir), tcc(b.dir)});
          }
        } else if (segment == '-') {
          if (b.dir == Direction::EAST || b.dir == Direction::WEST) {
            beams.emplace_back(Beam{b.position + b.dir, b.dir});
          } else if (b.dir == Direction::SOUTH || b.dir == Direction::NORTH) {
            beams.emplace_back(Beam{b.position + tc(b.dir), tc(b.dir)});
            beams.emplace_back(Beam{b.position + tcc(b.dir), tcc(b.dir)});
          }
        } else if (segment == '\\') {
          if (b.dir == Direction::NORTH || b.dir == Direction::SOUTH) {
            beams.emplace_back(Beam{b.position + tcc(b.dir), tcc(b.dir)});
          } else if (b.dir == Direction::EAST || b.dir == Direction::WEST) {
            beams.emplace_back(Beam{b.position + tc(b.dir), tc(b.dir)});
          }
        } else if (segment == '/') {
          if (b.dir == Direction::NORTH || b.dir == Direction::SOUTH) {
            beams.emplace_back(Beam{b.position + tc(b.dir), tc(b.dir)});
          } else if (b.dir == Direction::EAST || b.dir == Direction::WEST) {
            beams.emplace_back(Beam{b.position + tcc(b.dir), tcc(b.dir)});
          }
        }
      }
    }
    return visited_locs.size();
  }

  void part1(const std::string &input) {
    auto grid = split(input, "\n");
    std::cout << "Total: "
              << count_energized(Beam{{0, 0}, Direction::EAST}, grid)
              << std::endl;
  }

  void part2(const std::string &input) {
    auto grid = split(input, "\n");

    size_t max_energized = 0;
    // Upper edge, row = 0, col iterates
    for (int col = 0; col < grid.at(0).size(); ++col) {
      auto current = count_energized(Beam{{0, col}, Direction::SOUTH}, grid);
      max_energized = std::max(max_energized, current);
    }
    // Right edge, row iterates, col = grid.at(0).size() - 1
    for (int row = 0; row < grid.size(); ++row) {
      auto current = count_energized(
          Beam{{row, grid.at(0).size()}, Direction::WEST}, grid);
      max_energized = std::max(max_energized, current);
    }
    // Bottom edge, row = grid.size()-1, col iterates
    for (int col = 0; col < grid.at(0).size(); ++col) {
      auto current =
          count_energized(Beam{{grid.size() - 1, col}, Direction::NORTH}, grid);
      max_energized = std::max(max_energized, current);
    }
    // Left edge, row iterates, col is 0
    for (int row = 0; row < grid.at(0).size(); ++row) {
      auto current = count_energized(Beam{{row, 0}, Direction::EAST}, grid);
      max_energized = std::max(max_energized, current);
    }
    std::cout << "Total: " << max_energized;
  }
};