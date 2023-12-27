#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using position = std::pair<int, int>;

enum class Direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3, NONE = 5 };
template <typename T, typename U>
std::pair<T, U> operator+(const std::pair<T, U> &l, const std::pair<T, U> &r) {
  return {l.first + r.first, l.second + r.second};
}
inline std::pair<int, int> direction_to_offsets(Direction d);

template <typename T, typename U>
std::pair<T, U> operator+(const std::pair<T, U> &l, Direction dir) {
  auto r = direction_to_offsets(dir);
  return {l.first + r.first, l.second + r.second};
}

inline Direction tc(Direction orig) {
  return static_cast<Direction>((static_cast<int>(orig) + 1) % 4);
}

inline Direction tcc(Direction orig) {
  return static_cast<Direction>((static_cast<int>(orig) + 3) % 4);
}

inline std::pair<int, int> direction_to_offsets(Direction d) {
  if (Direction::NORTH == d) {
    return {-1, 0};
  } else if (Direction::EAST == d) {
    return {0, 1};
  } else if (Direction::SOUTH == d) {
    return {1, 0};
  } else if (Direction::WEST == d) {
    return {0, -1};
  } else {
    throw std::runtime_error("Invalid direction");
  }
}

inline bool position_in_grid(position position, size_t rows, size_t cols) {
  if (position.first < 0 || (size_t)position.first >= rows) {
    return false;
  }
  if (position.second < 0 || (size_t)position.second >= cols) {
    return false;
  }
  return true;
}

inline std::vector<std::string> split(std::string s, std::string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

inline std::vector<std::string> split(const std::string &s) {
  std::istringstream buffer(s);
  std::vector<std::string> ret{std::istream_iterator<std::string>(buffer),
                               std::istream_iterator<std::string>()};
  return ret;
}

static auto predicate = [](unsigned char ch) { return !std::isspace(ch); };
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), predicate));
}

static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), predicate).base(), s.end());
}

static inline std::string &trim(std::string &s) {
  rtrim(s);
  ltrim(s);
  return s;
}
