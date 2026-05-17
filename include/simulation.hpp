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
    size_t M;

  public:
    auto init(abstract::GlobalState &&state) -> Simulation;

    Simulation(event_queue_t &&event_queue, std::vector<Machine> &&machines,
               std::vector<std::vector<size_t>> &&T,
               size_t total_items, size_t M) noexcept;

    Simulation(const Simulation &) = delete;
    auto operator=(const Simulation &) -> Simulation & = delete;

    Simulation(Simulation &&) = default;
    auto operator=(Simulation &&) -> Simulation & = default;

    ~Simulation() = default;

    auto run() -> void;
    
  private:
    auto process_start(const Event &event) -> void;
    auto process_finish(const Event &event) -> void;
    auto process_wait(const Event &event) -> void;
    auto process_ready(const Event &event) -> void;
    auto process_stop(const Event &event) -> void;
    
    auto find_fitting_machine() -> Machine&;
  };

} // namespace simulation

