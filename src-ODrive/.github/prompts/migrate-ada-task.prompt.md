---
name: migrate-ada-task
description: Migrate Ada cyclic tasks to C++ CyclicTask wrappers
agent: Ada to C++ Migrator
tools: ['read', 'edit/editFiles', 'search/codebase']
---

# Migrate Ada Task to C++ CyclicTask

Convert Ada tasks to Modern C++ using the CyclicTask wrapper.

## Input

- **Ada File**: ${input:ada_file:Path to .ads/.adb with task definition}
- **Task Name**: ${input:task_name:Name of the Ada task}

## Migration Pattern

### Ada Task Structure

```ada
-- Task specification
task Housekeeping with Priority => 30;

-- Task body
task body Housekeeping is
   Period    : constant Time_Span := Milliseconds(1000);
   Deadline  : constant Time_Span := Milliseconds(100);
   Offset    : constant Time_Span := Milliseconds(1000);
   Next_Time : Time := Clock + Offset;
begin
   loop
      delay until Next_Time;
      
      -- Do work
      Read_Sensors;
      Store_Data;
      
      Next_Time := Next_Time + Period;
   end loop;
end Housekeeping;
```

### C++ Equivalent

```cpp
class Housekeeping {
public:
    static constexpr auto kPeriod    = std::chrono::milliseconds{1000};
    static constexpr auto kDeadline  = std::chrono::milliseconds{100};
    static constexpr auto kOffset    = std::chrono::milliseconds{1000};
    static constexpr int  kPriority  = 30;
    
    Housekeeping();
    
    void start() { task_.start(); }
    void stop()  { task_.stop(); }
    
private:
    Sensor sensor_;
    embedded::concurrency::CyclicTask task_;
    
    void do_work();
};
```

## Task Timing Parameters

Extract from Ada and map to C++ config:

| Ada Constant | C++ CyclicTaskConfig Field |
|--------------|---------------------------|
| `Period` | `.period` |
| `Deadline` | `.deadline` |
| `Offset` (initial delay) | `.start_delay` |
| `Priority` (task pragma) | `.priority` |

## Implementation Pattern

```cpp
// housekeeping.hpp
#pragma once

#include "concurrency/cyclic_task.hpp"
#include "sensors/sensor.hpp"

namespace obdh {

/// @brief Periodic housekeeping task
/// @note Migrated from Ada task `Housekeeping`
class Housekeeping {
public:
    // Timing parameters from Ada specification
    static constexpr auto kPeriod     = std::chrono::milliseconds{1000};
    static constexpr auto kDeadline   = std::chrono::milliseconds{100};
    static constexpr auto kStartDelay = std::chrono::milliseconds{1000};
    static constexpr int  kPriority   = 30;
    
    Housekeeping();
    ~Housekeeping() = default;
    
    // Non-copyable (like Ada tasks)
    Housekeeping(const Housekeeping&) = delete;
    Housekeeping& operator=(const Housekeeping&) = delete;
    
    void start();
    void stop();
    
    [[nodiscard]] bool is_running() const;
    [[nodiscard]] std::uint64_t cycle_count() const;
    
private:
    Sensor temperature_sensor_;
    embedded::concurrency::CyclicTask task_;
    
    void do_work();
};

} // namespace obdh
```

```cpp
// housekeeping.cpp
#include "housekeeping.hpp"
#include "storage/storage.hpp"
#include "realtime/clock.hpp"

namespace obdh {

Housekeeping::Housekeeping()
    : task_{
        embedded::concurrency::CyclicTaskConfig{
            .period = kPeriod,
            .deadline = kDeadline,
            .start_delay = kStartDelay,
            .priority = kPriority,
            .name = "Housekeeping"
        },
        [this] { do_work(); }
    }
{
    // Initialize hardware (Ada elaboration equivalent)
    temperature_sensor_.initialize({
        .adc_channel = 5,
        .gpio_port = 0,  // Port A
        .gpio_pin = 5
    });
}

void Housekeeping::start() {
    task_.start();
}

void Housekeeping::stop() {
    task_.stop();
}

bool Housekeeping::is_running() const {
    return task_.is_running();
}

std::uint64_t Housekeeping::cycle_count() const {
    return task_.cycle_count();
}

void Housekeeping::do_work() {
    using namespace embedded::realtime;
    
    // Read sensor data
    SensorReadings readings{
        .temperature = temperature_sensor_.get_reading()
    };
    
    // Get timestamp
    auto [seconds, _] = split(Clock::now());
    
    // Create data record
    SensorData data{
        .readings = readings,
        .timestamp = MissionTime{static_cast<std::uint64_t>(seconds)}
    };
    
    // Store in protected buffer
    storage().put(data);
}

} // namespace obdh
```

## Special Cases

### 1. Tasks with Entry Points (Rendezvous)

If the Ada task has entries (for rendezvous), use `std::condition_variable`:

```ada
-- Ada
task Server is
   entry Request(Data : in Request_Type);
end Server;
```

```cpp
// C++
class Server {
    std::mutex mutex_;
    std::condition_variable cv_;
    std::optional<RequestType> pending_request_;
    
public:
    void request(const RequestType& data) {
        {
            std::lock_guard lock{mutex_};
            pending_request_ = data;
        }
        cv_.notify_one();
    }
    
private:
    void task_loop() {
        while (running_) {
            std::unique_lock lock{mutex_};
            cv_.wait(lock, [this] { return pending_request_.has_value(); });
            
            auto req = *pending_request_;
            pending_request_.reset();
            lock.unlock();
            
            process(req);
        }
    }
};
```

### 2. Select Statement with Alternatives

Map `select` statements to `std::condition_variable` with timeout:

```ada
-- Ada
select
   accept Request(...);
or
   delay 1.0;
   Handle_Timeout;
end select;
```

```cpp
// C++
if (cv_.wait_for(lock, 1s, [this] { return has_request_; })) {
    handle_request();
} else {
    handle_timeout();
}
```

### 3. Task Arrays

```ada
-- Ada
task type Worker is ... end Worker;
Workers : array (1 .. 4) of Worker;
```

```cpp
// C++
std::array<std::unique_ptr<Worker>, 4> workers;
for (auto& w : workers) {
    w = std::make_unique<Worker>();
    w->start();
}
```

## Output Requirements

Generate:

1. **Header** with class declaration matching Ada task spec
2. **Source** with implementation matching Ada task body
3. **Tests** for:
   - Task starts and stops correctly
   - Work function executes at correct period
   - Deadline monitoring works

## Template

Use:
- `.github/skills/ada-cpp-migration/templates/cyclic_task.hpp`
