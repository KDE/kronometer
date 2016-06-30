set(core_SRCS
    lap.cpp
    session.cpp
    stopwatch.cpp
    timeformat.cpp)

add_library(core STATIC ${core_SRCS})
target_link_libraries(core Qt5::Core)
