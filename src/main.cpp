#include <iostream>
#include <format>

#include "parser.hpp"
#include "simulation.hpp"

auto main(int argc, char *argv[]) -> int {
  if (argc != 2) {
    std::cout << std::format("Usage: {} filename", argv[0]);
  }

  auto global_state = parser::parse_from_file(argv[1]);

  auto simulation = simulation::Simulation::init(std::move(global_state));

  simulation.run();

  return 0;
}
