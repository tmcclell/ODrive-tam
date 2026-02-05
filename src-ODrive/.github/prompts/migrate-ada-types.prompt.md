---
name: migrate-ada-types
description: Migrate Ada type definitions to Modern C++ strong types
agent: Ada to C++ Migrator
tools: ['read', 'edit/editFiles', 'search/codebase']
---

# Migrate Ada Types to C++ Strong Types

Convert Ada type definitions to Modern C++ equivalents with preserved type safety.

## Input

- **Ada File**: ${input:ada_file:Path to .ads file with type definitions}
- **Output Directory**: ${input:output_dir:C++ include directory (default: include/)}

## Migration Rules

### 1. Derived Types → StrongType

```ada
-- Ada
type Sensor_Reading is new UInt16;
```

```cpp
// C++
#include "strong_type.hpp"

struct SensorReadingTag {};
using SensorReading = embedded::StrongType<
    std::uint16_t, 
    SensorReadingTag,
    embedded::Addable,
    embedded::Subtractable
>;
```

### 2. Range Types → RangeType

```ada
-- Ada  
type ADC_Value is range 0 .. 4095;
```

```cpp
// C++
#include "range_type.hpp"

using AdcValue = embedded::RangeType<std::uint16_t, 0, 4095>;
```

### 3. Records → Structs

```ada
-- Ada
type Sensor_Data is record
   Reading   : Sensor_Reading;
   Timestamp : Mission_Time;
end record;
```

```cpp
// C++
struct SensorData {
    SensorReading reading;
    MissionTime   timestamp;
    
    constexpr auto operator<=>(const SensorData&) const = default;
};
```

### 4. Packed Records

```ada
-- Ada
type Packet is record ... end record with Pack;
```

```cpp
// C++
struct [[gnu::packed]] Packet { ... };
```

### 5. Arrays

```ada
-- Ada
type Log is array (1 .. 10) of Data;
```

```cpp
// C++
using Log = std::array<Data, 10>;
```

### 6. Enumerations

```ada
-- Ada
type State is (Idle, Running, Fault);
for State use (Idle => 0, Running => 1, Fault => 2);
```

```cpp
// C++
enum class State : std::uint8_t {
    Idle    = 0,
    Running = 1,
    Fault   = 2
};
```

## Output Requirements

Generate a header file with:

1. **Header guard**: `#pragma once`
2. **Includes**: Only what's needed
3. **Namespace**: Match Ada package hierarchy
4. **Documentation**: Doxygen comments noting Ada origin
5. **Constexpr**: Where possible
6. **Tests**: Include compile-time tests with `static_assert`

## Template

Use templates from:
- `.github/skills/ada-cpp-migration/templates/strong_type.hpp`
- `.github/skills/ada-cpp-migration/templates/range_type.hpp`

## Example Output

```cpp
// hk_data.hpp - Migrated from hk_data.ads
#pragma once

#include <array>
#include <cstdint>
#include <compare>
#include "embedded/strong_type.hpp"
#include "embedded/range_type.hpp"

namespace obdh {

/// @brief Sensor reading from ADC (Ada: type Sensor_Reading is new UInt16)
struct SensorReadingTag {};
using SensorReading = embedded::StrongType<std::uint16_t, SensorReadingTag>;

/// @brief Mission elapsed time in seconds (Ada: type Mission_Time is new UInt64)
struct MissionTimeTag {};
using MissionTime = embedded::StrongType<std::uint64_t, MissionTimeTag>;

/// @brief Collection of sensor readings (Ada: type Sensor_Readings is record)
struct SensorReadings {
    SensorReading temperature;
    
    constexpr auto operator<=>(const SensorReadings&) const = default;
};

/// @brief Timestamped sensor data (Ada: type Sensor_Data is record)
struct SensorData {
    SensorReadings readings;
    MissionTime    timestamp;
    
    constexpr auto operator<=>(const SensorData&) const = default;
};

/// @brief HK log capacity (Ada: HK_Log_Length : constant Positive := 5)
inline constexpr std::size_t kHkLogLength = 5;

/// @brief HK data log (Ada: type HK_Log is array (1 .. HK_Log_Length) of Sensor_Data)
using HkLog = std::array<SensorData, kHkLogLength>;

// Compile-time validation
static_assert(sizeof(SensorReading) == sizeof(std::uint16_t));
static_assert(sizeof(MissionTime) == sizeof(std::uint64_t));

} // namespace obdh
```
