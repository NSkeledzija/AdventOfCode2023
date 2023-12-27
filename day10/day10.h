#pragma once

#include "utils.h"
#include <array>
#include <cstddef>
#include <iostream>
#include <set>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

class Day10 {
public:
  bool does_pipe_connect(char c, Direction d) {
    if (c == '|') {
      return d == Direction::NORTH || d == Direction::SOUTH;
    } else if (c == '-') {
      return d == Direction::EAST || d == Direction::WEST;
    } else if (c == 'L') {
      return d == Direction::NORTH || d == Direction::EAST;
    } else if (c == 'J') {
      return d == Direction::NORTH || d == Direction::WEST;
    } else if (c == '7') {
      return d == Direction::SOUTH || d == Direction::WEST;
    } else if (c == 'F') {
      return d == Direction::SOUTH || d == Direction::EAST;
    } else if (c == 'S') {
      return true;
    } else if (c == '.') {
      return false;
    } else {
      throw std::runtime_error("Invalid pipe!");
    }
  }

  Direction reverse(Direction d) {
    if (d == Direction::NORTH)
      return Direction::SOUTH;
    if (d == Direction::EAST)
      return Direction::WEST;
    if (d == Direction::SOUTH)
      return Direction::NORTH;
    if (d == Direction::WEST)
      return Direction::EAST;
    throw std::runtime_error("Invalid direction!");
  }

  bool connected(std::pair<int, int> current, Direction d,
                 const std::vector<std::vector<char>> &grid) {
    if (!does_pipe_connect(grid.at(current.first).at(current.second), d)) {
      return false;
    }
    std::pair<int, int> direction = direction_to_offsets(d);
    auto destination = current + direction;
    if (destination.first < 0 || destination.first > grid.size() - 1) {
      return false;
    }

    if (destination.second < 0 ||
        destination.second > grid.at(destination.first).size() - 1) {
      return false;
    }

    auto pipe = grid.at(destination.first).at(destination.second);
    return does_pipe_connect(pipe, reverse(d));
  }

  auto parse_input(const std::string &input) {
    auto lines = split(input, "\n");
    std::vector<std::vector<char>> out;
    for (const auto &line : lines) {
      std::vector<char> pipes;
      for (auto c : line) {
        pipes.push_back(c);
      }
      out.push_back(pipes);
    }
    return out;
  }

  auto find_start(const std::vector<std::vector<char>> &grid) {
    for (int row = 0; row < grid.size(); ++row) {
      for (int col = 0; col < grid.at(row).size(); ++col) {
        if (grid.at(row).at(col) == 'S') {
          return std::pair{row, col};
        }
      }
    }
    throw std::runtime_error("Invalid grid!");
  }

  bool is_clockwise_turn(Direction current, Direction next) {
    if (current == Direction::NORTH && next == Direction::EAST ||
        current == Direction::EAST && next == Direction::SOUTH ||
        current == Direction::SOUTH && next == Direction::WEST ||
        current == Direction::WEST && next == Direction::NORTH) {
      return true;
    }
    return false;
  }

  bool is_counter_clockwise_turn(Direction current, Direction next) {
    if (current == Direction::NORTH && next == Direction::WEST ||
        current == Direction::WEST && next == Direction::SOUTH ||
        current == Direction::SOUTH && next == Direction::EAST ||
        current == Direction::EAST && next == Direction::NORTH) {
      return true;
    }
    return false;
  }

  std::tuple<bool, std::vector<Direction>, std::set<std::pair<int, int>>>
  analyze_loop(const std::vector<std::vector<char>> &grid) {
    auto start = find_start(grid);
    const std::array<Direction, 4> directions = {
        Direction::NORTH, Direction::EAST, Direction::SOUTH, Direction::WEST};
    auto current_position = start;
    std::pair<int, int> next_position;
    char current = 'S';
    size_t clockwise_turns = 0;
    size_t counter_clockwise_turns = 0;
    std::set<std::pair<int, int>> visited;
    Direction prev_direction = Direction::NONE;

    std::vector<Direction> steps_taken;
    do {
      visited.insert(current_position);
      for (auto d : directions) {
        if (prev_direction == reverse(d))
          continue;
        next_position = current_position + direction_to_offsets(d);
        if (connected(current_position, d, grid)) {
          if (next_position == start) {
            return {clockwise_turns > counter_clockwise_turns, steps_taken,
                    visited};
          }
          if (visited.find(next_position) != visited.end()) {
            continue;
          }
          current_position = next_position;
          current = grid.at(current_position.first).at(current_position.second);
          if (is_clockwise_turn(prev_direction, d)) {
            ++clockwise_turns;
          }
          if (is_counter_clockwise_turn(prev_direction, d)) {
            ++counter_clockwise_turns;
          }
          steps_taken.push_back(d);
          prev_direction = d;
          break;
        }
      }
    } while (current != 'S');
    throw std::runtime_error("Invalid traversal!");
  }

  size_t traverse_grid(const std::vector<std::vector<char>> &grid) {
    auto start = find_start(grid);
    const std::array<Direction, 4> directions = {
        Direction::NORTH, Direction::EAST, Direction::SOUTH, Direction::WEST};
    auto current_position = start;
    std::pair<int, int> next_position;
    char current = 'S';
    size_t steps = 0;
    std::set<std::pair<int, int>> visited;
    do {
      visited.insert(current_position);
      for (auto d : directions) {
        next_position = current_position + direction_to_offsets(d);
        if (connected(current_position, d, grid)) {
          if (steps > 1 && next_position == start) {
            return steps;
          }
          if (visited.find(next_position) != visited.end()) {
            continue;
          }
          current_position = next_position;
          current = grid.at(current_position.first).at(current_position.second);
          ++steps;
          break;
        }
      }
    } while (current != 'S');
    throw std::runtime_error("Invalid traversal!");
  }

  bool paint(std::vector<std::vector<char>> &grid,
             const std::pair<int, int> &paint_position,
             const std::set<std::pair<int, int>> &visited) {

    if (paint_position.first < 0 || paint_position.first > grid.size() - 1) {
      return false;
    }

    if (paint_position.second < 0 ||
        paint_position.second > grid.at(paint_position.first).size() - 1) {
      return false;
    }

    if (grid.at(paint_position.first).at(paint_position.second) == '.') {
      grid.at(paint_position.first).at(paint_position.second) = 'X';
      return true;
    }

    if (visited.find(paint_position) == visited.end()) {
      grid.at(paint_position.first).at(paint_position.second) = 'X';
      return true;
    }
    return false;
  }

  std::vector<Direction> paint_directions(Direction d, char p, bool clockwise) {
    if (p == '|') {
      if (clockwise) {
        if (d == Direction::NORTH) {
          return {Direction::EAST};
        }
        if (d == Direction::SOUTH) {
          return {Direction::WEST};
        }
        throw std::runtime_error("Invalid paint combination!");
      }
      if (d == Direction::NORTH) {
        return {Direction::WEST};
      }
      if (d == Direction::SOUTH) {
        return {Direction::EAST};
      }
      throw std::runtime_error("Invalid paint combination!");
    }

    if (p == '-') {
      if (clockwise) {
        if (d == Direction::EAST) {
          return {Direction::SOUTH};
        }
        if (d == Direction::WEST) {
          return {Direction::NORTH};
        }
        throw std::runtime_error("Invalid paint combination!");
      }
      if (d == Direction::EAST) {
        return {Direction::NORTH};
      }
      if (d == Direction::WEST) {
        return {Direction::SOUTH};
      }
      throw std::runtime_error("Invalid paint combination!");
    }

    if (p == 'L') {
      if (clockwise) {
        if (d == Direction::SOUTH) {
          return {Direction::WEST, Direction::SOUTH};
        }
        if (d == Direction::WEST) {
          return {Direction::NORTH, Direction::EAST};
        }
        throw std::runtime_error("Invalid paint combination!");
      }
      if (d == Direction::SOUTH) {
        return {Direction::EAST, Direction::NORTH};
      }
      if (d == Direction::WEST) {
        return {Direction::SOUTH, Direction::WEST};
      }
      throw std::runtime_error("Invalid paint combination!");
    }

    if (p == 'J') {
      if (clockwise) {
        if (d == Direction::SOUTH) {
          return {Direction::WEST, Direction::NORTH};
        }
        if (d == Direction::EAST) {
          return {Direction::EAST, Direction::SOUTH};
        }
        throw std::runtime_error("Invalid paint combination!");
      }
      if (d == Direction::SOUTH) {
        return {Direction::SOUTH, Direction::EAST};
      }
      if (d == Direction::EAST) {
        return {Direction::WEST, Direction::NORTH};
      }
      throw std::runtime_error("Invalid paint combination!");
    }

    if (p == '7') {
      if (clockwise) {
        if (d == Direction::NORTH) {
          return {Direction::NORTH, Direction::EAST};
        }
        if (d == Direction::EAST) {
          return {Direction::WEST, Direction::SOUTH};
        }
        throw std::runtime_error("Invalid paint combination!");
      }
      if (d == Direction::NORTH) {
        return {Direction::SOUTH, Direction::WEST};
      }
      if (d == Direction::EAST) {
        return {Direction::EAST, Direction::NORTH};
      }
      throw std::runtime_error("Invalid paint combination!");
    }

    if (p == 'F') {
      if (clockwise) {
        if (d == Direction::WEST) {
          return {Direction::NORTH, Direction::WEST};
        }
        if (d == Direction::NORTH) {
          return {Direction::EAST, Direction::SOUTH};
        }
        throw std::runtime_error("Invalid paint combination!");
      }
      if (d == Direction::WEST) {
        return {Direction::SOUTH, Direction::EAST};
      }
      if (d == Direction::NORTH) {
        return {Direction::WEST, Direction::NORTH};
      }
      throw std::runtime_error("Invalid paint combination!");
    }

    throw std::runtime_error("Invalid paint combination!");
  }

  void paint_surrounded(std::vector<std::vector<char>> &grid) {
    auto [clockwise, steps, visited] = analyze_loop(grid);
    auto start = find_start(grid);
    auto current_position = start;
    for (auto s : steps) {
      current_position = current_position + direction_to_offsets(s);
      auto paint_dirs = paint_directions(
          s, grid.at(current_position.first).at(current_position.second),
          clockwise);
      for (auto paint_dir : paint_dirs) {
        auto paint_position =
            current_position + direction_to_offsets(paint_dir);
        while (paint(grid, paint_position, visited)) {
          paint_position = paint_position + direction_to_offsets(paint_dir);
        }
      }
    }
  }

  void part1(const std::string &input) {
    auto grid = parse_input(input);
    auto start = find_start(grid);
    auto steps = traverse_grid(grid);
    std::cout << "Steps: " << steps << std::endl;
    std::cout << "Furthest steps: " << steps / 2 + steps % 2 << std::endl;
  }

  void part2(const std::string &input) {
    auto grid = parse_input(input);

    auto steps = traverse_grid(grid);
    std::cout << "Steps: " << steps << std::endl;
    std::cout << "Furthest steps: " << steps / 2 + steps % 2 << std::endl;
    auto [is_clockwise, d, visited] = analyze_loop(grid);
    std::cout << "Loop clockwise: " << is_clockwise << std::endl;
    paint_surrounded(grid);
    size_t painted = 0;
    for (size_t row = 0; row < grid.size(); ++row) {
      for (size_t col = 0; col < grid.at(row).size(); ++col) {
        std::cout << grid.at(row).at(col);

        if (grid.at(row).at(col) == 'X') {
          ++painted;
        }
      }
      std::cout << std::endl;
    }
    std::cout << "Painted: " << painted << std::endl;
  }
};