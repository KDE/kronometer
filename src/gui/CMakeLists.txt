set(gui_SRCS
    digitdisplay.cpp
    generalsettings.cpp
    mainwindow.cpp
    sessiondialog.cpp
    timedisplay.cpp)

ki18n_wrap_ui(gui_SRCS
    colorsettings.ui
    fontsettings.ui
    generalsettings.ui
    sessiondialog.ui
    timedisplay.ui)

kconfig_add_kcfg_files(gui_SRCS settings.kcfgc GENERATE_MOC)

add_library(gui STATIC ${gui_SRCS})
target_link_libraries(gui
    Qt5::Core
    Qt5::Widgets
    KF5::I18n
    KF5::WidgetsAddons
    KF5::XmlGui
    core
    models)
