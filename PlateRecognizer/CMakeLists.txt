cmake_minimum_required (VERSION 2.8.4)

project (PlateRecognizer CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(FIRST_RUN OFF CACHE BOOL "First Run of CMake")
set(OPENCV_PATH "opencv/build" CACHE STRING "Path to opencv/build path")

if (CMAKE_GENERATOR STREQUAL "Visual Studio 14 2015 Win64")
set(OPENCVBINDIR "${OPENCV_PATH}/x64/vc14/bin")
set(OPENCVLIBDIR "${OPENCV_PATH}/x64/vc14/lib")
elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 12 2013 Win64")
set(OPENCVBINDIR "${OPENCV_PATH}/x64/vc12/bin")
set(OPENCVLIBDIR "${OPENCV_PATH}/x64/vc12/lib")
else()
set(OPENCVBINDIR "${OPENCV_PATH}/x64/vc15/bin")
set(OPENCVLIBDIR "${OPENCV_PATH}/x64/vc15/lib")
endif()
set(OPENCVINCLUDEDIR "${OPENCV_PATH}/include")

include_directories(${OPENCVINCLUDEDIR})
include_directories(./MNIST/include)

set(PLATFORM "x64")

if(FIRST_RUN)
	file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
	file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)

	file(COPY ${OPENCVBINDIR}/opencv_world430.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
	file(COPY ${OPENCVBINDIR}/opencv_world430d.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
endif(FIRST_RUN)

add_subdirectory(MNIST)
add_subdirectory(src)
