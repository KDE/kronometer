set(core_SRCS
    lap.cpp
    session.cpp
    stopwatch.cpp
    timeformat.cpp
    lap.h
    session.h
    stopwatch.h
    timeformat.h)

add_library(core STATIC ${core_SRCS})
target_link_libraries(core Qt::Core)
