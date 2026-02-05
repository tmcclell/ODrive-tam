// cyclic_task.hpp - Cyclic task wrapper (Ada task equivalent)
// Template for Ada to C++ migration
//
// Usage:
//   CyclicTask housekeeping{
//       {.period = 1000ms, .priority = 30, .name = "Housekeeping"},
//       [] { do_work(); }
//   };
//   housekeeping.start();
//
#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>
#include <string_view>

// FreeRTOS support (optional)
#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

namespace embedded::concurrency {

// ============================================================================
// Task Configuration
// ============================================================================

/// @brief Configuration for cyclic tasks (matches Ada task discriminants)
struct CyclicTaskConfig {
    /// @brief Task period (time between invocations)
    std::chrono::milliseconds period{1000};
    
    /// @brief Relative deadline for each invocation (for monitoring)
    std::chrono::milliseconds deadline{100};
    
    /// @brief Delay before first execution
    std::chrono::milliseconds start_delay{0};
    
    /// @brief Task priority (higher = more important)
    /// @note Maps to RTOS priority; ignored in pure std::thread mode
    int priority{30};
    
    /// @brief Task name for debugging
    const char* name{"CyclicTask"};
    
    /// @brief Stack size in words (RTOS only)
    std::size_t stack_size{512};
};

/// @brief Task state for monitoring
enum class TaskState {
    Created,    ///< Task created but not started
    Running,    ///< Task is actively running
    Suspended,  ///< Task is suspended
    Terminated  ///< Task has stopped
};

// ============================================================================
// Cyclic Task Implementation
// ============================================================================

/// @brief Cyclic task wrapper - Ada cyclic task equivalent
///
/// Provides a portable abstraction over:
/// - `std::jthread` for host/simulation builds
/// - FreeRTOS tasks for embedded builds
///
/// Features:
/// - Precise timing using `sleep_until` (Ada `delay until` equivalent)
/// - Graceful shutdown via atomic flag
/// - Deadline monitoring (optional)
///
class CyclicTask {
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;
    using WorkFunction = std::function<void()>;
    
    /// @brief Construct a cyclic task
    /// @param config Task configuration
    /// @param work Function to execute each cycle
    template <typename Func>
    explicit CyclicTask(CyclicTaskConfig config, Func&& work)
        : config_{config}
        , work_{std::forward<Func>(work)}
        , state_{TaskState::Created}
    {}
    
    /// @brief Destructor - stops the task if running
    ~CyclicTask() {
        stop();
    }
    
    // Non-copyable, non-movable (like Ada tasks)
    CyclicTask(const CyclicTask&) = delete;
    CyclicTask& operator=(const CyclicTask&) = delete;
    CyclicTask(CyclicTask&&) = delete;
    CyclicTask& operator=(CyclicTask&&) = delete;
    
    /// @brief Start the task
    void start() {
        if (running_.exchange(true, std::memory_order_acq_rel)) {
            return;  // Already running
        }
        
        state_.store(TaskState::Running, std::memory_order_release);
        
#ifdef USE_FREERTOS
        xTaskCreate(
            task_entry_point,
            config_.name,
            config_.stack_size,
            this,
            config_.priority,
            &task_handle_
        );
#else
        thread_ = std::jthread{[this](std::stop_token st) { run_loop(st); }};
#endif
    }
    
    /// @brief Stop the task gracefully
    void stop() {
        running_.store(false, std::memory_order_release);
        
#ifdef USE_FREERTOS
        if (task_handle_ != nullptr) {
            // Give task time to exit cleanly
            vTaskDelay(pdMS_TO_TICKS(config_.period.count() * 2));
            vTaskDelete(task_handle_);
            task_handle_ = nullptr;
        }
#else
        if (thread_.joinable()) {
            thread_.request_stop();
            thread_.join();
        }
#endif
        
        state_.store(TaskState::Terminated, std::memory_order_release);
    }
    
    /// @brief Check if task is running
    [[nodiscard]] bool is_running() const noexcept {
        return running_.load(std::memory_order_acquire);
    }
    
    /// @brief Get current task state
    [[nodiscard]] TaskState state() const noexcept {
        return state_.load(std::memory_order_acquire);
    }
    
    /// @brief Get task name
    [[nodiscard]] std::string_view name() const noexcept {
        return config_.name;
    }
    
    /// @brief Get task period
    [[nodiscard]] std::chrono::milliseconds period() const noexcept {
        return config_.period;
    }
    
    /// @brief Get number of completed cycles
    [[nodiscard]] std::uint64_t cycle_count() const noexcept {
        return cycle_count_.load(std::memory_order_acquire);
    }
    
    /// @brief Get number of deadline misses
    [[nodiscard]] std::uint64_t deadline_misses() const noexcept {
        return deadline_misses_.load(std::memory_order_acquire);
    }
    
private:
    CyclicTaskConfig config_;
    WorkFunction work_;
    std::atomic<bool> running_{false};
    std::atomic<TaskState> state_{TaskState::Created};
    std::atomic<std::uint64_t> cycle_count_{0};
    std::atomic<std::uint64_t> deadline_misses_{0};
    
#ifdef USE_FREERTOS
    TaskHandle_t task_handle_{nullptr};
    
    /// @brief FreeRTOS task entry point
    static void task_entry_point(void* param) {
        auto* self = static_cast<CyclicTask*>(param);
        self->run_loop_rtos();
    }
    
    /// @brief Main loop for FreeRTOS
    void run_loop_rtos() {
        TickType_t last_wake_time = xTaskGetTickCount();
        
        // Initial delay
        if (config_.start_delay.count() > 0) {
            vTaskDelay(pdMS_TO_TICKS(config_.start_delay.count()));
            last_wake_time = xTaskGetTickCount();
        }
        
        while (running_.load(std::memory_order_acquire)) {
            auto start = xTaskGetTickCount();
            
            // Execute work
            work_();
            
            // Check deadline
            auto elapsed = xTaskGetTickCount() - start;
            if (elapsed > pdMS_TO_TICKS(config_.deadline.count())) {
                deadline_misses_.fetch_add(1, std::memory_order_relaxed);
            }
            
            cycle_count_.fetch_add(1, std::memory_order_relaxed);
            
            // Wait until next period (absolute timing)
            vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(config_.period.count()));
        }
        
        // Self-delete
        vTaskDelete(nullptr);
    }
#else
    std::jthread thread_;
    
    /// @brief Main loop for std::jthread
    void run_loop([[maybe_unused]] std::stop_token stop_token) {
        auto next_time = Clock::now() + config_.start_delay;
        
        // Wait for start delay
        if (config_.start_delay.count() > 0) {
            std::this_thread::sleep_until(next_time);
        }
        
        next_time = Clock::now() + config_.period;
        
        while (running_.load(std::memory_order_acquire) && 
               !stop_token.stop_requested()) {
            
            auto start = Clock::now();
            
            // Execute work
            work_();
            
            // Check deadline
            auto elapsed = Clock::now() - start;
            if (elapsed > config_.deadline) {
                deadline_misses_.fetch_add(1, std::memory_order_relaxed);
            }
            
            cycle_count_.fetch_add(1, std::memory_order_relaxed);
            
            // Ada: delay until Next_Time;
            std::this_thread::sleep_until(next_time);
            next_time += config_.period;
        }
    }
#endif
};

// ============================================================================
// One-Shot Task (for non-periodic work)
// ============================================================================

/// @brief One-shot task that runs once and completes
class OneShotTask {
public:
    using WorkFunction = std::function<void()>;
    
    template <typename Func>
    explicit OneShotTask(const char* name, Func&& work)
        : name_{name}
        , work_{std::forward<Func>(work)}
    {}
    
    ~OneShotTask() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    
    OneShotTask(const OneShotTask&) = delete;
    OneShotTask& operator=(const OneShotTask&) = delete;
    
    void start() {
        thread_ = std::jthread{[this] {
            work_();
            completed_.store(true, std::memory_order_release);
        }};
    }
    
    [[nodiscard]] bool is_completed() const noexcept {
        return completed_.load(std::memory_order_acquire);
    }
    
    void wait() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    
private:
    const char* name_;
    WorkFunction work_;
    std::jthread thread_;
    std::atomic<bool> completed_{false};
};

} // namespace embedded::concurrency
