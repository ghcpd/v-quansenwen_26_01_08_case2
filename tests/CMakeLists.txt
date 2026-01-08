add_executable(unit_tests
    test_audio_histogram.cpp
)

target_include_directories(unit_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(unit_tests PRIVATE audio_histogram)

add_test(NAME unit_tests COMMAND unit_tests)
# Ensure tests run with the source-tree as working dir so data/ paths resolve
set_tests_properties(unit_tests PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
