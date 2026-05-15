#pragma once

#include <filesystem>
#include <fstream>
#include <optional>

#include "abstract.hpp"

namespace parser {
  struct ParserState {
    std::string_view data;
    size_t pos = 0;

    explicit ParserState(std::string_view buffer);

    auto skip_whitespace() noexcept -> void;
    auto consume() noexcept -> long long;
    auto consume_n(size_t n) noexcept -> std::vector<long long>;

    [[noreturn]] auto report_error() const -> void;
  };

  auto parse_from_file(std::string filename) -> abstract::GlobalState;
}
