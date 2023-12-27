#pragma once

#include "utils.h"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

class Day7 {
public:
  struct Hand {
    Hand(const std::string &cards, size_t bid) : cards(cards), bid(bid) {}
    std::string cards;
    size_t bid;
  };

  enum class Hand_rank {
    FIVE = 6,
    POKER = 5,
    FULL_HOUSE = 4,
    THREE = 3,
    TWO_PAIR = 2,
    PAIR = 1,
    HIGH_CARD = 0
  };

  size_t card_value(char c) {
    if (c == '2') {
      return 0;
    }
    if (c == '3') {
      return 1;
    }
    if (c == '4') {
      return 2;
    }
    if (c == '5') {
      return 3;
    }
    if (c == '6') {
      return 4;
    }
    if (c == '7') {
      return 5;
    }
    if (c == '8') {
      return 6;
    }
    if (c == '9') {
      return 7;
    }
    if (c == 'T') {
      return 8;
    }
    if (c == 'J') {
      return 9;
    }
    if (c == 'Q') {
      return 10;
    }
    if (c == 'K') {
      return 11;
    }
    if (c == 'A') {
      return 12;
    }
    throw std::runtime_error("Wrong card!");
  }

  size_t card_value_with_joker(char c) {
    if (c == 'J') {
      return 0;
    }
    if (c == '2') {
      return 1;
    }
    if (c == '3') {
      return 2;
    }
    if (c == '4') {
      return 3;
    }
    if (c == '5') {
      return 4;
    }
    if (c == '6') {
      return 5;
    }
    if (c == '7') {
      return 6;
    }
    if (c == '8') {
      return 7;
    }
    if (c == '9') {
      return 8;
    }
    if (c == 'T') {
      return 9;
    }
    if (c == 'Q') {
      return 10;
    }
    if (c == 'K') {
      return 11;
    }
    if (c == 'A') {
      return 12;
    }
    throw std::runtime_error("Wrong card!");
  }

  std::unordered_map<char, size_t> count(const Hand &h) {
    std::unordered_map<char, size_t> card_map;
    for (auto c : h.cards) {
      if (card_map.find(c) != card_map.end()) {
        ++card_map[c];
      } else {
        card_map[c] = 1;
      }
    }
    return card_map;
  }

  Hand_rank rank(const Hand &h) {
    auto cm = count(h);
    if (cm.size() == 1) {
      return Hand_rank::FIVE;
    }
    if (cm.size() == 2) {
      auto [type, count] = *cm.begin();
      if (count == 1 || count == 4) {
        return Hand_rank::POKER;
      }
      return Hand_rank::FULL_HOUSE;
    }

    size_t pairs = 0;
    if (cm.size() == 3) {
      for (const auto &[type, count] : cm) {
        if (count == 3) {
          return Hand_rank::THREE;
        }
      }
      return Hand_rank::TWO_PAIR;
    }

    if (cm.size() == 4) {
      return Hand_rank::PAIR;
    }
    return Hand_rank::HIGH_CARD;
  }

  Hand_rank rank_with_joker(const Hand &h) {
    auto cm = count(h);
    size_t joker_count = cm.find('J') != cm.end() ? cm['J'] : 0;
    if (cm.size() == 1 || (cm.size() == 2 && joker_count > 0)) {
      return Hand_rank::FIVE;
    }
    if (cm.size() == 2 || (cm.size() == 3 && joker_count > 0)) {
      auto it = cm.begin();
      if (it->first == 'J') {
        ++it;
      }
      auto [type, count] = *it;
      if (count == 1 || count == 4 || count + joker_count >= 4) {
        return Hand_rank::POKER;
      }
      return Hand_rank::FULL_HOUSE;
    }

    size_t pairs = 0;
    if (cm.size() == 3 || (cm.size() == 4 && joker_count > 0)) {
      for (const auto &[type, count] : cm) {
        if (type == 'J') {
          continue;
        }
        if (count == 3 || count + joker_count >= 3) {
          return Hand_rank::THREE;
        }
      }
      return Hand_rank::TWO_PAIR;
    }

    if (cm.size() == 4 || cm.size() == 5 && joker_count > 0) {
      return Hand_rank::PAIR;
    }
    return Hand_rank::HIGH_CARD;
  }

  bool compare(const Hand &left, const Hand &right) {
    Hand_rank lr = rank(left);
    Hand_rank rr = rank(right);

    if (lr == rr) {
      for (size_t index = 0; index < left.cards.size(); ++index) {
        if (card_value(left.cards[index]) < card_value(right.cards[index])) {
          return true;
        } else if (card_value(left.cards[index]) >
                   card_value(right.cards[index])) {
          return false;
        }
      }
      throw std::runtime_error("TOtally equal cards wtf!");
    }

    return lr < rr;
  }

  bool compare_with_joker(const Hand &left, const Hand &right) {
    Hand_rank lr = rank_with_joker(left);
    Hand_rank rr = rank_with_joker(right);

    if (lr == rr) {
      for (size_t index = 0; index < left.cards.size(); ++index) {
        auto leftval = card_value_with_joker(left.cards[index]);
        auto rightval = card_value_with_joker(right.cards[index]);
        if (leftval < rightval) {
          return true;
        } else if (leftval > rightval) {
          return false;
        }
      }
      throw std::runtime_error("TOtally equal cards wtf!");
    }

    return lr < rr;
  }

  std::vector<Hand> parse_input(const std::string &input) {
    auto tokens = split(input, "\n");
    std::vector<Hand> out;

    for (const auto &t : tokens) {
      auto inner_tokens = split(t);
      auto cards = inner_tokens[0];
      auto bid_string = inner_tokens[1];
      trim(cards);
      trim(bid_string);

      out.emplace_back(cards, stoi(bid_string));
    }
    return out;
  }

  void part1(const std::string &input) {
    auto hands = parse_input(input);
    std::sort(hands.begin(), hands.end(),
              std::bind(&Day7::compare, this, std::placeholders::_1,
                        std::placeholders::_2));

    size_t total = 0;
    size_t rank = 1;
    for (const auto &h : hands) {
      std::cout << h.cards << " " << h.bid * rank << std::endl;
      total += rank * h.bid;
      ++rank;
    }
    std::cout << "Total: " << total << std::endl;
  }

  void part2(const std::string &input) {
    auto hands = parse_input(input);
    std::sort(hands.begin(), hands.end(),
              std::bind(&Day7::compare_with_joker, this, std::placeholders::_1,
                        std::placeholders::_2));

    unsigned long long total = 0;
    unsigned long long rank = 1;
    for (const auto &h : hands) {
      std::cout << h.cards << " - bid: " << h.bid << " * " << rank << " "
                << h.bid * rank << std::endl;
      total += rank * h.bid;
      ++rank;
    }
    std::cout << "Total: " << total << std::endl;
  }
};