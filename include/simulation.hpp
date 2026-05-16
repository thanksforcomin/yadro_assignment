#pragma once

#include "abstract.hpp"
#include <deque>
#include <queue>

namespace simulation {
  using namespace abstract;
  using abstract::EventType;
  
  using event_queue_t =
      std::priority_queue<Event, std::vector<Event>, std::greater<Event>>;
  
  class Simulation {
    event_queue_t event_queue;
    std::vector<Machine> machines;
    std::vector<std::vector<size_t>> T;
    size_t total_items;
    size_t completed_items = 0;

  public:
    auto init(abstract::GlobalState &&state) -> Simulation;

    Simulation(event_queue_t &&event_queue, std::vector<Machine> &&machines,
               std::vector<std::vector<size_t>> &&T,
               size_t total_items) noexcept;

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
