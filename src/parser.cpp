#include <cstddef>
#include <iostream>
#include <charconv>
#include <system_error>
#include <ranges>
#include <algorithm>

#include "parser.hpp"
#include "abstract.hpp"

namespace parser {
  [[noreturn]] auto report_error(std::string_view line)->void {
    std::cout << line;
    std::exit(1);
  }
  
  auto get_lines(std::ifstream &&filestream) -> std::vector<std::string> {
    std::vector<std::string> lines;
    std::string curr_line;
    
    while (std::getline(filestream, curr_line)) {
      lines.push_back(std::move(curr_line));
    }
    
    return lines;
  }

  auto parse_line(std::string_view line) -> std::vector<size_t> {
    auto to_sv = [](auto &&data) -> std::string_view {
      return std::string_view(data.begin(), data.end());
    };
    
    auto filter_empty = [](std::string_view sv) -> bool { return !sv.empty(); };

    auto to_size_t = [&line](std::string_view sv) -> size_t {
      size_t val;
      auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), val);
      if (ec != std::errc{} || ptr != sv.data() + sv.size())
        // We can just exit() inside because spec allows hard termination on
        // formatting errors
        report_error(line);

      return val;
    };

    auto tokens = line | std::ranges::views::split(' ') |
                  std::ranges::views::transform(to_sv) |
                  std::ranges::views::filter(filter_empty) |
                  std::ranges::views::transform(to_size_t);


    return std::vector<size_t>(tokens.begin(), tokens.end());
  }

  auto parse_from_file(const std::string &filename) -> abstract::GlobalState {
    std::ifstream filestream(filename);
    if (!filestream) {
      std::cerr << "Could not open file\n";
      std::exit(1);
    }

    auto lines = get_lines(std::move(filestream));

    abstract::GlobalState state;

    auto it = lines.begin();

    if (it == lines.end())
      report_error("file ends prematurely");

    // First we parse the header
    auto& header_line = *it++;
    auto header = parse_line(header_line);
    if (header.size() != 2)
      report_error(header_line);

    state.M = header[0];
    state.N = header[1];

    if (state.M < 1 || state.M > 100 || state.N < 1 || state.N > 100)
      report_error(header_line);

    // Now we might start parsing all the Ts of all the items and the machines
    state.T.reserve(state.M > 0 ? state.M - 1 : 0);
    // M - 1 because there's M - 1 states in total and M - 2 transitions between
    // states
    for (size_t i = 0; i < state.M - 1; ++i) {
      if (it == lines.end())
        report_error("file ends prematurely");

      std::string_view line(*it++);
      auto row = parse_line(line);
      if (row.size() != state.N)
        report_error(line);

      if (!std::ranges::all_of(
              row, [](int num) -> bool { return num >= 0 && num <= 10000; }))
        report_error(line);
      state.T.push_back(std::move(row));
    }

    // Now we can start parsing the machines queue informatin
    state.machines.reserve(state.N);

    size_t total_items = 0;
    size_t next_id = 0;
    
    for (size_t i = 0; i < state.N; ++i) {
      if (it == lines.end())
        report_error("file ends prematurely");

      std::string_view line(*it++);
      auto nums = parse_line(line);
      if (nums.empty())
        report_error(line);

      int q = nums[0];
      if (q < 0 || nums.size() != static_cast<size_t>(q) + 1)
        report_error(line);

      auto items = nums | std::ranges::views::drop(1);
      if (!std::ranges::all_of(items, [M = state.M](int num) -> bool {
            return num >= 0 && num <= M - 2;
      }))
        report_error(line);

      abstract::Machine mach;
      mach.id = i;

      for (size_t item : items) {
        mach.workload.push_back(abstract::Item{.id = next_id++, .type = item});
      }

      total_items += q;
      state.machines.push_back(std::move(mach));
    }

    state.total_items = total_items;

    while (it != lines.end()) {
      std::string_view trailing(*it++);
      if (!std::ranges::all_of(trailing, [](char c) -> bool {
            return std::isspace(static_cast<unsigned char>(c));
      }))
        report_error(trailing);
    }

    return state;
  }

} // namespace parser

