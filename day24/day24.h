#pragma once

#include "utils.h"
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

class Day24 {
public:
  struct Hailstone {
    double x;
    double y;
    double z;
    double vx;
    double vy;
    double vz;
  };

  using eq_matrix = std::array<std::array<double, 5>, 4>;

  eq_matrix find_equation_matrix(const std::vector<Hailstone> &stones) {
    std::array<std::array<double, 5>, 4> equations;
    for (int i = 0; i < 4; ++i) {
      // A * X + B * Y + C * dx + D * dy = E
      double A = (stones.at(i + 1).vy - stones.at(i).vy);
      double B = (stones.at(i).vx - stones.at(i + 1).vx);
      double C = (stones.at(i).y - stones.at(i + 1).y);
      double D = (stones.at(i + 1).x - stones.at(i).x);
      double E = stones.at(i + 1).x * stones.at(i + 1).vy -
                 stones.at(i + 1).y * stones.at(i + 1).vx -
                 stones.at(i).x * stones.at(i).vy +
                 stones.at(i).y * stones.at(i).vx;
      equations[i] = {A, B, C, D, E};
    }
    return equations;
  }

  eq_matrix find_equation_matrix_z(const std::vector<Hailstone> &stones) {
    std::array<std::array<double, 5>, 4> equations;
    for (int i = 0; i < 4; ++i) {
      // A * Z + B * Y + C * dz + D * dy = E
      double A = (stones.at(i + 1).vy - stones.at(i).vy);
      double B = (stones.at(i).vz - stones.at(i + 1).vz);
      double C = (stones.at(i).y - stones.at(i + 1).y);
      double D = (stones.at(i + 1).z - stones.at(i).z);
      double E = stones.at(i + 1).z * stones.at(i + 1).vy -
                 stones.at(i + 1).y * stones.at(i + 1).vz -
                 stones.at(i).z * stones.at(i).vy +
                 stones.at(i).y * stones.at(i).vz;
      equations[i] = {A, B, C, D, E};
    }
    return equations;
  }

  void swap_rows(eq_matrix &mat, int i, int j) {
    for (int k = 0; k < mat.at(i).size(); ++k) {
      auto tmp = mat[i][k];
      mat[i][k] = mat[j][k];
      mat[j][k] = tmp;
    }
  }

  std::array<double, 4> solve(eq_matrix &mat) {
    for (int row = 0; row < mat.size(); ++row) {
      auto pivot_row = row;
      auto col = row; // Looking at diagonal so row == col
      auto pivot_val = mat[row][col];

      for (int next_row = row + 1; next_row < mat.size(); ++next_row) {
        if (fabs(mat[next_row][col]) > fabs(pivot_val)) {
          pivot_val = mat[next_row][col];
          pivot_row = next_row;
        }
      }

      if (pivot_val == 0) {
        throw std::runtime_error("Can't happen!");
      }

      if (pivot_row != row) {
        swap_rows(mat, pivot_row, row);
      }

      for (int next_row = row + 1; next_row < mat.size(); ++next_row) {
        auto f = mat[next_row][col] / mat[row][col];

        for (int next_col = col; next_col < mat.at(next_row).size();
             ++next_col) {
          mat[next_row][next_col] -= mat[row][next_col] * f;
        }

        mat[next_row][col] = 0;
      }
    }

    std::array<double, 4> solution;
    for (int i = mat.size() - 1; i >= 0; --i) {
      solution[i] = mat.at(i).at(mat.size());
      for (int j = i + 1; j < mat.size(); ++j) {
        solution[i] -= mat.at(i).at(j) * solution.at(j);
      }
      solution.at(i) = solution.at(i) / mat.at(i).at(i);
    }
    return solution;
  }

  void part2(const std::string &input) {
    auto lines = split(input, "\n");
    std::vector<Hailstone> stones;
    for (auto line : lines) {
      auto tokens = split(line, "@");
      auto point = split(tokens.at(0), ",");
      auto velocities = split(tokens.at(1), ",");
      Hailstone stone;
      stone.x = std::stod(point.at(0));
      stone.y = std::stod(point.at(1));
      stone.z = std::stod(point.at(2));
      stone.vx = std::stod(velocities.at(0));
      stone.vy = std::stod(velocities.at(1));
      stone.vz = std::stod(velocities.at(2));
      stones.emplace_back(stone);
    }

    auto matrix = find_equation_matrix(stones);
    auto solved = solve(matrix);
    auto matrix_z = find_equation_matrix_z(stones);
    auto solved_z = solve(matrix_z);
    std::cout << "Total: "
              << static_cast<long long>(
                     std::round(solved[0] + solved[1] + solved_z[0]))
              << std::endl;
  }
};