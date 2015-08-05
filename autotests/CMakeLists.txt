include(ECMAddTests)

find_package(Qt5Test REQUIRED NO_MODULE)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(libtest_SRCS
    ${CMAKE_SOURCE_DIR}/src/lap.cpp
    ${CMAKE_SOURCE_DIR}/src/session.cpp
    ${CMAKE_SOURCE_DIR}/src/stopwatch.cpp
    ${CMAKE_SOURCE_DIR}/src/timeformat.cpp)

add_library(libtest STATIC ${libtest_SRCS})
target_link_libraries(libtest Qt5::Core)

macro(autotests)
    foreach(testname ${ARGN})
        add_executable(${testname} ${testname}.cpp)
        add_test(${testname} ${testname})
        ecm_mark_as_test(${testname})
        target_link_libraries(${testname}
            libtest
            Qt5::Test)
    endforeach()
endmacro()

autotests(testlap
          testsession
          teststopwatch
          testtimeformat)
