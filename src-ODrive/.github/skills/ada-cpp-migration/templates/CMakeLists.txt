# CMakeLists.txt Template for Ada to C++ Migration
# Template for embedded C++ projects migrated from Ada
#
cmake_minimum_required(VERSION 3.25)

# ============================================================================
# Project Definition
# ============================================================================

project(embedded_cpp_migrated
    VERSION 1.0.0
    DESCRIPTION "C++ project migrated from Ada embedded system"
    LANGUAGES CXX
)

# ============================================================================
# C++ Standards and Compiler Settings
# ============================================================================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Build Types
# ============================================================================

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

# ============================================================================
# Platform Detection
# ============================================================================

option(EMBEDDED_TARGET "Build for embedded target (ARM Cortex-M)" OFF)
option(USE_FREERTOS "Use FreeRTOS for task management" OFF)

if(EMBEDDED_TARGET)
    # Cross-compilation settings
    message(STATUS "Building for embedded target")
    add_compile_definitions(EMBEDDED_TARGET)
    
    # Include ARM toolchain file
    if(NOT CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/arm-cortex-m4.cmake")
    endif()
endif()

if(USE_FREERTOS)
    add_compile_definitions(USE_FREERTOS)
    # Add FreeRTOS include paths and sources here
endif()

# ============================================================================
# Compiler Warnings (Strict - like Ada's strong typing)
# ============================================================================

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wconversion
    -Wsign-conversion
    -Wcast-qual
    -Wcast-align
    -Wold-style-cast
    -Woverloaded-virtual
    -Wnon-virtual-dtor
    -Wshadow
    -Wdouble-promotion
    -Wformat=2
    -Wundef
    -Wno-unused-parameter  # Sometimes needed for interface conformance
)

# Additional warnings for GCC
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(
        -Wlogical-op
        -Wduplicated-cond
        -Wduplicated-branches
        -Wnull-dereference
    )
endif()

# Additional warnings for Clang
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
    )
endif()

# ============================================================================
# Static Analysis (Optional)
# ============================================================================

option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
        set(CMAKE_CXX_CLANG_TIDY 
            "${CLANG_TIDY_EXE}"
            "-checks=*,-llvmlibc-*,-fuchsia-*,-altera-*"
            "-warnings-as-errors=*"
        )
        message(STATUS "clang-tidy enabled: ${CLANG_TIDY_EXE}")
    else()
        message(WARNING "clang-tidy not found")
    endif()
endif()

# ============================================================================
# Sanitizers (Debug builds only)
# ============================================================================

option(ENABLE_SANITIZERS "Enable AddressSanitizer and UBSan" OFF)

if(ENABLE_SANITIZERS AND NOT EMBEDDED_TARGET)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
    message(STATUS "Sanitizers enabled")
endif()

# ============================================================================
# Link-Time Optimization (Release builds)
# ============================================================================

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    message(STATUS "LTO enabled for release build")
endif()

# ============================================================================
# Include Directories
# ============================================================================

# Project headers
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/embedded  # Foundation libraries
)

# Third-party headers (if any)
# include_directories(${CMAKE_SOURCE_DIR}/third_party/...)

# ============================================================================
# Source Files
# ============================================================================

# Foundation libraries (migrated from Ada runtime support)
set(FOUNDATION_SOURCES
    # These are header-only, but list for documentation
)

# Core application sources
set(APP_SOURCES
    src/main.cpp
    # Add migrated modules here:
    # src/hk_data.cpp
    # src/storage.cpp
    # src/sensors.cpp
    # src/housekeeping.cpp
    # src/ttc.cpp
    # src/serial_port.cpp
    # src/fault_handler.cpp
)

# ============================================================================
# Main Executable
# ============================================================================

add_executable(${PROJECT_NAME} ${APP_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    # pthread for std::thread/mutex (not needed on Windows)
    $<$<PLATFORM_ID:Linux>:pthread>
)

# ============================================================================
# Embedded Target Specifics
# ============================================================================

if(EMBEDDED_TARGET)
    # Linker script
    set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/linker/stm32f4.ld")
    target_link_options(${PROJECT_NAME} PRIVATE
        -T${LINKER_SCRIPT}
        -Wl,--gc-sections
        -Wl,-Map=${PROJECT_NAME}.map
    )
    
    # Generate binary and hex files
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Generating binary and hex files"
    )
endif()

# ============================================================================
# Testing
# ============================================================================

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS AND NOT EMBEDDED_TARGET)
    enable_testing()
    
    # Fetch Catch2 for testing
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
    )
    FetchContent_MakeAvailable(Catch2)
    
    # Test executable
    add_executable(tests
        tests/test_main.cpp
        tests/test_strong_type.cpp
        tests/test_range_type.cpp
        tests/test_storage.cpp
        tests/test_cyclic_task.cpp
    )
    
    target_link_libraries(tests PRIVATE
        Catch2::Catch2WithMain
        $<$<PLATFORM_ID:Linux>:pthread>
    )
    
    target_include_directories(tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )
    
    include(CTest)
    include(Catch)
    catch_discover_tests(tests)
endif()

# ============================================================================
# Documentation
# ============================================================================

option(BUILD_DOCS "Build Doxygen documentation" OFF)

if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN NO)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
    
    doxygen_add_docs(docs
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        COMMENT "Generate API documentation"
    )
endif()

# ============================================================================
# Install Rules
# ============================================================================

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

# ============================================================================
# Summary
# ============================================================================

message(STATUS "")
message(STATUS "=== Build Configuration ===")
message(STATUS "Project:         ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:    ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler:        ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Embedded target: ${EMBEDDED_TARGET}")
message(STATUS "FreeRTOS:        ${USE_FREERTOS}")
message(STATUS "Tests:           ${BUILD_TESTS}")
message(STATUS "Clang-tidy:      ${ENABLE_CLANG_TIDY}")
message(STATUS "Sanitizers:      ${ENABLE_SANITIZERS}")
message(STATUS "")
