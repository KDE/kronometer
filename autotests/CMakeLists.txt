include(ECMAddTests)

find_package(Qt5Test ${QT_MIN_VERSION} REQUIRED NO_MODULE)

add_subdirectory(core)
add_subdirectory(gui)
