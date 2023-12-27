#pragma once

#include "utils.h"
#include <deque>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <tuple>
#include <unordered_map>

class Day20 {
public:
  struct Module {
    std::string name;
    std::vector<std::string> outputs;
    std::vector<std::string> inputs;
    virtual std::optional<std::pair<bool, std::vector<std::string>>>
    process_signal(const std::pair<bool, std::string> &signal) = 0;
  };

  struct Flip_flop : Module {
    std::optional<std::pair<bool, std::vector<std::string>>>
    process_signal(const std::pair<bool, std::string> &signal) override {
      if (signal.first == false) {
        state = !state;
        return {{state, outputs}};
      }
      return std::nullopt;
    };

    bool state = false;
  };

  struct Broadcaster : Module {
    std::optional<std::pair<bool, std::vector<std::string>>>
    process_signal(const std::pair<bool, std::string> &signal) override {
      return {{signal.first, outputs}};
    };
  };

  struct Conjunction : Module {
    std::optional<std::pair<bool, std::vector<std::string>>>
    process_signal(const std::pair<bool, std::string> &signal) override {
      bool all_high = true;
      input_state[signal.second] = signal.first;
      for (const auto &i : input_state) {
        if (i.second == false) {
          all_high = false;
          break;
        }
      }
      return {{!all_high, outputs}};
    };

    std::unordered_map<std::string, bool> input_state;
  };

  struct Output : Module {
    std::optional<std::pair<bool, std::vector<std::string>>>
    process_signal(const std::pair<bool, std::string> &signal) override {
      return std::nullopt;
    };
  };

  std::unordered_map<std::string, std::unique_ptr<Module>>
  parse_modules(const std::string &input) {
    auto lines = split(input, "\n");
    std::unordered_map<std::string, std::unique_ptr<Module>> modules;
    std::vector<std::string> conjunctions;
    for (const auto &l : lines) {
      std::string name;
      auto tokens = split(l, "->");
      std::unique_ptr<Module> m = nullptr;
      trim(tokens.at(0));
      if (tokens.at(0) == "broadcaster") {
        m = std::make_unique<Broadcaster>();
        name = tokens.at(0);
        modules[name] = std::move(m);
      } else {
        name = tokens.at(0).substr(1);
        if (tokens.at(0).at(0) == '&') {
          modules[name] = std::make_unique<Conjunction>();
          conjunctions.push_back(name);
        } else {
          modules[name] = std::make_unique<Flip_flop>();
        };
      }
      if (modules[name] == nullptr) {
        throw std::runtime_error("Invalid module");
      }
      modules[name]->name = name;
      auto outputs = split(tokens.at(1), ",");
      for (auto o : outputs) {
        trim(o);
        modules[name]->outputs.push_back(o);
      }
    }
    for (auto c : conjunctions) {
      for (const auto &m : modules) {
        for (const auto &o : m.second->outputs) {
          if (o == c) {
            auto &con = dynamic_cast<Conjunction &>(*modules[c]);
            con.input_state[m.first] = false;
          }
        }
      }
    }

    for (const auto &m : modules) {
      for (const auto &o : m.second->outputs) {
        if (modules.find(o) == modules.end()) {
          modules[o] = std::make_unique<Output>();
          modules[o]->name = o;
        }
      }
    }

    for (const auto &m : modules) {
      for (const auto &o : m.second->outputs) {
        modules.at(o)->inputs.push_back(m.first);
      }
    }

    return modules;
  }

  void part1(const std::string &input) {
    auto modules = parse_modules(input);
    std::deque<std::tuple<std::string, bool, std::string>> signal_queue;
    long long hp = 0;
    long long lp = 0;
    for (int i = 0; i < 1000; ++i) {
      signal_queue.push_back({"button", false, "broadcaster"});
      while (!signal_queue.empty()) {
        auto signal = signal_queue.front();
        if (std::get<1>(signal)) {
          ++hp;
        } else {
          ++lp;
        }
        signal_queue.pop_front();
        if (modules.find(std::get<2>(signal)) == modules.end()) {
          continue;
        }
        auto &m = modules.at(std::get<2>(signal));
        auto ns = m->process_signal({std::get<1>(signal), std::get<0>(signal)});
        if (ns.has_value()) {
          auto value = ns.value();
          for (const auto &s : value.second) {
            signal_queue.push_back({std::get<2>(signal), value.first, s});
          }
        }
      }
    }
    std::cout << "Total: " << hp * lp << std::endl;
  }

  void part2(const std::string &input) {
    auto modules = parse_modules(input);
    std::deque<std::tuple<std::string, bool, std::string>> signal_queue;
    unsigned long long bp = 0;

    std::unordered_map<std::string, unsigned long long> periods;

    // Ugly hack based on plotting the input with graphviz, probably the right
    // solution is to detect nodes with cycles and simulate this layer of the
    // graph
    periods["xc"] = 0;
    periods["bp"] = 0;
    periods["th"] = 0;
    periods["pd"] = 0;

    size_t cycle = 0;
    while (true) {
      signal_queue.push_back({"button", false, "broadcaster"});
      ++cycle;
      bool all_set = true;

      while (!signal_queue.empty()) {
        auto signal = signal_queue.front();
        signal_queue.pop_front();
        if (modules.find(std::get<2>(signal)) == modules.end()) {
          continue;
        }
        auto name = std::get<2>(signal);
        auto sig_val = std::get<1>(signal);
        if (periods.find(name) != periods.end()) {
          if (sig_val == false && cycle > 0) {
            periods[name] = cycle;
          }
        }

        all_set = true;
        for (auto &p : periods) {
          if (p.second == 0) {
            all_set = false;
            break;
          }
        }

        if (all_set) {
          break;
        }
        auto &m = modules.at(std::get<2>(signal));
        auto ns = m->process_signal({std::get<1>(signal), std::get<0>(signal)});
        if (ns.has_value()) {
          auto value = ns.value();
          for (const auto &s : value.second) {
            signal_queue.push_back({std::get<2>(signal), value.first, s});
          }
        }
      }
      if (all_set) {
        break;
      }
    }

    std::vector<unsigned long long> values;
    for (auto p : periods) {
      std::cout << p.first << " = " << p.second << std::endl;
      values.push_back(p.second);
    }
    unsigned long long total = values[0];
    for (int i = 1; i < 4; ++i) {
      total = std::lcm(total, values[i]);
    }

    std::cout << "Total: " << total << std::endl;
  }
};