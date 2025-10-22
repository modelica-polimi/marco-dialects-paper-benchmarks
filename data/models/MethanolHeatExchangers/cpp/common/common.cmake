cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE)
  message("No build type detected. Build type set to Release by default")
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS_RELEASE          "-O3")
set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O3 -g")
set(CMAKE_CXX_FLAGS_RELEASE        ${CMAKE_C_FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_RELWITHDEBINFO})

set(CMAKE_CXX_STANDARD 14)

set (common_src_prefix "${CMAKE_CURRENT_LIST_DIR}/src")

set (common_src
		${common_src_prefix}/main.cpp
		${common_src_prefix}/TimeStatAccumulator.cpp
		${common_src_prefix}/Event.cpp
		${common_src_prefix}/matrix.cpp
		${common_src_prefix}/OptionsMap.cpp
	)

set (common_hdr_prefix "${CMAKE_CURRENT_LIST_DIR}/include")

set (common_hdr
		${common_hdr_prefix}/CsvEmitter.hpp
		${common_hdr_prefix}/Emitter.hpp
		${common_hdr_prefix}/ModelSolutionI.hpp
		${common_hdr_prefix}/OptionsMap.hpp
		${common_hdr_prefix}/SpMat.hpp
		${common_hdr_prefix}/TimeStatAccumulator.hpp
		${common_hdr_prefix}/types.hpp
		${common_hdr_prefix}/SpMat_Eigen.hpp
		${common_hdr_prefix}/SpMat_KLU.hpp
		${common_hdr_prefix}/Event.hpp
		${common_hdr_prefix}/matrix.hpp
        ${common_hdr_prefix}/Mat.hpp
	)

set (common_include_dir ${common_hdr_prefix})

mark_as_advanced (common_src common_hdr common_include_dir)
