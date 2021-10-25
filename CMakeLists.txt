cmake_minimum_required(VERSION 3.12) 

set(PROJECT_NAME test)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(${PROJECT_NAME})
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "-g")

find_package(Lua51 REQUIRED)
find_package(X11 REQUIRED)
find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

file(GLOB SRC_FILES "src/*.c" "lib/mathc/*.c")
include_directories("include/")
include_directories("lib/mathc/")

file(COPY "res" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})

include_directories(${LUA_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES})

target_link_libraries(${PROJECT_NAME} m)
