project (PlateRecognizer CXX)
include_directories(../include
					${OPENCVINCLUDEDIR})

set(SOURCES_FILES	./main.cpp
					./Image.cpp
					./Plate.cpp
					./PlateRecognizer.cpp
					./NeuralNetwork.cpp
					)
					
add_executable(${PROJECT_NAME} ${SOURCES_FILES})

target_link_libraries(${PROJECT_NAME} 
	debug 		${OPENCVLIBDIR}/opencv_world310d.lib
	debug 		${MACHIN_PATH}/debug/DllForUnity.lib
	optimized	${OPENCVLIBDIR}/opencv_world310.lib
	optimized	${MACHIN_PATH}/release/DllForUnity.lib
	)