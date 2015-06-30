set(
    kronometer_SRCS
    digitdisplay.cpp
    lap.cpp
    lapmodel.cpp
    main.cpp
    mainwindow.cpp
    session.cpp
    sessiondialog.cpp
    sessionmodel.cpp
    stopwatch.cpp
    timedisplay.cpp
    timeformat.cpp
)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -std=gnu++0x")

qt5_wrap_ui(kronometer_SRCS
    widgets/colorsettings.ui
    widgets/fontsettings.ui
    widgets/generalsettings.ui
    widgets/guisettings.ui
    widgets/savesettings.ui
)

kconfig_add_kcfg_files(kronometer_SRCS settings.kcfgc)

add_executable(kronometer ${kronometer_SRCS})

target_link_libraries(
    kronometer
    KF5::I18n
    KF5::WidgetsAddons
    KF5::XmlGui
)

install(TARGETS kronometer ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kronometer.appdata.xml
        DESTINATION ${KDE_INSTALL_DATADIR}/kronometer)
install(FILES kronometer.kcfg
        DESTINATION ${KCFG_INSTALL_DIR})
install(FILES kronometerui.rc
        DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/kronometer)
install(PROGRAMS org.kde.kronometer.desktop
        DESTINATION ${KDE_INSTALL_APPDIR})

add_subdirectory(icons)

