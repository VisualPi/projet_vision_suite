#include <opencvincludes.h>
#include "PlateRecognizer.h"

std::string window_name = "Plate Recognizer";

int main( int argc, char** argv )
{
	try
	{
		if (argv[1] == "")
		{
			std::cerr << "No input folder." << std::endl;
			return -1;
		}

		/////////////////////////////////////////////////////
		cv::namedWindow( window_name, cv::WINDOW_AUTOSIZE );
		PlateRecognizer pr(argv[1]);
		pr.Process();
	}
	catch ( const std::exception& e )
	{
		std::cout << "STD Exception found : " << e.what() << std::endl;
		return -1;
	}
	catch ( ... )
	{
		std::cout << "Other Exception catch: " << std::endl;
		return -1;
	}
	return 0;
}

