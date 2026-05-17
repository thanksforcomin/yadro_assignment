#include <iostream>
#include <format>

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
              .k = item.id,
              .j = machine.id
          });
        }
      }
    } else {
      for (auto &machine : state.machines) {
        if (machine.workload.empty())
          continue;

        
        auto item = machine.workload.front();
        machine.workload.pop_front();  
        machine.total_workload -= T[item.type][machine.id];
        event_queue.push(Event{
            .type = START,
            .time = 0,
            .k = item.id,
            .i = item.type,
            .j = machine.id
        });
      }
    }

    return Simulation(std::move(event_queue), std::move(state.machines),
                      std::move(state.T), state.total_items, state.M);
  }


  Simulation::Simulation(event_queue_t &&event_queue,
                         std::vector<Machine> &&machines,
                         std::vector<std::vector<size_t>> &&T,
                         size_t total_items, size_t M) noexcept
      : event_queue(std::forward<decltype(event_queue)>(event_queue)),
        machines(std::forward<decltype(machines)>(machines)),
        T(std::forward<decltype(T)>(T)), total_items(total_items), M(M) {}

  auto Simulation::run() -> void {
    while (!event_queue.empty()) {
      auto event = event_queue.top();
      event_queue.pop();

      switch (event.type) {
      case START:
        process_start(event);
        break;
      case FINISH:
        process_finish(event);
        break;
      case WAIT:
        process_wait(event);
        break;
      case READY:
        process_ready(event);
        break;
      case STOP:
        process_stop(event);
        break;
      default:
        break;
      }
    }
  }

  auto Simulation::process_start(const Event &event) -> void {
    std::cout << std::format("start {} {} {} {}\n", event.time, event.k,
                             event.i, event.j);

    auto duration = T[event.i][event.j];
    machines[event.j].busy_until = event.time + duration;
    event_queue.push(Event{
        .type = FINISH,
        .time = event.time + duration,
        .k = event.k,
        .i = event.i,
        .j = event.j
    });
  }

  auto Simulation::process_finish(const Event &event) -> void {
    std::cout << std::format("finish {} {} {} {}\n", event.time, event.k, event.i,
                             event.j);

    if (event.i == M - 2) {
      event_queue.push(Event{
          .type = READY,
          .time = event.time,
          .k = event.k,
          .j = event.j
      });
    } else {
      // we have to find an appropriate machine to stuff our thing into
      auto next_type = event.i + 1;
      auto &machine = find_fitting_machine();

      if (machine.busy_until <= event.time) {
        // we immediately start processing it
        event_queue.push(Event{.type = START,
                               .time = event.time,
                               .k = event.k,
                               .i = next_type,
                               .j = machine.id

        });

      } else {
        // we start waiting
        event_queue.push(Event{.type = WAIT,
                               .time = event.time,
                               .k = event.k,
                               .i = next_type,
                               .j = machine.id,
                               .p = machine.workload.size()

        });
      }
    }

    if (!machines[event.j].workload.empty()) {
      auto &next_item = machines[event.j].workload.front();
      machines[event.j].workload.pop_front();

      machines[event.j].total_workload -= T[next_item.type][machines[event.j].id];
      event_queue.push(Event{.type = START,
                             .time = event.time,
                             .k = next_item.id,
                             .i = next_item.type,
                             .j = event.j

      });
    }
  }

  auto Simulation::process_wait(const Event &event) -> void {
    std::cout << std::format("wait {} {} {} {} {}\n", event.time, event.k,
                             event.i, event.j, event.p);

    auto item = Item{.id = event.k, .type = event.i};
    machines[event.j].total_workload += T[item.type][event.j];
    machines[event.j].workload.push_back(item);
  }

  auto Simulation::process_ready(const Event &event) -> void {
    std::cout << std::format("ready {} {} {}\n", event.time, event.k, event.j);

    completed_items++;
    if (completed_items == total_items)
      event_queue.push(Event{.type = STOP, .time = event.time});
  }

  auto Simulation::process_stop(const Event &event) -> void {
    std::cout << std::format("stop {}\n", event.time);
  }

  auto Simulation::find_fitting_machine() -> Machine & {
    size_t best_j = 0;
    size_t min_workload = machines[0].total_workload;
    for (size_t i = 0; i < machines.size(); ++i) {
      if (machines[i].total_workload < min_workload) {
        min_workload = machines[i].total_workload;
        best_j = i;
      }
    }

    return machines[best_j];
  }
} // namespace simulation

