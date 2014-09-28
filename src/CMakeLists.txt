include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(
    kronometer_SRCS
    main.cpp
    mainwindow.cpp
    stopwatch.cpp
    timedisplay.cpp
    digitdisplay.cpp
    lap.cpp
    lapmodel.cpp
    timeformat.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -std=gnu++0x")

kde4_add_ui_files(kronometer_SRCS
    widgets/generalsettings.ui
    widgets/fontsettings.ui
    widgets/savesettings.ui
    widgets/guisettings.ui
    widgets/colorsettings.ui
)

kde4_add_kcfg_files(kronometer_SRCS settings.kcfgc) 

kde4_add_executable(kronometer ${kronometer_SRCS})

target_link_libraries(kronometer ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

install(FILES kronometer.kcfg DESTINATION ${KCFG_INSTALL_DIR})
install(TARGETS kronometer DESTINATION ${BIN_INSTALL_DIR})
install(FILES kronometerui.rc DESTINATION  ${DATA_INSTALL_DIR}/kronometer)
install(PROGRAMS kronometer.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES kronometer.appdata.xml DESTINATION ${SHARE_INSTALL_PREFIX}/appdata)

add_subdirectory(icons)

