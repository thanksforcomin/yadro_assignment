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
    std::deque<Item> workload;
    size_t busy_until;
    size_t total_workload;
  };
  
  struct GlobalState {
    size_t M = 0;
    size_t N = 0;

    size_t total_items = 0;
    
    std::vector<std::vector<size_t>> T;
    std::vector<Machine> machines;
  };

  enum EventType {
    FINISH = 0,
    START,
    WAIT,
    READY,
    STOP
  };

  struct Event {
    EventType type;
    size_t time = 0;
    size_t k = 0;
    size_t i = 0;
    size_t j = 0;
    size_t p = 0;

    auto operator>(const Event &other) -> bool {
      if (time != other.time)
        return time > other.time;
      if (type != other.type)
        return type > other.type;
      if (j != other.j)
        return j > other.j;
      return k > other.k;
    }
  };
}
