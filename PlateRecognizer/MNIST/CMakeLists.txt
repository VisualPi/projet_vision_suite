project (MNIST CXX)

include_directories(./include
					${OPENCVINCLUDEDIR})

add_library(${PROJECT_NAME}
		SHARED
			./src/MNIST.cpp
			./include/MNIST.h)

target_link_libraries(${PROJECT_NAME}
	debug 		${OPENCVLIBDIR}/opencv_world310d.lib
	optimized	${OPENCVLIBDIR}/opencv_world310.lib
	)