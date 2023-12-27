#pragma once

#include "utils.h"
#include <array>
#include <cstddef>
#include <exception>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <unordered_map>

class Day19 {
public:
  struct Instruction {
    char left{};
    char operation{};
    unsigned long long right{};
    std::string target{};
  };

  using Part = std::array<int, 4>;
  using Parts = std::vector<Part>;
  using Workflows = std::unordered_map<std::string, std::vector<Instruction>>;

  const std::unordered_map<char, int> char_to_index{
      {'x', 0}, {'m', 1}, {'a', 2}, {'s', 3}};

  Parts parse_parts(const std::string &parts_string) {
    auto part_lines = split(parts_string, "\n");
    Parts parts;
    Workflows workflows;

    for (const auto &p : part_lines) {
      Part part;
      auto value_strings = split(p, ",");
      for (const auto &v : value_strings) {
        auto key_value = split(v, "=");
        auto key = key_value.at(0).back();
        auto value = stoi(key_value.at(1));
        part.at(char_to_index.at(key)) = value;
      }
      parts.push_back(part);
    }

    return parts;
  }

  Workflows parse_workflows(const std::string &workflow_string) {
    auto workflow_lines = split(workflow_string, "\n");
    Workflows workflows;
    for (const auto &l : workflow_lines) {
      auto tokens = split(l, "{");
      auto name = tokens.at(0);
      auto instruction_strings =
          split({tokens.at(1).begin(), tokens.at(1).end() - 1}, ",");

      std::vector<Instruction> instructions;
      for (const auto &is : instruction_strings) {
        Instruction inst;
        if (is.find_first_of(":") != is.npos) {
          auto instruction_parts = split(is, ":");
          inst.target = instruction_parts.at(1);
          auto pos = instruction_parts.at(0).find_first_of("<>");
          if (pos == std::string::npos) {
            throw std::runtime_error("Invalid instruction");
          }
          inst.operation = instruction_parts.at(0).at(pos);
          inst.left = instruction_parts.at(0).at(pos - 1);
          inst.right = stoi(instruction_parts.at(0).substr(pos + 1));
        } else {
          inst.target = is;
        }
        instructions.push_back(inst);
      }
      workflows[name] = instructions;
    }
    return workflows;
  }

  std::pair<Workflows, Parts> parse_input(const std::string &input) {
    auto tokens = split(input, "\n\n");

    auto workflows = parse_workflows(tokens.at(0));
    auto parts = parse_parts(tokens.at(1));

    return {workflows, parts};
  }

  void part1(const std::string &input) {
    auto [workflows, parts] = parse_input(input);
    size_t accepted = 0;
    size_t sum = 0;
    for (const auto &p : parts) {
      std::string current = "in";
      while (true) {
        auto instructions = workflows.at(current);
        for (const auto &i : instructions) {
          if (i.operation == 0) {
            current = i.target;
            break;
          }
          if (i.operation == '>' && p.at(char_to_index.at(i.left)) > i.right) {
            current = i.target;
            break;
          }
          if (i.operation == '<' && p.at(char_to_index.at(i.left)) < i.right) {
            current = i.target;
            break;
          }
        }
        if (current == "A") {
          for (int i = 0; i < 4; ++i) {
            sum += p[i];
          }
          ++accepted;
          break;
        }
        if (current == "R") {
          break;
        }
      }
    }
    std::cout << "Accepted: " << sum << std::endl;
  }

  unsigned long long count_combinations(
      const std::string &current, int instruction, const Workflows &workflows,
      const std::array<std::pair<unsigned long long, unsigned long long>, 4>
          &limits) {
    if (current == "R") {
      return 0;
    }

    unsigned long long combos = 1;
    if (current == "A") {
      for (int i = 0; i < 4; ++i) {
        combos *= (limits.at(i).second - limits.at(i).first) + 1;
      }
      return combos;
    }
    combos = 0;
    auto i = workflows.at(current).at(instruction);
    if (i.operation == 0) {
      combos += count_combinations(i.target, 0, workflows, limits);
    }
    if (i.operation == '>') {
      if (limits.at(char_to_index.at(i.left)).second > i.right) {
        auto new_limits = limits;
        if (new_limits.at(char_to_index.at(i.left)).first < i.right + 1) {
          new_limits.at(char_to_index.at(i.left)).first = i.right + 1;
        }
        combos += count_combinations(i.target, 0, workflows, new_limits);
      }
      if (limits.at(char_to_index.at(i.left)).first <= i.right) {
        auto new_limits = limits;
        if (new_limits.at(char_to_index.at(i.left)).second > i.right) {
          new_limits.at(char_to_index.at(i.left)).second = i.right;
        }
        combos +=
            count_combinations(current, instruction + 1, workflows, new_limits);
      }
    }
    if (i.operation == '<') {
      if (limits.at(char_to_index.at(i.left)).first < i.right) {
        auto new_limits = limits;
        if (new_limits.at(char_to_index.at(i.left)).second > i.right - 1) {
          new_limits.at(char_to_index.at(i.left)).second = i.right - 1;
        }
        combos += count_combinations(i.target, 0, workflows, new_limits);
      }
      if (limits.at(char_to_index.at(i.left)).second >= i.right) {
        auto new_limits = limits;
        if (new_limits.at(char_to_index.at(i.left)).first < i.right) {
          new_limits.at(char_to_index.at(i.left)).first = i.right;
        }
        combos +=
            count_combinations(current, instruction + 1, workflows, new_limits);
      }
    }
    return combos;
  }

  void part2(const std::string &input) {
    auto [workflows, _] = parse_input(input);
    std::string current = "in";

    std::array<std::pair<unsigned long long, unsigned long long>, 4> limits{
        {{1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}}};
    unsigned long long accepted =
        count_combinations(current, 0, workflows, limits);

    std::cout << "Accepted: " << accepted << std::endl;
  }
};