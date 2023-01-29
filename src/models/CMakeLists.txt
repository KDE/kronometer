set(models_SRCS
    lapmodel.cpp lapmodel.h
    sessionmodel.cpp sessionmodel.h)

add_library(models STATIC ${models_SRCS})
target_link_libraries(models
    Qt::Core
    KF${KF_MAJOR_VERSION}::I18n
    core)
