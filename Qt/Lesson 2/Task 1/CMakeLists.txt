cmake_minimum_required(VERSION 3.14)

project(Qt_Lesson2_Task1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Sql)


add_executable(Qt_Lesson2_Task1
  main.cpp
)
target_link_libraries(Qt_Lesson2_Task1 PRIVATE Qt6::Core)
target_link_libraries(Qt_Lesson2_Task1 PRIVATE Qt6::Network)
target_link_libraries(Qt_Lesson2_Task1 PRIVATE Qt6::Sql)

install(TARGETS Qt_Lesson2_Task1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
