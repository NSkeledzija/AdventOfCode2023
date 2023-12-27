#pragma once

#include "utils.h"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <math.h>
#include <sys/types.h>

class Day6 {
public:
  struct Race {
    uint64_t time{};
    uint64_t record{};
  };

  bool parse_input(std::vector<Race> &races, const std::string &input) {
    auto lines = split(input, "\n");
    auto time_line = lines[0];
    auto record_line = lines[1];

    auto time_strings = split(time_line);
    time_strings = {time_strings.begin() + 1, time_strings.end()};

    auto record_strings = split(record_line);
    record_strings = {record_strings.begin() + 1, record_strings.end()};

    assert(time_strings.size() == record_strings.size());
    for (size_t i = 0; i < time_strings.size(); ++i) {
      races.push_back({stoull(time_strings[i]), stoull(record_strings[i])});
    }
    return true;
  }

  bool parse_input_as_single_race(Race &race, const std::string &input) {
    auto lines = split(input, "\n");
    auto time_line = lines[0];
    auto record_line = lines[1];

    auto time_strings = split(time_line);
    time_strings = {time_strings.begin() + 1, time_strings.end()};

    auto record_strings = split(record_line);
    record_strings = {record_strings.begin() + 1, record_strings.end()};

    assert(time_strings.size() == record_strings.size());
    std::string full_time;
    std::string full_record;
    for (size_t i = 0; i < time_strings.size(); ++i) {
      full_time += time_strings[i];
      full_record += record_strings[i];
    }
    race.time = stoull(full_time);
    race.record = stoull(full_record);
    return true;
  }

  size_t win_count(const Race &race) {
    double a = 1;
    double b = -(double)race.time;
    double c = race.record + 1;
    double s1 = (-b - std::sqrt(std::pow(b, 2) - 4 * c)) / 2;
    double s2 = (-b + std::sqrt(std::pow(b, 2) - 4 * c)) / 2;

    int s1i = std::round(ceil(s1));
    int s2i = std::round(floor(s2));

    return s2i - s1i + 1;
  }

  bool part1(const std::string &input) {
    std::vector<Race> races;
    parse_input(races, input);
    size_t total = 1;
    for (const auto &r : races) {
      total *= win_count(r);
    }
    std::cout << "Total wins: " << total << std::endl;
    return true;
  }

  bool part2(const std::string &input) {
    Race race;
    parse_input_as_single_race(race, input);
    auto total = win_count(race);
    std::cout << "Total wins: " << total << std::endl;
    return true;
  }
};
