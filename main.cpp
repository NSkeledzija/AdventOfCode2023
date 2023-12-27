#include "day1/day1.h"
#include "day10/day10.h"
#include "day11/day11.h"
#include "day12/day12.h"
#include "day13/day13.h"
#include "day14/day14.h"
#include "day15/day15.h"
#include "day16/day16.h"
#include "day17/day17.h"
#include "day18/day18.h"
#include "day19/day19.h"
#include "day2/day2.h"
#include "day20/day20.h"
#include "day21/day21.h"
#include "day22/day22.h"
#include "day23/day23.h"
#include "day24/day24.h"
#include "day25/day25.h"
#include "day3/day3.h"
#include "day4/day4.h"
#include "day5/day5.h"
#include "day6/day6.h"
#include "day7/day7.h"
#include "day8/day8.h"
#include "day9/day9.h"
#include <fstream>
#include <iostream>

std::string read(const std::string &path) {
  std::ifstream t(path);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

int main() {
  std::cout << "==== AOC 2023 ====" << std::endl;

  const std::string DIR = "/home/nskeledz/projects/AdventOfCode2023/";

  Day25 d;
  d.part1(read(DIR + "/day25/input.txt"));

  return 0;
}