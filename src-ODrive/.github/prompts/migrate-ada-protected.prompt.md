---
name: migrate-ada-protected
description: Migrate Ada protected objects to C++ thread-safe classes
agent: Ada to C++ Migrator
tools: ['read', 'edit/editFiles', 'search/codebase']
---

# Migrate Ada Protected Object to C++

Convert Ada protected objects to Modern C++ thread-safe classes.

## Input

- **Ada File**: ${input:ada_file:Path to .ads/.adb with protected object}
- **Protected Object Name**: ${input:protected_name:Name of the protected object}

## Migration Pattern

### Ada Protected Object Structure

```ada
protected Buffer is
   -- Procedures (exclusive write access)
   procedure Put(Data : in Sensor_Data);
   
   -- Functions (shared read access)  
   function Empty return Boolean;
   function Last return Sensor_Data;
   
   -- Entries (blocking with barrier)
   entry Get(Data : out Sensor_Data);
   
private
   Store    : Data_Array;
   Has_Data : Boolean := False;
end Buffer;
```

### C++ Equivalent

```cpp
class Buffer {
public:
    // Procedure → unique_lock
    void put(const SensorData& data);
    
    // Function → shared_lock (const)
    [[nodiscard]] bool empty() const;
    [[nodiscard]] SensorData last() const;
    
    // Entry → condition_variable wait
    [[nodiscard]] SensorData get();  // Blocks until data available
    
private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;  // For entries
    DataArray store_;
    bool has_data_{false};
};
```

## Implementation Rules

### 1. Protected Procedures → Exclusive Lock

```cpp
void Buffer::put(const SensorData& data) {
    std::lock_guard lock{mutex_};
    // ... implementation
    cv_.notify_one();  // Wake waiting entries
}
```

### 2. Protected Functions → Shared Lock (or mutex for simplicity)

```cpp
[[nodiscard]] bool Buffer::empty() const {
    std::lock_guard lock{mutex_};
    return !has_data_;
}
```

### 3. Protected Entries → Condition Variable

```cpp
[[nodiscard]] SensorData Buffer::get() {
    std::unique_lock lock{mutex_};
    cv_.wait(lock, [this] { return has_data_; });  // Barrier
    has_data_ = false;
    return store_;
}
```

### 4. Multiple Entries → Multiple CVs or Shared CV

If multiple entries exist with different barriers, consider:
- Separate `std::condition_variable` per entry
- Or use `notify_all()` and let each entry recheck its barrier

## Output Requirements

Generate:

1. **Header file** (`.hpp`):
   - Class declaration
   - Doxygen comments noting Ada origin
   - `[[nodiscard]]` on getters
   - `const` on read-only methods

2. **Source file** (`.cpp`):
   - Implementation with proper locking
   - Notify calls after state changes

3. **Test file** (`test_*.cpp`):
   - Thread safety tests
   - Barrier condition tests
   - Deadlock-free verification

## Template

Use base class from:
- `.github/skills/ada-cpp-migration/templates/protected_object.hpp`

## Example Output

```cpp
// storage.hpp - Migrated from storage.ads
#pragma once

#include <array>
#include <condition_variable>
#include <mutex>
#include <optional>
#include "hk_data.hpp"

namespace obdh {

/// @brief Thread-safe circular buffer for sensor data
/// @note Migrated from Ada protected object `Storage.Buffer`
class Storage {
public:
    static constexpr std::size_t kCapacity = 5;
    
    /// @brief Store sensor data (Ada: procedure Put)
    void put(const SensorData& data);
    
    /// @brief Retrieve oldest data, blocks if empty (Ada: entry Get)
    [[nodiscard]] SensorData get();
    
    /// @brief Try to get data without blocking
    [[nodiscard]] std::optional<SensorData> try_get();
    
    /// @brief Get most recent data (Ada: function Last)
    [[nodiscard]] std::optional<SensorData> last() const;
    
    /// @brief Check if buffer is empty (Ada: function Empty)
    [[nodiscard]] bool empty() const;
    
    /// @brief Check if buffer is full (Ada: function Full)
    [[nodiscard]] bool full() const;
    
private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    
    std::array<SensorData, kCapacity> buffer_{};
    std::size_t head_{0};      // Next write position
    std::size_t tail_{0};      // Next read position
    std::size_t count_{0};     // Current item count
    std::size_t last_idx_{0};  // Most recent item
};

/// @brief Global storage instance (Ada package-level object)
Storage& storage();

} // namespace obdh
```

```cpp
// storage.cpp
#include "storage.hpp"

namespace obdh {

void Storage::put(const SensorData& data) {
    {
        std::lock_guard lock{mutex_};
        buffer_[head_] = data;
        last_idx_ = head_;
        head_ = (head_ + 1) % kCapacity;
        
        if (count_ < kCapacity) {
            ++count_;
        } else {
            tail_ = (tail_ + 1) % kCapacity;  // Overwrite oldest
        }
    }
    cv_.notify_one();  // Wake any waiting get()
}

SensorData Storage::get() {
    std::unique_lock lock{mutex_};
    cv_.wait(lock, [this] { return count_ > 0; });  // Barrier: when not empty
    
    SensorData data = buffer_[tail_];
    tail_ = (tail_ + 1) % kCapacity;
    --count_;
    return data;
}

std::optional<SensorData> Storage::try_get() {
    std::lock_guard lock{mutex_};
    if (count_ == 0) {
        return std::nullopt;
    }
    SensorData data = buffer_[tail_];
    tail_ = (tail_ + 1) % kCapacity;
    --count_;
    return data;
}

std::optional<SensorData> Storage::last() const {
    std::lock_guard lock{mutex_};
    if (count_ == 0) {
        return std::nullopt;
    }
    return buffer_[last_idx_];
}

bool Storage::empty() const {
    std::lock_guard lock{mutex_};
    return count_ == 0;
}

bool Storage::full() const {
    std::lock_guard lock{mutex_};
    return count_ == kCapacity;
}

Storage& storage() {
    static Storage instance;
    return instance;
}

} // namespace obdh
```
