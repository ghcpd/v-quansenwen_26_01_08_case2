cmake_minimum_required(VERSION 3.20)
project(runtime_error_0105 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(audio_histogram
    src/audio_histogram.cpp
)

target_include_directories(audio_histogram
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(audio_histogram_app
    src/main.cpp
)

target_link_libraries(audio_histogram_app PRIVATE audio_histogram)

enable_testing()
add_subdirectory(tests)
