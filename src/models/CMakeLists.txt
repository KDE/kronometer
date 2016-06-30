set(models_SRCS
    lapmodel.cpp
    sessionmodel.cpp)

add_library(models STATIC ${models_SRCS})
target_link_libraries(models
    Qt5::Core
    KF5::I18n
    core)
