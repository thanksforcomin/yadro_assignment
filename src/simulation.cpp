#include "simulation.hpp"
#include "abstract.hpp"

namespace simulation {
  auto Simulation::init(abstract::GlobalState &&state) -> Simulation {
    event_queue_t event_queue;
    
    for (auto &machine : state.machines) {
      for (auto const &item : machine.workload) {
        machine.total_workload += state.T[item.type][machine.id];
      }
    }

    // A curious edge case where we don't need to post the "start" statements
    if (state.M == 1) {
      for (auto const &machine : state.machines) {
        for (auto const &item : machine.workload) {
          event_queue.push(Event{
              .type = READY,
              .time = 0,
              .j = machine.id
          });
        }
      }
    } else {
      for (auto &machine : state.machines) {
        auto item = machine.workload.front();
        machine.workload.pop_front();  
        machine.total_workload -= T[item.type][machine.id];
        event_queue.push(Event{
            .type = START,
            .time = 0,
            .k = item.id,
            .j = machine.id
        });
      }
    }

    return Simulation(std::move(event_queue), std::move(state.machines),
                      std::move(state.T), state.total_items);

  }
  
}
