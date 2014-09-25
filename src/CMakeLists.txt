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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -ansi -pedantic -std=gnu++0x")

qt5_wrap_ui(kronometer_SRCS
    widgets/generalsettings.ui
    widgets/fontsettings.ui
    widgets/savesettings.ui
    widgets/guisettings.ui
    widgets/colorsettings.ui
)

kconfig_add_kcfg_files(kronometer_SRCS settings.kcfgc) 

add_executable(kronometer ${kronometer_SRCS})

target_link_libraries(
    kronometer
    KF5::I18n
    KF5::KIOWidgets
    KF5::WidgetsAddons
    KF5::XmlGui
)

install(FILES kronometer.kcfg DESTINATION ${KCFG_INSTALL_DIR})
install(TARGETS kronometer DESTINATION ${BIN_INSTALL_DIR})
install(FILES kronometerui.rc DESTINATION  ${KXMLGUI_INSTALL_DIR}/kronometer)
install(PROGRAMS kronometer.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES kronometer.appdata.xml DESTINATION ${SHARE_INSTALL_PREFIX}/appdata)

add_subdirectory(icons)

