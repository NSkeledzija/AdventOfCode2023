#pragma once
#include "utils.h"
#include <cstddef>
#include <iostream>
#include <limits>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class Day2 {
public:
  static constexpr char EXAMPLE[] =
      "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\nGame 2: 1 blue, "
      "2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue\nGame 3: 8 green, 6 "
      "blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red\nGame 4: 1 green, "
      "3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red\nGame 5: 6 red, "
      "1 blue, 3 green; 2 blue, 1 red, 2 green";

  struct Round {
    size_t red{};
    size_t green{};
    size_t blue{};
  };

  struct Game {
    std::vector<Round> rounds;
  };

  Round parse_round(const std::string &input) {
    std::string trimmed = input;
    trim(trimmed);
    auto colors = split(trimmed, ",");
    Round round;
    for (auto &c : colors) {
      trim(c);
      auto values = split(c, " ");
      size_t count = atoi(values[0].c_str());

      if (values[1] == "red") {
        round.red = count;
      } else if (values[1] == "green") {
        round.green = count;
      } else if (values[1] == "blue") {
        round.blue = count;
      } else {
        throw std::runtime_error("Wrong color name!");
      }
    }
    return round;
  }

  Game parse_game(const std::string &input) {
    auto tokens = split(input, ":");
    auto data = tokens[1];

    auto round_strings = split(data, ";");
    Game game;
    for (const auto &r : round_strings) {
      game.rounds.push_back(parse_round(r));
    }
    return game;
  }

  std::vector<Game> parse_games(const std::string &input) {
    std::stringstream ss(input);
    std::string line;
    std::vector<Game> games;
    while (std::getline(ss, line)) {
      games.push_back(parse_game(line));
    }
    return games;
  }

  bool is_game_possible(const Game &game) {
    const size_t RED_CUBES = 12;
    const size_t GREEN_CUBES = 13;
    const size_t BLUE_CUBES = 14;

    for (const auto &round : game.rounds) {
      if (round.red > RED_CUBES) {
        return false;
      }
      if (round.green > GREEN_CUBES) {
        return false;
      }
      if (round.blue > BLUE_CUBES) {
        return false;
      }
    }
    return true;
  }

  size_t power_of_game(const Game &game) {
    size_t max_red = std::numeric_limits<size_t>::min();
    size_t max_green = std::numeric_limits<size_t>::min();
    size_t max_blue = std::numeric_limits<size_t>::min();
    for (const auto &round : game.rounds) {
      max_red = std::max(max_red, round.red);
      max_green = std::max(max_green, round.green);
      max_blue = std::max(max_blue, round.blue);
    }
    return max_red * max_blue * max_green;
  }

  void part1(const std::string &input) {
    auto games = parse_games(input);
    size_t game_id = 1;
    size_t sum = 0;
    for (const auto &g : games) {
      if (is_game_possible(g)) {
        sum += game_id;
      }
      ++game_id;
    }

    std::cout << "Possible game sum: " << sum << std::endl;
  }

  void part2(const std::string &input) {
    auto games = parse_games(input);
    size_t sum = 0;
    for (const auto &g : games) {
      auto p = power_of_game(g);
      sum += p;
    }
    std::cout << "Power of all games: " << sum << std::endl;
  }
};