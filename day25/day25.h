#pragma once

#include "utils.h"
#include <cstddef>
#include <functional>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

class Day25 {
public:
  struct Node {
    std::string name;
    int traffic;
    std::vector<std::string> edges;
  };

  std::unordered_map<std::string, Node> nodes;

  std::optional<std::vector<std::string>>
  find_path(const std::string &first, const std::string &second,
            const std::set<std::pair<std::string, std::string>> &avoiding) {
    if (nodes.find(first) == nodes.end() || nodes.find(second) == nodes.end()) {
      throw std::runtime_error("Can't happen!");
    }

    std::queue<std::pair<std::string, std::vector<std::string>>> queue;
    std::unordered_set<std::string> visited;
    queue.push({first, {}});

    while (!queue.empty()) {
      auto [current, path] = queue.front();
      if (current == second) {
        return queue.front().second;
      }
      queue.pop();
      visited.insert(current);

      for (const auto &e : nodes.at(current).edges) {
        if (visited.find(e) == visited.end() &&
            avoiding.find({e, current}) == avoiding.end() &&
            avoiding.find({current, e}) == avoiding.end()) {
          auto new_path = path;
          new_path.push_back(e);
          queue.push({e, new_path});
        }
      }
    }
    return std::nullopt;
  }

  size_t alternate_paths(const std::string &first, const std::string &second) {
    std::set<std::pair<std::string, std::string>> avoiding;
    avoiding.insert({first, second});
    size_t paths = 0;
    auto first_path = find_path(first, second, avoiding);
    if (first_path.has_value()) {
      for (int i = 0; i < first_path->size() - 1; i += 1) {
        avoiding.insert({first_path->at(i), first_path->at(i + 1)});
      }
    }
    auto second_path = find_path(first, second, avoiding);
    if (second_path.has_value()) {
      for (int i = 0; i < second_path->size() - 1; i += 1) {
        avoiding.insert({second_path->at(i), second_path->at(i + 1)});
      }
    }
    auto third_path = find_path(first, second, avoiding);
    if (!third_path.has_value()) {
      return 2;
    }
    return 3;
  }

  size_t count_connected(
      const std::string &node,
      const std::set<std::pair<std::string, std::string>> &snipped) {
    std::unordered_set<std::string> connected;

    std::queue<std::string> queue;
    queue.push(node);
    while (!queue.empty()) {
      auto current = queue.front();
      queue.pop();
      connected.insert(current);

      for (auto e : nodes.at(current).edges) {
        if (connected.find(e) == connected.end() &&
            snipped.find({e, current}) == snipped.end() &&
            snipped.find({current, e}) == snipped.end()) {
          queue.push(e);
        }
      }
    }

    return connected.size();
  }

  size_t solve(const std::set<std::pair<std::string, std::string>> &snipped) {
    std::unordered_set<size_t> sizes;
    for (auto e : snipped) {
      sizes.insert(count_connected(e.first, snipped));
      sizes.insert(count_connected(e.second, snipped));
    }

    if (sizes.size() != 2) {
      throw std::runtime_error("FUCK YOU!");
    }
    size_t sum = 1;
    for (auto s : sizes) {
      sum *= s;
    }
    return sum;
  }

  void part1(const std::string &input) {
    auto lines = split(input, "\n");

    for (const auto &line : lines) {
      auto name = split(line, ":").at(0);
      auto connected = split(trim(split(line, ":").at(1)), " ");
      trim(name);
      if (nodes.find(name) == nodes.end()) {
        nodes[name] = Node{name, 0, {}};
      }
      for (auto c : connected) {
        trim(c);
        nodes[name].edges.push_back(c);
        if (nodes.find(c) == nodes.end()) {
          nodes[c] = Node{c, 0, {name}};
        } else {
          nodes[c].edges.push_back(name);
        }
      }
    }

    std::set<std::pair<std::string, std::string>> snipped;
    for (auto n : nodes) {
      for (auto e : n.second.edges) {
        auto i = alternate_paths(n.first, e);

        if (i <= 2) {
          snipped.insert({n.first, e});
        }
      }
    }

    std::cout << "Total: " << solve(snipped) << std::endl;
  }
};