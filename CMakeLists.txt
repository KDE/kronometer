project(kronometer)

cmake_minimum_required (VERSION 2.8.12)

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMInstallIcons)
include(FeatureSummary)

find_package(Qt5 REQUIRED
    COMPONENTS
    Widgets
)
find_package(KF5 REQUIRED
    COMPONENTS
    Config
    CoreAddons
    DocTools
)

add_subdirectory(src)
add_subdirectory(doc)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
