cmake_minimum_required(VERSION 3.5)

project(Qt_Coursework VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Concurrent PrintSupport Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
        workloadwindow.h
        workloadwindow.cpp
        workloadwindow.ui
)

add_executable(Qt_Coursework
    ${PROJECT_SOURCES}
)

add_compile_definitions(QCUSTOMPLOT_USE_LIBRARY)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Libs)
target_link_libraries(Qt_Coursework PRIVATE Qt6::Widgets
                                            Qt::Core
                                            Qt6::Sql
                                            Qt6::Concurrent
                                            Qt6::PrintSupport
                                            qcustomplot2.dll
                                            libcrypto-3-x64
                                            libiconv-2
                                            libintl-9
                                            libpq
                                            libssl-3-x64)

