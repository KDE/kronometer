project(kronometer)

cmake_minimum_required(VERSION 3.1)
set(QT_MIN_VERSION 5.15.0)
set(KF_MIN_VERSION 5.92.0)

set(PROJECT_VERSION 2.3.70)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(FeatureSummary)
include(ECMDeprecationSettings)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED
    COMPONENTS
    Core
    Gui
    Widgets)

find_package(KF5 ${KF_MIN_VERSION} REQUIRED
    COMPONENTS
    CoreAddons
    Config
    Crash
    DocTools
    I18n
    WidgetsAddons
    XmlGui)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX KRONOMETER
    VERSION_HEADER version.h)

add_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -DQT_USE_QSTRINGBUILDER)

ecm_set_disabled_deprecation_versions(QT 5.15.2
    KF 5.102
)

add_subdirectory(desktop)
add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(autotests)

ki18n_install(po)
if(KF5DocTools_FOUND)
    add_subdirectory(doc)
    kdoctools_install(po)
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
