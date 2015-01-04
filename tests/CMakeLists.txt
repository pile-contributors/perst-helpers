# ============================================================================
#
# This file hosts all tests.
#
# If INCLUDE_PILE_SOURCES_DIRECTLY is ON then PERST_SOURCES and
# PERST_HEADERS are defined we add them to the executable. PERST_LIBRARY
# is empty.
#
# On the other hand if INCLUDE_PILE_SOURCES_DIRECTLY is
# OFF then PERST_SOURCES and PERST_HEADERS are empty and we link
# against PERST_LIBRARY that is now the name of the target that
# builds the library.
#
# ============================================================================

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

if (NOT INCLUDE_PILE_SOURCES_DIRECTLY)
    set (PERST_LIBRARY perst)
endif()

add_executable(test_basic
    ${PERST_SOURCES}
    ${PERST_HEADERS}
    basic.cc)

target_link_libraries(test_basic
    ${GTEST_BOTH_LIBRARIES}
    ${PERST_LIBRARY})

add_test(
    NAME test_basic
    COMMAND $<TARGET_FILE:test_basic>)
