set(gui_SRCS
    colorsettings.cpp
    digitdisplay.cpp
    fontsettings.cpp
    generalsettings.cpp
    lapitemdelegate.cpp
    mainwindow.cpp
    sessiondialog.cpp
    timedisplay.cpp
    colorsettings.h
    digitdisplay.h
    fontsettings.h
    generalsettings.h
    lapitemdelegate.h
    mainwindow.h
    sessiondialog.h
    timedisplay.h
)

ki18n_wrap_ui(gui_SRCS
    colorsettings.ui
    fontsettings.ui
    generalsettings.ui
    sessiondialog.ui
    timedisplay.ui)

qt_add_resources(gui_SRCS kronometerui.qrc)
kconfig_add_kcfg_files(gui_SRCS settings.kcfgc GENERATE_MOC)

add_library(gui STATIC ${gui_SRCS})
target_link_libraries(gui
    Qt::Core
    Qt::Widgets
    KF5::I18n
    KF5::WidgetsAddons
    KF5::XmlGui
    core
    models)
