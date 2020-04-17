project (PlateRecognizer CXX)
include_directories(../include
				    ../MNIST/include
					${OPENCVINCLUDEDIR})
					
add_executable( ${PROJECT_NAME} 
			./main.cpp
			./Image.cpp
			./Plate.cpp
			./PlateRecognizer.cpp
			../include/Image.h
			../include/Image.hpp
			../include/NeuralNetwork.h
			../include/opencvincludes.h
			../include/Plate.h
			../include/PlateRecognizer.h
			../include/Util.hpp
			#./NeuralNetwork.cpp
			)

target_link_libraries(${PROJECT_NAME}
	debug		MNIST
	optimized   MNIST
	debug 		${OPENCVLIBDIR}/opencv_world310d.lib
	optimized	${OPENCVLIBDIR}/opencv_world310.lib
	)