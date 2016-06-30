add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(models)

add_executable(kronometer main.cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(kronometer PRIVATE -Werror -pedantic)
endif()

target_link_libraries(kronometer
    KF5::Crash
    gui)

install(TARGETS kronometer ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES gui/kronometer.kcfg
        DESTINATION ${KDE_INSTALL_KCFGDIR})
install(FILES kronometerui.rc
        DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/kronometer)
