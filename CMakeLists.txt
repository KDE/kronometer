project(kronometer)

cmake_minimum_required(VERSION 3.1)
set(QT_MIN_VERSION 5.0.0)
set(KF5_MIN_VERSION 5.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

find_package(ECM 1.6.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMInstallIcons)
include(FeatureSummary)

if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED
    COMPONENTS
    Core
    Gui
    Widgets)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED
    COMPONENTS
    Config
    DocTools
    I18n
    WidgetsAddons
    XmlGui)

add_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -DQT_USE_QSTRINGBUILDER)

add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(icons)
if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
