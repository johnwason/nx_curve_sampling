cmake_minimum_required(VERSION 3.3)
project(nx_curve_sampling)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

find_package(NX REQUIRED)

add_library(${PROJECT_NAME} SHARED nx_curve_sampling.cpp nx_curve_sampling.h)
target_link_libraries(${PROJECT_NAME} NX::nxopencpp NX::ufun NX::ugopenint)
target_compile_definitions(${PROJECT_NAME} PRIVATE -DNXCURVESAMPLING_EXPORTS=1)