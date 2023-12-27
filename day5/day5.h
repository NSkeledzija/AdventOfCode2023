#pragma once

#include "utils.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <limits>
#include <queue>
#include <stdexcept>
#include <unordered_map>

class Day5 {
public:
  struct Mapping {
    size_t source;
    size_t destination;
    size_t length;
  };

  bool parse_seeds(const std::string &line, std::vector<size_t> &seeds) {
    auto tokens = split(line);
    for (const auto &t :
         std::vector<std::string>{tokens.begin() + 1, tokens.end()}) {
      seeds.push_back(stol(t));
    }
    return true;
  }

  bool parse_seed_ranges(const std::string &line, std::vector<size_t> &seeds) {
    auto tokens = split(line);
    for (size_t i = 1; i < tokens.size() - 1; i += 2) {
      if (i + 1 == tokens.size()) {
        throw std::runtime_error("Wrong seed parse!");
      }
      auto begin = stol(tokens[i]);
      auto length = stol(tokens[i + 1]);
      for (size_t j = 0; j < length; ++j) {
        seeds.push_back(begin + j);
      }
    }
    return true;
  }

  bool parse_seed_ranges(const std::string &line,
                         std::vector<std::pair<size_t, size_t>> &seeds) {
    auto tokens = split(line);
    for (size_t i = 1; i < tokens.size() - 1; i += 2) {
      if (i + 1 == tokens.size()) {
        throw std::runtime_error("Wrong seed parse!");
      }
      auto begin = stol(tokens[i]);
      auto length = stol(tokens[i + 1]);
      seeds.push_back({begin, begin + length});
    }
    return true;
  }

  bool parse_map(const std::vector<std::string> &lines,
                 std::vector<Mapping> &map) {
    for (const auto &line :
         std::vector<std::string>{lines.begin() + 1, lines.end()}) {
      auto tokens = split(line);
      Mapping m;
      m.destination = stol(tokens[0]);
      m.source = stol(tokens[1]);
      m.length = stol(tokens[2]);
      map.push_back(m);
    }
    return true;
  }

  bool parse_maps(const std::vector<std::string> &map_descriptors,
                  std::vector<std::vector<Mapping>> &maps) {
    for (const auto &md : map_descriptors) {
      auto lines = split(md, "\n");
      std::vector<Mapping> map;
      parse_map(lines, map);
      maps.push_back(map);
    }
    return true;
  }

  bool parse_input(const std::string &input, std::vector<size_t> &seeds,
                   std::vector<std::vector<Mapping>> &maps) {
    auto tokens = split(input, "\n\n");
    parse_seeds(tokens[0], seeds);
    parse_maps({tokens.begin() + 1, tokens.end()}, maps);
    return true;
  }

  bool parse_input_seed_ranges(const std::string &input,
                               std::vector<size_t> &seeds,
                               std::vector<std::vector<Mapping>> &maps) {
    auto tokens = split(input, "\n\n");
    parse_seed_ranges(tokens[0], seeds);
    parse_maps({tokens.begin() + 1, tokens.end()}, maps);
    return true;
  }

  bool parse_input_seed_ranges(const std::string &input,
                               std::vector<std::pair<size_t, size_t>> &seeds,
                               std::vector<std::vector<Mapping>> &maps) {
    auto tokens = split(input, "\n\n");
    parse_seed_ranges(tokens[0], seeds);
    parse_maps({tokens.begin() + 1, tokens.end()}, maps);
    return true;
  }

  size_t transform_seed(size_t seed, const std::vector<Mapping> &map) {
    for (const auto &m : map) {
      if (seed >= m.source && seed < m.source + m.length) {
        return m.destination + seed - m.source;
      }
    }
    return seed;
  }

  std::optional<std::pair<size_t, size_t>>
  find_overlap(std::pair<size_t, size_t> seeds, std::pair<size_t, size_t> map) {
    auto seed_begin = seeds.first;
    auto seed_end = seeds.second;
    auto map_begin = map.first;
    auto map_end = map.second;

    assert(seed_begin < seed_end);
    assert(map_begin < map_end);

    auto begin = std::max(seed_begin, map_begin);
    auto end = std::min(seed_end, map_end);

    if (begin < end) {
      return {{begin, end}};
    }

    return std::nullopt;
  }

  std::vector<std::pair<size_t, size_t>>
  substract(std::pair<size_t, size_t> seeds,
            std::pair<size_t, size_t> overlap) {
    if (overlap.first < seeds.first || overlap.first > seeds.second) {
      throw std::runtime_error("Invalid overlap");
    }
    if (overlap.second < seeds.first || overlap.second > seeds.second) {
      throw std::runtime_error("Invalid overlap");
    }

    std::vector<std::pair<size_t, size_t>> ranges;
    if (overlap.first > seeds.first && overlap.second < seeds.second) {
      ranges.push_back({seeds.first, overlap.first});
      ranges.push_back({overlap.second, seeds.second});
    } else if (overlap.first > seeds.first && overlap.second == seeds.second) {
      ranges.push_back({seeds.first, overlap.first});
    } else if (overlap.first == seeds.first && overlap.second < seeds.second) {
      ranges.push_back({overlap.second, seeds.second});
    } else if (overlap.first == seeds.first && overlap.second == seeds.second) {
      return {};
    } else {
      throw std::runtime_error("Invalid overlap!");
    }
    return ranges;
  }

  std::vector<std::pair<size_t, size_t>>
  transform_seed_range(std::pair<size_t, size_t> seeds,
                       const std::vector<Mapping> &map) {
    std::vector<std::pair<size_t, size_t>> new_seeds;
    std::queue<std::pair<size_t, size_t>> seed_ranges;
    seed_ranges.push(seeds);
    for (const auto &m : map) {
      if (seed_ranges.empty()) {
        break;
      }
      auto current_seeds = seed_ranges.front();
      seed_ranges.pop();
      auto overlap =
          find_overlap(current_seeds, {m.source, m.source + m.length});
      if (overlap.has_value()) {
        auto transformed_begin = m.destination + overlap->first - m.source;
        auto transformed_end =
            transformed_begin + (overlap->second - overlap->first);
        auto transformed =
            std::pair<size_t, size_t>{transformed_begin, transformed_end};
        new_seeds.push_back(transformed);
        for (const auto &r : substract(current_seeds, overlap.value())) {
          seed_ranges.push(r);
        }
      } else {
        seed_ranges.push(current_seeds);
      }
    }

    while (!seed_ranges.empty()) {
      new_seeds.push_back(seed_ranges.front());
      seed_ranges.pop();
    }

    return new_seeds;
  }

  bool find_min_location(const std::vector<size_t> &seeds,
                         const std::vector<std::vector<Mapping>> &maps) {
    size_t min_location = std::numeric_limits<size_t>::max();
    for (const auto &c : seeds) {
      size_t transformed = c;

      for (const auto &m : maps) {
        transformed = transform_seed(transformed, m);
      }
      min_location = std::min(min_location, transformed);
    }
    std::cout << "Min location: " << min_location << std::endl;
    return true;
  }

  bool
  find_min_location_ranges(const std::vector<std::pair<size_t, size_t>> &seeds,
                           const std::vector<std::vector<Mapping>> &maps) {
    size_t min_location = std::numeric_limits<size_t>::max();

    size_t processed = 0;
    std::vector<std::pair<size_t, size_t>> prev = seeds;
    std::vector<std::pair<size_t, size_t>> next;

    for (const auto &m : maps) {
      for (const auto &r : prev) {
        auto transformed = transform_seed_range(r, m);
        for (const auto &t : transformed) {
          next.push_back(t);
        }
      }
      prev = next;
      next = {};
    }

    for (const auto &r : prev) {
      min_location = std::min(min_location, r.first);
    }

    std::cout << "Min location: " << min_location << std::endl;
    return true;
  }

  bool part1(const std::string &input) {
    std::vector<size_t> seeds;
    std::vector<std::vector<Mapping>> maps;
    parse_input(input, seeds, maps);
    find_min_location(seeds, maps);
    return true;
  }

  bool part2(const std::string &input) {
    std::vector<std::pair<size_t, size_t>> seeds;
    std::vector<std::vector<Mapping>> maps;
    parse_input_seed_ranges(input, seeds, maps);
    find_min_location_ranges(seeds, maps);
    return true;
  }
};