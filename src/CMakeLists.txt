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
    sessiondialog.ui
    widgets/colorsettings.ui
    widgets/fontsettings.ui
    widgets/generalsettings.ui)

kconfig_add_kcfg_files(kronometer_SRCS settings.kcfgc)

add_executable(kronometer ${kronometer_SRCS})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(kronometer PRIVATE -pedantic)
endif()

target_link_libraries(kronometer
    KF5::Crash
    KF5::I18n
    KF5::WidgetsAddons
    KF5::XmlGui)

install(TARGETS kronometer ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kronometer.kcfg
        DESTINATION ${KDE_INSTALL_KCFGDIR})
install(FILES kronometerui.rc
        DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/kronometer)
