#pragma once
#include "utils.h"
#include <algorithm>
#include <cstddef>
#include <deque>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

class Day22 {
public:
  struct Brick {
    int x1;
    int y1;
    int z1;
    int x2;
    int y2;
    int z2;
    int id;

    bool operator<(const Brick &other) { return z1 < other.z1; }

    bool operator==(const Brick &other) const {
      return x1 == other.x1 && x2 == other.x2 && y1 == other.y1 &&
             y2 == other.y2 && z1 == other.z1 && z2 == other.z2;
    }

    bool operator!=(const Brick &other) const { return !(*this == other); }
  };

  using Heightmap = std::vector<std::vector<std::vector<int>>>;

  std::pair<std::vector<Brick>, Heightmap>
  parse_input(const std::string &input) {
    auto lines = split(input, "\n");
    std::vector<Brick> bricks;
    int max_x = 0;
    int max_y = 0;
    int id = 0;
    for (const auto &l : lines) {
      auto parts = split(l, "~");
      auto start = split(parts.at(0), ",");
      auto end = split(parts.at(1), ",");
      Brick b;
      b.x1 = stoi(start.at(0));
      b.y1 = stoi(start.at(1));
      b.z1 = stoi(start.at(2));
      max_x = std::max(max_x, b.x1);
      max_y = std::max(max_y, b.y1);
      b.x2 = stoi(end.at(0));
      b.y2 = stoi(end.at(1));
      b.z2 = stoi(end.at(2));
      max_x = std::max(max_x, b.x2);
      max_y = std::max(max_y, b.y2);
      b.id = id++;
      bricks.push_back(b);
    }

    std::vector<std::vector<int>> row(max_y + 1, std::vector<int>());
    Heightmap heightmap(max_x + 1, row);

    return {bricks, heightmap};
  }

  void settle_bricks(std::vector<Brick> &bricks, Heightmap &heightmap) {
    std::sort(bricks.begin(), bricks.end());
    for (int x = 0; x < heightmap.size(); ++x) {
      for (int y = 0; y < heightmap.at(0).size(); ++y) {
        heightmap.at(x).at(y).clear();
      }
    }
    for (int brick_index = 0; brick_index < bricks.size(); ++brick_index) {
      auto &b = bricks.at(brick_index);
      int landing_height = 0;
      for (int x = b.x1; x <= b.x2; ++x) {
        for (int y = b.y1; y <= b.y2; ++y) {
          landing_height =
              std::max(landing_height, (int)heightmap.at(x).at(y).size());
        }
      }

      auto height = (b.z2 - b.z1);
      b.z1 = landing_height + 1;
      b.z2 = b.z1 + height;

      for (int x = b.x1; x <= b.x2; ++x) {
        for (int y = b.y1; y <= b.y2; ++y) {
          while (heightmap.at(x).at(y).size() < landing_height) {
            heightmap.at(x).at(y).push_back(-1);
          }
          for (int z = b.z1; z <= b.z2; ++z) {
            heightmap.at(x).at(y).push_back(brick_index);
          }
        }
      }
    }
  }

  std::vector<int> find_supported(int brick_index,
                                  const std::vector<Brick> &bricks,
                                  const Heightmap &heightmap) {
    auto b = bricks.at(brick_index);
    std::vector<int> supported;
    for (int x = b.x1; x <= b.x2; ++x) {
      for (int y = b.y1; y <= b.y2; ++y) {
        if (b.z2 < heightmap.at(x).at(y).size() &&
            heightmap.at(x).at(y).at(b.z2) != -1) {
          supported.push_back(heightmap.at(x).at(y).at(b.z2));
        }
      }
    }
    return supported;
  }

  std::unordered_set<int> find_supports(int brick_index,
                                        const std::vector<Brick> &bricks,
                                        const Heightmap &heightmap) {
    auto b = bricks.at(brick_index);
    std::unordered_set<int> supports;
    for (int x = b.x1; x <= b.x2; ++x) {
      for (int y = b.y1; y <= b.y2; ++y) {
        if (b.z1 - 2 >= 0 && heightmap.at(x).at(y).at(b.z1 - 2) != -1) {
          supports.insert(heightmap.at(x).at(y).at(b.z1 - 2));
        }
      }
    }
    return supports;
  }

  std::unordered_set<int> find_fallen(int brick_index,
                                      const std::vector<Brick> &bricks,
                                      const Heightmap &heightmap) {
    std::deque<int> queue;
    std::unordered_set<int> fallen;
    fallen.insert(brick_index);
    queue.push_back(brick_index);

    while (!queue.empty()) {
      auto current = queue.front();
      queue.pop_front();

      auto supported = find_supported(current, bricks, heightmap);
      for (auto sp : supported) {
        auto supports = find_supports(sp, bricks, heightmap);
        auto should_fall = true;
        for (auto s : supports) {
          if (fallen.find(s) == fallen.end()) {
            should_fall = false;
            break;
          }
        }
        if (should_fall && fallen.find(sp) == fallen.end()) {
          fallen.insert(sp);
          queue.push_back(sp);
        }
      }
    }
    return fallen;
  }

  void part1(const std::string &input) {
    auto [bricks, heightmap] = parse_input(input);
    settle_bricks(bricks, heightmap);
    size_t count = 0;
    for (int brick_index = 0; brick_index < bricks.size(); ++brick_index) {
      if (find_fallen(brick_index, bricks, heightmap).size() == 1) {
        ++count;
      }
    }
    std::cout << "Total: " << count << std::endl;
  }

  void part2(const std::string &input) {
    auto [bricks, heightmap] = parse_input(input);
    settle_bricks(bricks, heightmap);

    size_t fallen_count = 0;
    for (int brick_index = 0; brick_index < bricks.size(); ++brick_index) {
      fallen_count += find_fallen(brick_index, bricks, heightmap).size() - 1;
    }
    std::cout << "Total: " << fallen_count << std::endl;
  }
};