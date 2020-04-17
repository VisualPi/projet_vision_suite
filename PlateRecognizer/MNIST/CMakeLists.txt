project (MNIST CXX)

include_directories(PRIVATE ./include)

add_library(${PROJECT_NAME} ./src/MNIST.cpp)


target_link_libraries(${PROJECT_NAME}
	debug 		${OPENCVLIBDIR}/opencv_world430d.lib
	optimized	${OPENCVLIBDIR}/opencv_world430.lib
	)