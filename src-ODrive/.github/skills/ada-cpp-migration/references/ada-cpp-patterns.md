# Ada to C++ Quick Pattern Reference

> Fast lookup for common Ada → C++ mappings. For detailed explanations, see `migration-plan.md`.

---

## Type Mappings

### Strong Types (Derived Types)

```ada
-- Ada: Derived type with same operations
type Sensor_Reading is new UInt16;
type Mission_Time is new UInt64;
```

```cpp
// C++: Strong type wrapper
struct SensorReadingTag {};
using SensorReading = embedded::StrongType<std::uint16_t, SensorReadingTag>;

struct MissionTimeTag {};
using MissionTime = embedded::StrongType<std::uint64_t, MissionTimeTag>;
```

### Range-Constrained Types

```ada
-- Ada: Range constraint (validated at runtime)
type ADC_Value is range 0 .. 4095;
subtype Percentage is Integer range 0 .. 100;
```

```cpp
// C++: Range-validated wrapper
using AdcValue = embedded::RangeType<std::uint16_t, 0, 4095>;
using Percentage = embedded::RangeType<int, 0, 100>;
```

### Records (Structs)

```ada
-- Ada: Record type
type Sensor_Data is record
   Reading   : Sensor_Reading;
   Timestamp : Mission_Time;
end record;
```

```cpp
// C++: Struct with defaulted comparison
struct SensorData {
    SensorReading reading;
    MissionTime   timestamp;
    
    constexpr auto operator<=>(const SensorData&) const = default;
};
```

### Packed Records

```ada
-- Ada: Packed record for serialization
type Telemetry_Packet is record
   Header : UInt8;
   Data   : Sensor_Data;
end record with Pack;
```

```cpp
// C++: Packed struct
struct [[gnu::packed]] TelemetryPacket {
    std::uint8_t header;
    SensorData   data;
};
```

### Arrays

```ada
-- Ada: Constrained array
type HK_Log is array (1 .. 5) of Sensor_Data;
```

```cpp
// C++: std::array
using HkLog = std::array<SensorData, 5>;
```

### Enumerations

```ada
-- Ada: Enumeration with representation
type Motor_State is (Idle, Calibrating, Running, Fault);
for Motor_State use (Idle => 0, Calibrating => 1, Running => 2, Fault => 3);
```

```cpp
// C++: Scoped enum with explicit underlying type
enum class MotorState : std::uint8_t {
    Idle       = 0,
    Calibrating = 1,
    Running    = 2,
    Fault      = 3
};
```

---

## Concurrency Mappings

### Cyclic Task

```ada
-- Ada: Periodic task
task Housekeeping with Priority => 30;
task body Housekeeping is
   Period    : constant Time_Span := Milliseconds(1000);
   Next_Time : Time := Clock + Milliseconds(1000);
begin
   loop
      delay until Next_Time;
      Do_Work;
      Next_Time := Next_Time + Period;
   end loop;
end Housekeeping;
```

```cpp
// C++: CyclicTask wrapper
embedded::concurrency::CyclicTask housekeeping{
    {
        .period = std::chrono::milliseconds{1000},
        .start_delay = std::chrono::milliseconds{1000},
        .priority = 30,
        .name = "Housekeeping"
    },
    [] { do_work(); }
};

// In main:
housekeeping.start();
```

### Protected Object (Basic)

```ada
-- Ada: Protected object with procedure and function
protected Buffer is
   procedure Put(Data : in Sensor_Data);
   function  Empty return Boolean;
private
   Store : Sensor_Data;
   Has_Data : Boolean := False;
end Buffer;
```

```cpp
// C++: Mutex-protected class
class Buffer {
    mutable std::mutex mutex_;
    SensorData store_{};
    bool has_data_{false};
    
public:
    void put(const SensorData& data) {
        std::lock_guard lock{mutex_};
        store_ = data;
        has_data_ = true;
    }
    
    [[nodiscard]] bool empty() const {
        std::lock_guard lock{mutex_};
        return !has_data_;
    }
};
```

### Protected Object (With Entry/Barrier)

```ada
-- Ada: Protected entry with barrier
protected Buffer is
   entry Get(Data : out Sensor_Data);  -- Blocks if empty
   procedure Put(Data : in Sensor_Data);
private
   Store : Sensor_Data;
   Has_Data : Boolean := False;
end Buffer;

protected body Buffer is
   entry Get(Data : out Sensor_Data) when Has_Data is
   begin
      Data := Store;
      Has_Data := False;
   end Get;
end Buffer;
```

```cpp
// C++: Condition variable for blocking
class Buffer {
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    SensorData store_{};
    bool has_data_{false};
    
public:
    SensorData get() {  // Blocks until data available
        std::unique_lock lock{mutex_};
        cv_.wait(lock, [this] { return has_data_; });
        has_data_ = false;
        return store_;
    }
    
    void put(const SensorData& data) {
        {
            std::lock_guard lock{mutex_};
            store_ = data;
            has_data_ = true;
        }
        cv_.notify_one();
    }
};
```

---

## Time Operations

### Clock and Duration

```ada
-- Ada
with Ada.Real_Time; use Ada.Real_Time;

Period : constant Time_Span := Milliseconds(100);
Now    : Time := Clock;
```

```cpp
// C++
#include <chrono>

using namespace std::chrono_literals;
constexpr auto kPeriod = 100ms;
auto now = std::chrono::steady_clock::now();
```

### Delay Until (Absolute Time)

```ada
-- Ada
delay until Next_Time;
```

```cpp
// C++
std::this_thread::sleep_until(next_time);
```

### Delay (Relative Time)

```ada
-- Ada
delay 0.5;  -- 500 milliseconds
```

```cpp
// C++
std::this_thread::sleep_for(500ms);
```

### Time Arithmetic

```ada
-- Ada
Next_Time := Next_Time + Period;
Elapsed := Clock - Start_Time;
```

```cpp
// C++
next_time += period;
auto elapsed = std::chrono::steady_clock::now() - start_time;
```

---

## I/O Operations

### Stream Output (Serialization)

```ada
-- Ada
Sensor_Data'Output(Serial_Stream, Data);
```

```cpp
// C++
serial_port.write_object(data);
```

### Stream Input (Deserialization)

```ada
-- Ada
Data := Sensor_Data'Input(Serial_Stream);
```

```cpp
// C++
auto result = serial_port.read_object<SensorData>();
if (result) {
    auto data = *result;
}
```

---

## Error Handling

### Exception Raising

```ada
-- Ada
raise Constraint_Error with "Value out of range";
```

```cpp
// C++ (with exceptions)
throw std::out_of_range("Value out of range");

// C++ (embedded, no exceptions)
CONSTRAINT_CHECK(false, "Value out of range");  // Triggers fault handler
```

### Exception Handling

```ada
-- Ada
begin
   Risky_Operation;
exception
   when Constraint_Error =>
      Handle_Error;
end;
```

```cpp
// C++ (with exceptions)
try {
    risky_operation();
} catch (const std::out_of_range& e) {
    handle_error();
}

// C++ (embedded, no exceptions)
auto result = risky_operation();
if (!result) {
    handle_error(result.error());
}
```

### Last Chance Handler

```ada
-- Ada
procedure Last_Chance_Handler(Msg : System.Address; Line : Integer) is
begin
   -- Display error and halt
   loop null; end loop;
end Last_Chance_Handler;
```

```cpp
// C++
[[noreturn]] void last_chance_handler(
    std::string_view message,
    std::source_location loc = std::source_location::current()
) {
    __disable_irq();
    // Display error on LCD or blink LED
    while (true) { __WFI(); }
}
```

---

## Package/Module Structure

### Ada Package → C++ Header/Source

```ada
-- sensors.ads (specification)
package Sensors is
   type Sensor is tagged private;
   procedure Initialize(S : in out Sensor);
   function Get_Reading(S : Sensor) return Sensor_Reading;
private
   type Sensor is tagged record
      Channel : ADC_Channel;
   end record;
end Sensors;
```

```cpp
// sensors.hpp (header)
#pragma once
#include "hk_data.hpp"

namespace obdh {

class Sensor {
    AdcChannel channel_{};
    bool initialized_{false};
    
public:
    void initialize(AdcChannel channel);
    [[nodiscard]] SensorReading get_reading() const;
};

} // namespace obdh
```

```ada
-- sensors.adb (body)
package body Sensors is
   procedure Initialize(S : in Out Sensor) is
   begin
      -- Implementation
   end Initialize;
end Sensors;
```

```cpp
// sensors.cpp (source)
#include "sensors.hpp"

namespace obdh {

void Sensor::initialize(AdcChannel channel) {
    // Implementation
}

SensorReading Sensor::get_reading() const {
    // Implementation
}

} // namespace obdh
```

---

## Constants and Pragmas

### Package-Level Constants

```ada
-- Ada
HK_Log_Length : constant Positive := 5;
Max_Voltage   : constant Float := 56.0;
```

```cpp
// C++
inline constexpr std::size_t kHkLogLength = 5;
inline constexpr float kMaxVoltage = 56.0f;
```

### Pragma Inline

```ada
-- Ada
pragma Inline(Small_Function);
```

```cpp
// C++
[[gnu::always_inline]] inline void small_function() { }
// Or just define in header (compiler decides)
```

### Pragma Volatile

```ada
-- Ada
pragma Volatile(Hardware_Register);
```

```cpp
// C++
volatile std::uint32_t* hardware_register = 
    reinterpret_cast<volatile std::uint32_t*>(0x40020000);
```
