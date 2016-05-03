#include <opencvinlcudes.h>
#include "PlateRecognizer.h"

std::string window_name = "Plate Recognizer";

#define CREATE_DATASET 1

int main( int argc, char** argv )
{
	try
	{
#if CREATE_DATASET == 0
		std::ifstream ifs( "C:\\Users\\Jonathan\\Documents\\ESGI\\5A\\machine_learning\\datasets\\English\\toto.log", std::ios::in );
		std::fstream fimages( "C:\\Users\\Jonathan\\Documents\\ESGI\\5A\\machine_learning\\datasets\\English\\images", std::ios::out | std::ios::binary );
		std::fstream flabels( "C:\\Users\\Jonathan\\Documents\\ESGI\\5A\\machine_learning\\datasets\\English\\labels", std::ios::out );
		std::string line;
		std::vector<std::string> chars{ "0","1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };
		int i = 0, cpt = 0;
		while ( std::getline( ifs, line ) )
		{
			cv::Mat tmp = cv::imread( line, CV_LOAD_IMAGE_GRAYSCALE );
			cv::resize( tmp, tmp, cv::Size( 20, 20 ) );
			for ( int i = 0 ; i < 20 ; ++i )
				for ( int j = 0 ; j < 20 ; ++j )
					fimages << ( static_cast<int>( tmp.data[i * 20 + j] ) == 255 ? 1 : 0 );
			fimages << std::endl;
			flabels << chars[i] << std::endl;
			if ( cpt == 1016 )
			{
				i++;
				cpt = 0;
			}
			cpt++;
		}
		fimages.close();
		flabels.close();
		ifs.close();
		return 0;
#endif

		/////////////////////////////////////////////////////
		cv::namedWindow( window_name, CV_WINDOW_AUTOSIZE );
		PlateRecognizer pr( "C:\\Users\\Jonathan\\Documents\\ESGI\\5A\\projet_vision_suite\\trunk\\images\\plates" );
		pr.Process();
	}
	catch ( std::exception& e )
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

