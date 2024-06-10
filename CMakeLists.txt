cmake_minimum_required(VERSION 3.15.3)
project(leoplay)

set(CMAKE_CXX_STANDARD 20)

find_package(fmt REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)



include_directories(${GST_INCLUDE_DIRS})

set(SOURCES
	leoplay.cpp
	cfgparse.h
)

add_executable(leoplay ${SOURCES})
target_link_libraries(leoplay ${GST_LIBRARIES} fmt::fmt)
