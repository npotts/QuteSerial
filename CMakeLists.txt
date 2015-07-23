cmake_minimum_required(VERSION 2.8.11)

SET(APPNAME "Qute-Serial")

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOUIC ON) # Instruct CMake to run uic on ui files automatically when needed.
set(CMAKE_AUTORCC ON) #same, but for RCC files
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall") # enable warnings (always good)

project(${APPNAME})

if( MINGW )
	#this is a VERY ugly hack....
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") #remove the pesky windows GUI window
    message( STATUS "    Installing system-libraries: MinGW DLLs." )
    get_filename_component( Mingw_Path ${CMAKE_CXX_COMPILER} PATH )
    message("Mingw_Path = ${Mingw_Path}")
    install(FILES ${Mingw_Path}/libwinpthread-1.dll DESTINATION "/")
    install(FILES ${Mingw_Path}/libwinpthread-1.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/libwinpthread-1.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/libgcc_s_dw2-1.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/libstdc++-6.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/../../../5.4/mingw491_32/bin/icudt53.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/../../../5.4/mingw491_32/bin/icuin53.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/../../../5.4/mingw491_32/bin/icuuc53.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/../../../5.4/mingw491_32/bin/Qt5Core.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/../../../5.4/mingw491_32/bin/Qt5Gui.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/../../../5.4/mingw491_32/bin/Qt5Widgets.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/../../../5.4/mingw491_32/bin/Qt5Network.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/../../../5.4/mingw491_32/bin/Qt5SerialPort.dll DESTINATION "/")
	install(FILES ${Mingw_Path}/../../../5.4/mingw491_32/bin/Qt5Sql.dll DESTINATION "/")
    install(DIRECTORY ${Mingw_Path}/../../../5.4/mingw491_32/plugins/platforms DESTINATION / PATTERN "*d.dll" EXCLUDE)
	install(DIRECTORY ${Mingw_Path}/../../../5.4/mingw491_32/plugins/sqldrivers DESTINATION / PATTERN "*d.dll" EXCLUDE)
endif( MINGW )
include( InstallRequiredSystemLibraries )

#use our own mirror of a built qwt
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5SerialPort REQUIRED)

#GLOB(SRC sources/*)
SET(SRC 
	sources/QHexView
	sources/macros.cpp
	sources/macros.h
	sources/macros.ui
	sources/main.cpp
	sources/qhexview.cpp
	sources/qhexview.h
	sources/quteserial.cpp
	sources/quteserial.h
	sources/quteserial.ui
	sources/rxwidget.cpp
	sources/rxwidget.h
	sources/rxwidget.ui
	sources/txwidget.cpp
	sources/txwidget.h
	sources/txwidget.ui
	)

SET(RESOURCES resources/icons.qrc)

add_executable(${APPNAME} ${SRC} ${RESOURCES} ${UIS_HDRS})
target_link_libraries(${APPNAME} Qt5::SerialPort Qt5::Widgets Qt5::Network Qt5::Sql)
qt5_use_modules(${APPNAME} Widgets Network Sql Core)

if(WIN32)
	install(TARGETS ${APPNAME} RUNTIME DESTINATION "/")
ELSE() #Not windows
	install(TARGETS ${APPNAME} RUNTIME DESTINATION ".")
ENDIF()