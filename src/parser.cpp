#include <cstddef>
#include <iostream>
#include <charconv>
#include <system_error>

#include "parser.hpp"
#include "abstract.hpp"

namespace parser {
  ParserState::ParserState(std::string_view buffer) : data(buffer) {}

  auto ParserState::skip_whitespace() noexcept -> void {
    while (pos < data.size()) {
      auto c = data[pos];
      if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
        ++pos;
      } else {
        break;
      }
    }
  }

  auto ParserState::consume() noexcept -> long long {
    skip_whitespace();

    if (pos > data.size())
      report_error();

    long long val;
    auto [ptr, ec] =
      std::from_chars(data.data() + pos, data.data() + data.size(), val);
    if (ec != std::errc() || ptr == data.data() + pos)
      report_error();

    pos = ptr - data.data();
    return val;
  }

  auto ParserState::consume_n(size_t n) noexcept -> std::vector<long long> {
    std::vector<long long> toks(n);
    for (size_t i = 0; i < n; ++i) {
      toks[i] = consume();
    }
    return toks;
  }

  auto ParserState::report_error() const -> void {
    size_t line_start = pos;
    while (line_start > 0 && data[line_start - 1] != '\n')
      line_start--;

    size_t line_end = pos;
    while (pos < data.size() && data[line_end] != '\n')
      line_end--;

    std::cout << data.substr(line_start, line_end - line_start) << '\n';
    std::exit(1);
  }
  
}
