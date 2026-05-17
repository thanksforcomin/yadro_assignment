#pragma once

#include <filesystem>
#include <fstream>
#include <optional>

#include "abstract.hpp"

namespace parser {
  auto parse_from_file(const std::string &filename) -> abstract::GlobalState;
}

