include(ECMAddTests)

find_package(Qt5Test REQUIRED NO_MODULE)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(libtest_SRCS
    ${CMAKE_SOURCE_DIR}/src/session.cpp
    ${CMAKE_SOURCE_DIR}/src/lap.cpp)

add_library(libtest STATIC ${libtest_SRCS})
target_link_libraries(libtest Qt5::Core)

macro(add_unit_test testname testmain testsrc)
    set(${testname}_SRCS ${testmain} ${testsrc})
    add_executable(${testname} ${${testname}_SRCS})
    add_test(${testname} ${testname})
    ecm_mark_as_test(${testname})
    target_link_libraries(${testname}
        libtest
        Qt5::Test)
endmacro()

add_unit_test(test_timeformat
    testtimeformat.cpp ${CMAKE_SOURCE_DIR}/src/timeformat.cpp)
add_unit_test(test_stopwatch
    teststopwatch.cpp ${CMAKE_SOURCE_DIR}/src/stopwatch.cpp)
add_unit_test(test_lap
    testlap.cpp ${CMAKE_SOURCE_DIR}/src/lap.cpp)
add_unit_test(test_session
    testsession.cpp ${CMAKE_SOURCE_DIR}/src/session.cpp)
