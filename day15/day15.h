#pragma once

#include "utils.h"
#include <array>
#include <chrono>
#include <cstddef>
#include <deque>
#include <iostream>
#include <list>
#include <string>

using sc = std::chrono::steady_clock;
class Day15 {
public:
  struct Lens {
    Lens(const std::string &label, int len) : label(label), len(len) {}
    int hash() const {
      int code = 0;
      for (auto c : label) {
        code += (int)c;
        code *= 17;
        code %= 256;
      }
      return code;
    }
    std::string label;
    int len;
  };

  class Lens_hash_map {
  public:
    void insert(const Lens &lens) {
      auto hash = lens.hash();
      for (auto &l : boxes.at(lens.hash())) {
        if (l.label == lens.label) {
          l.len = lens.len;
          return;
        }
      }
      boxes.at(hash).push_back(lens);
      return;
    }

    void remove(const Lens &lens) {
      auto hash = lens.hash();
      auto &box = boxes.at(hash);
      box.erase(std::remove_if(
                    box.begin(), box.end(),
                    [lens](const Lens &l) { return lens.label == l.label; }),
                box.end());
    }

    long long power() {
      long long power = 0;
      for (int box = 0; box < 256; ++box) {
        auto tmp = boxes.at(box);
        for (int slot = boxes.at(box).size() - 1; slot >= 0; --slot) {
          power += (box + 1) * (slot + 1) * tmp.at(slot).len;
        }
      }
      return power;
    }

    std::array<std::vector<Lens>, 256> boxes;
  };

  void part1(const std::string &input) {
    auto sequences = split(input, ",");
    long long sum = 0;
    for (const auto &seq : sequences) {
      Lens l(seq, 0);
      sum += l.hash();
    }
    std::cout << "Total = " << sum << std::endl;
  }

  void part2(const std::string &input) {
    auto sequences = split(input, ",");
    Lens_hash_map map;
    for (const auto &seq : sequences) {
      if (seq.find("=") != seq.npos) {
        auto tokens = split(seq, "=");
        map.insert({tokens.at(0), stoi(tokens.at(1))});
      }
      if (seq.find("-") != seq.npos) {
        auto tokens = split(seq, "-");
        map.remove({tokens.at(0), 0});
      }
    }
    std::cout << "Total = " << map.power() << std::endl;
  }

  std::pair<Lens, bool> random_operation() {
    auto randchar = []() -> char {
      const char charset[] = "abcdefghijklmnopqrstuvwxyz";
      const size_t max_index = (sizeof(charset) - 1);
      return charset[rand() % max_index];
    };

    auto label_length = 1 + (rand() % 10);
    std::string str(label_length, 0);
    std::generate_n(str.begin(), label_length, randchar);

    bool addition = (rand() % 2);
    auto focal_length = 1 + (rand() % 10);

    return {Lens{str, focal_length}, addition};
  }
};