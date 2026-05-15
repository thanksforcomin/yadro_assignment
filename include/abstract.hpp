#pragma once

#include <vector>
#include <deque>

namespace abstract {
  struct Item {
    int id;
    int type;
  };
  
  struct Machine {
    std::deque<Item> queue;
  };
  
  struct GlobalState {
    int M = 0;
    int N = 0;
    
    std::vector<std::vector<int>> T;
    std::vector<Machine> machines;
    
    int total_items = 0;
  };
}
