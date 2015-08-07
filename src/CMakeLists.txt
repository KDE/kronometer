set(kronometer_SRCS
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
    widgets/generalsettings.cpp)

ki18n_wrap_ui(kronometer_SRCS
    widgets/colorsettings.ui
    widgets/fontsettings.ui
    widgets/generalsettings.ui)

kconfig_add_kcfg_files(kronometer_SRCS settings.kcfgc)

add_executable(kronometer ${kronometer_SRCS})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(kronometer PRIVATE -Werror -pedantic)
endif()

target_link_libraries(kronometer
    KF5::I18n
    KF5::WidgetsAddons
    KF5::XmlGui)

install(TARGETS kronometer ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES org.kde.kronometer.appdata.xml
        DESTINATION ${KDE_INSTALL_DATADIR}/kronometer)
install(FILES kronometer.kcfg
        DESTINATION ${KCFG_INSTALL_DIR})
install(FILES kronometerui.rc
        DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/kronometer)
install(PROGRAMS org.kde.kronometer.desktop
        DESTINATION ${KDE_INSTALL_APPDIR})
