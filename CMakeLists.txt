project(kronometer)
 
find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})
 
set(
	kronometer_SRCS 
	main.cpp
	mainwindow.cpp
	qstopwatch.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
 
kde4_add_executable(kronometer ${kronometer_SRCS})
 
target_link_libraries(kronometer ${KDE4_KDEUI_LIBS})
 
install(TARGETS kronometer DESTINATION ${BIN_INSTALL_DIR})
install(FILES kronometerui.rc 
        DESTINATION  ${DATA_INSTALL_DIR}/kronometer)
        
kde4_install_icons(${ICON_INSTALL_DIR})
kde4_update_iconcache()