#pragma once

#include <array>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <sys/types.h>

class Day1 {
public:
  bool part1(const std::string &data) {
    std::stringstream ss{data};
    std::string line;
    long long sum = 0;
    while (std::getline(ss, line, '\n')) {
      int first_digit = -1;
      int second_digit = -1;
      for (auto c : line) {
        if (std::isdigit(c)) {
          if (first_digit == -1) {
            first_digit = c - '0';
          }
          second_digit = c - '0';
        }
      }
      if (first_digit == -1 || second_digit == -1) {
        std::cout << "Didn't find a digit!";
        return false;
      }

      auto calibration = first_digit * 10 + second_digit;
      std::cout << "Calibration at line " << line << ": " << calibration
                << std::endl;
      sum += calibration;
    }
    std::cout << "Calibration sum: " << sum << std::endl;
    return true;
  }

  bool is_word_at_index(const std::string &line, size_t index,
                        const std::string &word) {
    if (index > line.size() - 1) {
      std::cout << "Invalid index provided to string of length: " << line.size()
                << ", index: " << index << std::endl;
    }
    if (line.size() - index < word.size()) {
      return false;
    }

    for (size_t i = 0; i < word.length(); ++i) {
      auto offset = index + i;
      if (line[offset] != word[i]) {
        return false;
      }
    }

    return true;
  }

  int parse_int_at_index(const std::string &line, size_t index) {
    const std::array<std::string, 9> WORDS = {
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    for (size_t word_index = 0; word_index < WORDS.size(); ++word_index) {
      if (is_word_at_index(line, index, WORDS[word_index])) {
        return word_index + 1;
      }
    }
    return -1;
  }

  bool solve_line(int &out, const std::string &line) {
    int first_digit = -1;
    int second_digit = -1;
    for (size_t index = 0; index < line.size(); ++index) {
      auto c = line[index];
      if (std::isdigit(c)) {
        if (first_digit == -1) {
          first_digit = c - '0';
        }
        second_digit = c - '0';
      }

      auto int_value = parse_int_at_index(line, index);
      if (int_value != -1) {
        if (first_digit == -1) {
          first_digit = int_value;
        }
        second_digit = int_value;
      }
    }
    auto calibration = first_digit * 10 + second_digit;

    std::cout << "Calibration at line " << line << ": " << calibration
              << std::endl;
    out = calibration;
    return true;
  }

  bool part2(const std::string &data) {
    std::stringstream ss{data};
    std::string line;
    size_t line_index = 0;
    long long sum = 0;
    while (std::getline(ss, line, '\n')) {
      int calibration;
      if (!solve_line(calibration, line)) {
        return false;
      }
      sum += calibration;
    }
    std::cout << "Calibration sum: " << sum << std::endl;
    return true;
  }

  static constexpr char EXAMPLE[] =
      "1abc2\npqr3stu8vwx\na1b2c3d4e5f\ntreb7uchet\n";

  static constexpr char EXAMPLE2[] = {
      "two1nine\neightwothree\nabcone2threexyz\nxtwone3four\n4nineeightseven2\n"
      "zoneight234\n7pqrstsixteen"};
};
