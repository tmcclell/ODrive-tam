/**
 * @file demo_buggy.cpp
 * @brief Demo file for Section 6: Debugging with Copilot
 * 
 * This file contains intentional bugs for demonstration purposes.
 * DO NOT include in production builds!
 * 
 * Bugs included:
 * 1. Off-by-one error in circular buffer (log_fault)
 * 2. Race condition between ISR and main loop
 * 3. Integer overflow in RPM calculation
 */

#include <cstdint>
#include <cstdio>

//=============================================================================
// Bug 1: Off-By-One Error in Circular Buffer
// BUG: Can you spot the off-by-one error?
//=============================================================================

#define FAULT_HISTORY_SIZE 10

class FaultLogger {
private:
    uint32_t fault_history_[FAULT_HISTORY_SIZE];
    size_t fault_idx_ = 0;

public:
    
    void log_fault(uint32_t error_code) {
        fault_history_[fault_idx_] = error_code;
        fault_idx_++;
        
        if (fault_idx_ > FAULT_HISTORY_SIZE) {
            fault_idx_ = 0;
        }
    }

    void print_faults() {
        printf("Fault History:\n");
        for (size_t i = 0; i < FAULT_HISTORY_SIZE; i++) {
            printf("  [%zu]: 0x%08X\n", i, fault_history_[i]);
        }
    }
    
    size_t get_index() const { return fault_idx_; }
};

//=============================================================================
// Bug 2: Race Condition (ISR vs Main Loop)
// RACE: Main loop reads these while ISR writes!
//=============================================================================

class Encoder {
private:
    int32_t count_ = 0;
    int32_t last_count_ = 0;
    float position_estimate_ = 0.0f;
    float velocity_estimate_ = 0.0f;
    int32_t cpr_ = 8192;  // Counts per revolution

public:
    // Called from 8 kHz interrupt
    void update_isr() {
        int32_t delta = 10;  // Simulated hardware read
        count_ += delta;
        last_count_ = count_ - delta;
        
        position_estimate_ = (float)count_ / (float)cpr_;
        velocity_estimate_ = (float)delta / (float)cpr_ * 8000.0f;
    }

    // Called from main loop (1 kHz)
    float get_position() {
        return position_estimate_;  // RACE: ISR writes this!
    }

    float get_velocity() {
        return velocity_estimate_;  // RACE: ISR writes this!
    }
};

//=============================================================================
// Bug 3: Integer Overflow in Speed Calculation
// BUG: Integer overflow at high speeds!
// BUG: This intermediate calculation overflows int32_t at high speeds!
// At 10,000 RPM: delta_count ≈ 1000
// 1000 * 60 * 1000000 = 60,000,000,000 (exceeds int32_t max of 2,147,483,647)
//=============================================================================

class SpeedCalculator {
private:
    int32_t count_ = 0;
    int32_t last_count_ = 0;
    int32_t cpr_ = 8192;  // Counts per revolution

public:
    void set_counts(int32_t current, int32_t last) {
        count_ = current;
        last_count_ = last;
    }

    float calculate_rpm() {
        int32_t delta_count = count_ - last_count_;
        int32_t delta_time_us = 125;  // 8 kHz = 125 μs

        int32_t rpm = (delta_count * 60 * 1000000) / (cpr_ * delta_time_us);

        return (float)rpm;
    }
};

//=============================================================================
// Test Harness (for manual testing)
//=============================================================================

#ifdef DEMO_MAIN
int main() {
    printf("=== Bug 1: Off-By-One Error Demo ===\n");
    FaultLogger logger;
    
    // Log 15 faults - should wrap around safely
    for (int i = 0; i < 15; i++) {
        printf("Logging fault %d, index before: %zu\n", i, logger.get_index());
        logger.log_fault(0x1000 + i);
    }
    logger.print_faults();
    
    printf("\n=== Bug 3: Integer Overflow Demo ===\n");
    SpeedCalculator calc;
    
    // Low speed - should work
    calc.set_counts(100, 0);
    printf("Low speed (100 delta): %.1f RPM\n", calc.calculate_rpm());
    
    // High speed - will overflow!
    calc.set_counts(1000, 0);
    printf("High speed (1000 delta): %.1f RPM (OVERFLOW!)\n", calc.calculate_rpm());
    
    return 0;
}
#endif
