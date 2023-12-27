#pragma once

#include "utils.h"
#include <cmath>
#include <cstddef>
#include <iostream>
#include <regex>
#include <sstream>
#include <streambuf>
#include <string>
#include <unordered_set>

class Day4 {
public:
  class Card {
  public:
    size_t id;
    size_t winning_numbers;

    size_t score() const {
      if (winning_numbers > 0) {
        return static_cast<size_t>(round(pow(2, winning_numbers - 1)));
      }
      return 0;
    }
  };

  Card parse_card(const std::string &line) {
    auto tokens = split(line, ":");
    auto id_str = split(tokens.at(0)).at(1);
    tokens = split(tokens.at(1), "|");
    auto left = tokens.at(0);
    auto right = tokens.at(1);
    trim(left);
    trim(right);
    auto winning = split(left);
    auto played = split(right);
    std::unordered_set<int> winning_numbers;
    for (const auto &w : winning) {
      winning_numbers.insert(stoi(w));
    }
    size_t count = 0;
    for (const auto &p : played) {
      auto current = stoi(p);
      if (winning_numbers.find(current) != winning_numbers.end()) {
        ++count;
      }
    }
    return {static_cast<size_t>(stoi(id_str)), count};
  }

  std::vector<Card> parse_cards(const std::string &input) {
    std::stringstream ss(input);
    std::string line;
    size_t score = 0;
    std::vector<Card> output;
    while (std::getline(ss, line)) {
      output.push_back(parse_card(line));
    }
    return output;
  }

  void part1(const std::string &input) {
    size_t score = 0;
    for (const auto &c : parse_cards(input)) {
      score += c.score();
    }
    std::cout << "Total score: " << score << std::endl;
  }

  void part2(const std::string &input) {
    auto cards = parse_cards(input);
    std::vector<std::pair<Card, size_t>> total_cards;
    for (const auto &c : cards) {
      total_cards.push_back({c, 1});
    }

    for (auto &[card, count] : total_cards) {
      for (size_t copy = 0; copy < card.winning_numbers; ++copy) {
        size_t index = card.id + copy;
        total_cards[index].second += count;
      }
    }

    size_t total_count = 0;
    for (auto &[_, count] : total_cards) {
      total_count += count;
    }
    std::cout << "Total count: " << total_count << std::endl;
  }
};