#pragma once
#include "utils.h"
#include <cmath>
#include <cstddef>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <unordered_map>
#include <utility>

class Day8 {
public:
  using Nodes =
      std::unordered_map<std::string, std::pair<std::string, std::string>>;
  void parse_input(const std::string &input, std::string &directions,
                   Nodes &nodes) {
    auto tokens = split(input, "\n\n");
    directions = tokens.at(0);
    trim(directions);
    auto node_lines = split(tokens.at(1), "\n");
    for (const auto &m : node_lines) {
      std::regex rgx("([A-Z]+) = \\(([A-Z]+), ([A-Z]+)\\)");
      std::smatch matches;
      if (!std::regex_search(m, matches, rgx)) {
        throw std::runtime_error("Invalid input!");
      }

      nodes[matches[1].str()] = {matches[2].str(), matches[3].str()};
    }
  }

  void part1(const std::string &input) {
    std::string directions;
    Nodes nodes;
    parse_input(input, directions, nodes);
    std::cout << directions << std::endl;
    std::string node = "AAA";
    size_t direction_index = 0;
    size_t total_steps = 0;
    do {
      if (directions[direction_index] == 'R') {
        node = nodes.at(node).second;
      } else if (directions[direction_index] == 'L') {
        node = nodes.at(node).first;
      } else {
        throw std::runtime_error("Invalid input!");
      }
      ++total_steps;
      direction_index =
          direction_index == directions.size() - 1 ? 0 : direction_index + 1;
    } while (node != "ZZZ");
    std::cout << "Total: " << total_steps << std::endl;
  }

  std::vector<std::string> get_all_nodes_that_end_with(const Nodes &nodes,
                                                       char end) {
    std::vector<std::string> out;
    for (const auto &n : nodes) {
      if (n.first.at(n.first.size() - 1) == end) {
        out.push_back(n.first);
      }
    }
    return out;
  }

  bool all_node_end_with(const std::vector<std::string> &nodes, char end) {
    std::vector<std::string> out;
    for (const auto &n : nodes) {
      if (n.at(n.size() - 1) != end) {
        return false;
      }
    }
    return true;
  }

  std::pair<size_t, size_t>
  find_period_and_Zoffset(const std::string &node, const Nodes &nodes,
                          const std::string &directions) {
    size_t direction_index = 0;
    size_t total_steps = 0;
    std::string current = node;
    size_t z_offset = 0;
    std::string first_z_node = "";
    do {
      if (directions[direction_index] == 'R') {
        current = nodes.at(current).second;
      } else if (directions[direction_index] == 'L') {
        current = nodes.at(current).first;
      } else {
        throw std::runtime_error("Invalid input!");
      }
      ++total_steps;
      if (current.at(current.size() - 1) == 'Z') {
        if (first_z_node.empty()) {
          z_offset = total_steps;
          first_z_node = current;
        } else {
          return {total_steps - z_offset, z_offset};
        }
      }
      direction_index =
          direction_index == directions.size() - 1 ? 0 : direction_index + 1;
    } while (true);
  }

  size_t gcd(size_t a, size_t b) {
    size_t bound = std::min(a, b);
    for (size_t x = bound; x > 1; --x) {
      if (a % x == 0 && b % x == 0) {
        return x;
      }
    }
    return 1;
  }

  size_t lcm(size_t a, size_t b) { return a * b / gcd(a, b); }

  void part2(const std::string &input) {
    std::string directions;
    std::unordered_map<std::string, std::pair<std::string, std::string>> nodes;
    parse_input(input, directions, nodes);
    auto starting_nodes = get_all_nodes_that_end_with(nodes, 'A');
    size_t lowest_period;
    for (size_t n_index = 0; n_index < starting_nodes.size(); ++n_index) {
      auto n = starting_nodes[n_index];
      auto [period, zoffset] = find_period_and_Zoffset(n, nodes, directions);
      std::cout << n << " = " << period << ", " << zoffset << std::endl;
      if (n_index == 0) {
        lowest_period = period;
      } else {
        lowest_period = lcm(lowest_period, period);
      }
    }
    std::cout << lowest_period << std::endl;
  }
};