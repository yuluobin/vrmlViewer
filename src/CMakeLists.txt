cmake_minimum_required(VERSION 2.8.11)

project(vrmlViewer2)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# This variable is used to locate the bin directory where the
# application will be installed
set(CMAKE_INSTALL_PREFIX
    ${PROJECT_SOURCE_DIR}/..
    CACHE PATH "Project bin directory" FORCE)

# you can comment the following line
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

# set to the proper directory in your machine 
set(Qt5_cmake_DIR "C:\\Qt\\5.12.3\\msvc2017_64\\lib\\cmake\\")

# This is needded for CMake to find the Qt libraries
set(CMAKE_PREFIX_PATH
    ${Qt5_cmake_DIR}
    CACHE PATH "Qt5 cmake directory" FORCE)

# you can comment the following line
message ("CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}") 

# alternative way to make CMake find the Qt libraries
# set(Qt5core_DIR    ${Qt5_cmake_DIR}/Qt5core)
# set(Qt5Widgets_DIR ${Qt5_cmake_DIR}/Qt5Widgets)
# set(Qt5Svg_DIR     ${Qt5_cmake_DIR}/Qt5Svg)

# Find the Qt libraries
find_package(Qt5core)
find_package(Qt5Widgets)
find_package(Qt5Svg)

set(FORMS_DIR ${PROJECT_SOURCE_DIR}/../forms)
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/../assets)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/../lib)

# needed to include Eigen
set(EIGEN_DIR ${PROJECT_SOURCE_DIR}/../../../../eigen-3-3-7)
message ("EIGEN_DIR = ${EIGEN_DIR}")

set(INC_DIR ${EIGEN_DIR})
set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES)

# add current dir to include search path
include_directories(${PROJECT_SOURCE_DIR} ${INC_DIR})

add_subdirectory(io)
add_subdirectory(util)
add_subdirectory(wrl)

#library list
set(LIB_LIST Qt5::Core Qt5::Widgets Qt5::Svg io util wrl)

# build command line executable wrlTest
add_subdirectory(test)

# build interactive application WrlViewer1
add_subdirectory(viewer)


