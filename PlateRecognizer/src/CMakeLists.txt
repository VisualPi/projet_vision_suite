project (PlateRecognizer CXX)

include_directories(
                 PUBLIC 
                    ../include)
					
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
			)

target_link_libraries(${PROJECT_NAME}
	debug		MNIST
	optimized   MNIST
	debug 		${OPENCVLIBDIR}/opencv_world430d.lib
	optimized	${OPENCVLIBDIR}/opencv_world430.lib
	)