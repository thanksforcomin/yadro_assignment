#pragma once

#include "abstract.hpp"
#include <deque>
#include <queue>

namespace simulation {
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

  struct Item {
    size_t id;
    size_t type;
  };
  
  struct Machine {
    size_t id;
    std::deque<Item> workload;
    size_t busy_until;
    size_t current_workload;
  };

  using event_queue_t =
      std::priority_queue<Event, std::vector<Event>, std::greater<Event>>;
  
  class Simulation {
    event_queue_t event_queue;
    std::vector<Machine> machines;

  public:
    auto init(abstract::GlobalState &&state) -> Simulation;

    Simulation(event_queue_t &&event_queue,
               std::vector<Machine> &&machines) noexcept;

    Simulation(const Simulation &) = delete;
    auto operator=(const Simulation &) -> Simulation & = delete;

    Simulation(Simulation &&) = delete;
    auto operator=(Simulation &&) -> Simulation & = delete;

    ~Simulation() = default;

    auto process_event();

  private:
    auto find_fitting_machine() -> Machine&;
    
  };
  
}
