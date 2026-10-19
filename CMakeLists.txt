cmake_minimum_required(VERSION 3.5)

project(LinAlg LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

add_executable(LinAlg)

target_include_directories(LinAlg PRIVATE include)

target_sources(LinAlg PRIVATE
	src/matrix.c
	src/array.c
)





