include(ECMAddTests)

find_package(Qt5Test ${QT_MIN_VERSION} REQUIRED NO_MODULE)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(tested_SRCS
    ${CMAKE_SOURCE_DIR}/src/digitdisplay.cpp
    ${CMAKE_SOURCE_DIR}/src/lap.cpp
    ${CMAKE_SOURCE_DIR}/src/session.cpp
    ${CMAKE_SOURCE_DIR}/src/stopwatch.cpp
    ${CMAKE_SOURCE_DIR}/src/timedisplay.cpp
    ${CMAKE_SOURCE_DIR}/src/timeformat.cpp)

add_library(autotests STATIC ${tested_SRCS})
target_link_libraries(autotests
    Qt5::Core
    Qt5::Widgets
    KF5::I18n)

ecm_add_tests(
    testlap.cpp
    testsession.cpp
    teststopwatch.cpp
    testtimedisplay.cpp
    testtimeformat.cpp
    LINK_LIBRARIES autotests Qt5::Test)
