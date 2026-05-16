#pragma once

#include <cstddef>
#include <vector>
#include <deque>

namespace abstract {
  struct Item {
    size_t id;
    size_t type;
  };

  struct Machine {
    size_t id;
    std::deque<Item> queue;
  };
  
  struct GlobalState {
    size_t M = 0;
    size_t N = 0;

    size_t total_items = 0;
    
    std::vector<std::vector<int>> T;
    std::vector<Machine> machines;
  };
}
