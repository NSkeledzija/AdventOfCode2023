#pragma once

#include "utils.h"
#include <cmath>
#include <cstddef>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>

class Day23 {
public:
  struct Path {
    position p;
    int distance;
    std::set<position> points;

    bool operator<(const Path &other) const {
      return distance < other.distance;
    }
  };

  void part1(const std::string &input) {
    auto grid = split(input, "\n");
    const position start = {0, 1};
    const position end = {grid.size() - 1, grid.at(0).size() - 2};

    std::deque<Path> queue;

    queue.push_back(Path{start, 0, std::set<position>{start}});
    const size_t rows = grid.size();
    const size_t cols = grid.at(0).size();
    const std::unordered_map<char, Direction> map_arrow = {
        {'^', Direction::NORTH},
        {'>', Direction::EAST},
        {'v', Direction::SOUTH},
        {'<', Direction::WEST}};

    int max_distance = 0;

    while (!queue.empty()) {
      auto current = queue.front();
      auto cc = grid.at(current.p.first).at(current.p.second);
      queue.pop_front();

      if (current.p == end) {
        std::cout << "Total: " << current.distance << std::endl;
        max_distance = std::max(max_distance, current.distance);
      }

      if (cc == '.') {
        auto directions = {Direction::NORTH, Direction::EAST, Direction::SOUTH,
                           Direction::WEST};
        for (auto d : directions) {
          auto np = current.p + d;
          if (current.points.find(np) == current.points.end() &&
              position_in_grid(np, rows, cols) && cc != '#') {
            auto new_points = current.points;
            new_points.insert(np);
            queue.push_back(Path{np, current.distance + 1, new_points});
          }
        }
      }

      if (map_arrow.find(cc) != map_arrow.end()) {
        auto d = map_arrow.at(cc);
        auto np = current.p + d;
        if (current.points.find(np) == current.points.end() &&
            position_in_grid(np, rows, cols) && cc != '#') {
          auto new_points = current.points;
          new_points.insert(np);
          queue.push_back(Path{np, current.distance + 1, new_points});
        }
      }
    }

    std::cout << "Max: " << max_distance << std::endl;
  }

  void part2(const std::string &input) {
    auto grid = split(input, "\n");
    const position start = {0, 1};
    const position end = {grid.size() - 1, grid.at(0).size() - 2};

    std::deque<Path> queue;

    queue.push_back(Path{start, 0, std::set<position>{start}});
    const size_t rows = grid.size();
    const size_t cols = grid.at(0).size();
    const std::unordered_map<char, Direction> map_arrow = {
        {'^', Direction::NORTH},
        {'>', Direction::EAST},
        {'v', Direction::SOUTH},
        {'<', Direction::WEST}};

    int max_distance = 0;

    while (!queue.empty()) {
      auto current = queue.front();
      auto cc = grid.at(current.p.first).at(current.p.second);
      queue.pop_front();

      if (current.p == end) {
        std::cout << "Total: " << current.distance << std::endl;
        max_distance = std::max(max_distance, current.distance);
      }

      if (cc == '.' || map_arrow.find(cc) != map_arrow.end()) {
        auto directions = {Direction::NORTH, Direction::EAST, Direction::SOUTH,
                           Direction::WEST};
        for (auto d : directions) {
          auto np = current.p + d;
          if (current.points.find(np) == current.points.end() &&
              position_in_grid(np, rows, cols) && cc != '#') {
            auto new_points = current.points;
            new_points.insert(np);
            queue.push_back(Path{np, current.distance + 1, new_points});
          }
        }
      }
    }
    std::cout << "Max: " << max_distance << std::endl;
  }
};