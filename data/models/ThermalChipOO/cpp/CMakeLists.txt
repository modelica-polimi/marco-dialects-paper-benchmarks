cmake_minimum_required(VERSION 3.5)

project(ThermalChip VERSION 1.0.0 LANGUAGES CXX)

set (app_name "simulation")

string (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/common")
include (common) # defines common headers and cpp files, including main.cpp

set (app_src
	ModelSolution.cpp
	EulerExp/BaseThermalChip.cpp
	EulerExp/ThermalChipSimpleBoundary.cpp
	${common_src}
)

set (app_hdr
	EulerExp/BaseThermalChip.hpp
	EulerExp/ThermalChipSimpleBoundary.hpp
	${common_hdr}
)

add_executable(${app_name} ${app_src} ${app_hdr})
target_include_directories(${app_name} PRIVATE ${common_include_dir})
