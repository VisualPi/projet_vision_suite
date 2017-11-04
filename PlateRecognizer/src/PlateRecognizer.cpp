#include "PlateRecognizer.h"

#define	MOY_MAX 26000
#define	MOY_MIN 3000
const bool showSteps = false; //show all steps of the analyse
const bool showResult = true;//show the result in the Detection function (useful for HARD_CHECK "debugging")
const std::string window_name = "Plate Recognizer";
const std::vector<char> chars{ '0','1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

PlateRecognizer::PlateRecognizer() : images_( std::vector<Image>() )/*, knn_( cv::ml::KNearest::create() )*/
{
}
PlateRecognizer::PlateRecognizer( const std::string& path ) : images_( std::vector<Image>() ) /*,knn_( cv::ml::KNearest::create())*/
{
	std::string cmd = "dir " + path + " /b > c:\\temp\\foo.txt"; //commande windows pour mettre dans un fichier toutes les photos d'un dossier
	system( cmd.c_str() );
	std::ifstream ifs( "c:\\temp\\foo.txt", std::ios::in );//lecture du fichier rempli précédemment
	std::string line( "" );
	if ( ifs.is_open() )
	{
		while ( getline( ifs, line ) )
		{
			cv::Mat tmp = cv::imread( path + "\\" + line, CV_LOAD_IMAGE_COLOR ); //chargement de chaque photo
			images_.push_back( tmp );//stylay (pas besoin d'appeler le contructeur : Image(tmp), il le fait implicitement)
		}
		ifs.close();
	}

}
PlateRecognizer::PlateRecognizer( const PlateRecognizer& plateRecognizer ) : images_( plateRecognizer.images_ ) /*,knn_( plateRecognizer.knn_ )*/
{}

PlateRecognizer::~PlateRecognizer()
{
	images_.clear();
}

void PlateRecognizer::Process()
{
	//knn_ = cv::Algorithm::load < cv::ml::KNearest>( "C:\\Users\\Jonathan\\Documents\\ESGI\\5A\\machine_learning\\datasets\\savedDatas"); //chargement des données entrainées
	for ( std::vector<Image>::iterator it = images_.begin(); it != images_.end(); ++it )//pour chacune des images du dossier
	{
		cv::Mat image1 = ( *it ).GetImage();//on stock dans une variable temp l'image en cours
		std::vector<Plate> tmpplates = std::vector<Plate>();
		if ( Detection( image1, tmpplates ) )//detections des plaques temporaires
		{
			( *it ).SetPlates( tmpplates );
			cv::Mat result;
			image1.copyTo( result );
			if ( showResult )//si jamais ce booleen est a true on affiche les rectangle autours des plaques détectées
			{
				for ( std::vector<Plate>::const_iterator pit = ( *it ).GetPlates().cbegin(); pit != ( *it ).GetPlates().cend(); ++pit )
				{
					cv::Point2f rect_points[4];
					( *pit ).GetRect().points( rect_points );
					for ( int j = 0; j < 4; j++ )
						line( result, rect_points[j], rect_points[( j + 1 ) % 4], cv::Scalar( 255, 100, 0 ), 5, 8 );
				}
				cv::imshow( "PlateRecognizer", result );
				cv::waitKey();
			}
			for ( std::vector<Plate>::iterator pit = ( *it ).GetPlates().begin(); pit != ( *it ).GetPlates().end(); ++pit )//pour chacunes des plaques detectées
			{
				CharactersDetection( *pit );//on detecte les caractere et on rempli le vector chars de la Plate courante
				std::cout << "number of chars detected = " << ( *pit ).GetChars().size() << std::endl;
			}
			const int size = 20; //la taille de chaque caractere (a etre redimensionné)
			const int tolerence = 60; //la tolerance : chaque pixel est entre 0 et 255 pour du nb on choisis une valeur tolerance pour dire : plus petit que la tolerance = 0 sinon 1
			for ( std::vector<Plate>::const_iterator pit = ( *it ).GetPlates().cbegin(); pit != ( *it ).GetPlates().cend(); ++pit )//pour chacune des plaques
			{
				std::vector<char> letters = std::vector<char>();
				for ( std::vector<Chars>::const_iterator cit = ( *pit ).GetChars().cbegin(); cit != ( *pit ).GetChars().cend(); ++cit )//pour chacuns des carateres de la plaque
				{
					cv::Mat resized;
					cv::resize( ( *cit ).m_char, resized, cv::Size( size, size ) );//on resize en size*size (meme valeur que pour le dataset)					
					cv::Mat matResults( 1, 20 * 20, CV_32F );
					for ( int i = 0; i < 20 * 20; ++i )
						matResults.at<float>( cv::Point( i, 0 ) ) = ( (int) ( resized.data[i] ) > tolerence ) ? 0 : 1; //on change la valeur des pixels en 0 ou 1 (comme pour le dataset)
					//int f = knn_->predict( matResults );//resultat du predict avec le caractere courant
					//std::cout << "result: " << chars[f] << std::endl;//affichage du caractere trouvé par le KNN
					//if (f != -1)//-1 pour le bruit, pas encore geré
					//	letters.push_back( chars[f] );//remplissage de la chaine de caractere correspondante a la plaque
					if ( showResult )
					{
						cv::imshow( "Chars", resized );
						cv::waitKey();
					}
				}
				std::cout << "Found for this image : ";
				for ( auto it = letters.begin() ; it != letters.end() ; ++it )
					std::cout << *it;//affichage de la chaine de caractere trouvé correspondante a la plaque
				std::cout << std::endl;
			}
		}
		else
			std::cout << "No Plate found in this picture" << std::endl;
	}
}


//Role: 	valider les plaques (rect) potentielles, en fonction des dimensions et de l'aire
//Entrées:	contours (const std::vector<cv::Point>&)
//Sorties:	booleen, true si dimension et aire bonne, false sinon
bool PlateRecognizer::Validate( const std::vector<cv::Point>& cont )
{
	cv::RotatedRect rect = cv::minAreaRect( cv::Mat( cont ) );
	bool bRet = false;
	int width = rect.boundingRect().width;
	int height = rect.boundingRect().height;
	if ( ( width > height ) && ( width / height >= 2 ) )
	{
		int moy = height * width;
		//std::cout << "moy : " << moy << std::endl;
		if ( ( moy <= MOY_MAX ) && ( moy >= MOY_MIN ) )
			bRet = true;
	}
	return bRet;
}
//Role: 	Detecter les plaques d'immatriculation potentielles
//Entrées:	Image a traiter (const cv::Mat&)
//Sorties:	liste de plaque d'immatriculation (std::vector<Plate>&)
bool PlateRecognizer::Detection( const cv::Mat& input_image, std::vector<Plate>& plates )
{
	cv::Mat gray;
	cv::cvtColor( input_image, gray, cv::COLOR_BGR2GRAY );
	if ( showSteps )
	{
		cv::imshow( "test", gray );
		cv::waitKey();
	}
	cv::Mat blur;
	cv::GaussianBlur( gray, blur, cv::Size( 5, 5 ), 0 );
	if ( showSteps )
	{
		cv::imshow( "test", blur );
		cv::waitKey();
	}
	cv::Mat sobel;
	cv::Sobel( blur, sobel, CV_8U, 1, 0, 3 );
	if ( showSteps )
	{
		cv::imshow( "test", sobel );
		cv::waitKey();
	}
	cv::Mat tres;
	threshold( sobel, tres, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU );
	if ( showSteps )
	{
		cv::imshow( "test", tres );
		cv::waitKey();
	}
	cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size( 23, 2 ) );
	cv::Mat closing;
	cv::morphologyEx( tres, closing, cv::MORPH_CLOSE, element );
	if ( showSteps )
	{
		cv::imshow( "test", closing );
		cv::waitKey();
	}
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours( closing, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE );
	if ( showSteps )
	{
		cv::Mat contourImg;
		closing.copyTo( contourImg );
		cv::drawContours( contourImg, contours,
						  -1, // draw all contours
						  cv::Scalar( 255, 0, 0 ), // in blue
						  1 ); // with a thickness of 1
		cv::imshow( "test", contourImg );
		cv::waitKey();
	}
	std::vector<std::vector<cv::Point> >::iterator itc = contours.begin();
	cv::Mat result;
	input_image.copyTo( result );
	while ( itc != contours.end() )
	{
		if ( Validate( *itc ) )
		{
			cv::RotatedRect rect = cv::minAreaRect( cv::Mat( *itc ) );
			cv::Rect r = rect.boundingRect();
			if ( r.x + r.width >= input_image.size().width )
				r.width -= std::abs( ( input_image.size().width - ( r.x + r.width ) ) );
			if ( r.y + r.height >= input_image.size().height )
				r.height -= std::abs( ( input_image.size().height - ( r.y + r.height ) ) );

			if ( r.x < 0 )
				r.x = 0;
			if ( r.y < 0 )
				r.y = 0;

			std::cout << rect.boundingRect() << std::endl;
			rect.boundingRect() = r;
			std::cout << rect.boundingRect() << std::endl;
			cv::Mat pMat = input_image( r );
			plates.push_back( Plate( pMat, rect ) );
			++itc;
		}
		else
			itc = contours.erase( itc );
	}
	return plates.size() > 0 ? true : false;
}
//Role: 	Recentre et resize le caractere
//Entrées:	caractere a traiter (const cv::Mat&)
//Sorties:	caractrere traité (cv::Mat)
cv::Mat PlateRecognizer::preprocessChar( const cv::Mat& in )
{
	//Remap image
	int h = in.rows;
	int w = in.cols;
	cv::Mat transformMat = cv::Mat::eye( 2, 3, CV_32F );
	int m = std::max( w, h );
	transformMat.at<float>( 0, 2 ) = m / 2 - w / 2;
	transformMat.at<float>( 1, 2 ) = m / 2 - h / 2;

	cv::Mat warpImage( m, m, in.type() );
	warpAffine( in, warpImage, transformMat, warpImage.size(), CV_INTER_LINEAR, IPL_BORDER_CONSTANT, cv::Scalar( 0 ) );

	cv::Mat out;
	resize( warpImage, out, cv::Size( 20, 20 ) );
	if ( showSteps )
	{
		cv::imshow( "wrapped char", warpImage );
		cv::waitKey();
	}
	return out;
}
//Role: 	Validation des caractere (partie pratiquement enlever pour laisser ça au NN, on regarde seulement si le caractere a une aire superieure a 100)
//Entrées:	caractere a valider (const cv::Mat&)
//Sorties:	booleen, true si caractere valide, false sinon
bool PlateRecognizer::ValidateChar( const cv::Mat& in )
{
	if ( in.size().width < in.size().height )
		if ( in.size().width * in.size().height > 100 )
			return true;
	return false;

	//float aspect = 45.0f / 77.0f;
	//float charAspect = (float) in.cols / (float) in.rows;
	//float error = 0.35;
	//float minHeight = 15;
	//float maxHeight = 28;
	////We have a different aspect ratio for number 1, and it can be ~0.2
	//float minAspect = 0.2;
	//float maxAspect = aspect + aspect*error;
	////area of pixels
	//float area = cv::countNonZero( in );
	////bb area
	//float bbArea = in.cols*in.rows;
	////% of pixel in area
	//float percPixels = area / bbArea;

	//std::cout << "Aspect: " << aspect << " [" << minAspect << "," << maxAspect << "] " << "Area " << percPixels << " Char aspect " << charAspect << " Height char " << in.rows << "\n";
	//if ( percPixels < 0.8 && charAspect > minAspect && charAspect < maxAspect && in.rows >= minHeight && in.rows < maxHeight )
	//	return true;
	//else
	//	return false;

}
//Role: 	Detection des caractere dans une plaque
//Entrées:	la plaque d'immatriculation (entree/sortie)
//Sorties:	La meme plaque avec les caractere dans un vector<Chars>
void PlateRecognizer::CharactersDetection( Plate& plate )
{
	cv::Mat input = plate.GetImage();
	cv::Mat gray;
	cv::cvtColor( input, gray, cv::COLOR_BGR2GRAY );
	//Threshold input image
	cv::Mat img_threshold;
	cv::Canny( gray, img_threshold, 100, 100 * 2 );
	if ( showSteps )
	{
		imshow( "Threshold plate", img_threshold );
		cv::waitKey();
	}

	cv::Mat img_contours;
	img_threshold.copyTo( img_contours );
	//Find contours of possibles characters
	std::vector< std::vector< cv::Point> > contours;
	findContours( img_contours,
				  contours, // a vector of contours
				  CV_RETR_EXTERNAL, // retrieve the external contours
				  CV_CHAIN_APPROX_NONE ); // all pixels of each contours

	if ( showSteps )
	{
		imshow( "contours", img_contours );
		cv::waitKey();
	}

	// Draw blue contours on a white image
	cv::Mat result;
	img_threshold.copyTo( result );
	cvtColor( result, result, CV_GRAY2RGB );
	cv::drawContours( result, contours,
					  -1, // draw all contours
					  cv::Scalar( 255, 0, 0 ), // in blue
					  1 ); // with a thickness of 1

						   //Start to iterate to each contour founded
	std::vector<std::vector<cv::Point> >::iterator itc = contours.begin();
	if ( showSteps )
	{
		cv::imshow( "result", result );
		cv::waitKey();

	}
	//Remove patch that are no inside limits of aspect ratio and area.    
	while ( itc != contours.end() )
	{

		//Create bounding rect of object
		cv::Rect mr = cv::boundingRect( cv::Mat( *itc ) );
		rectangle( result, mr, cv::Scalar( 0, 255, 0 ) );
		//Crop image
		cv::Mat auxRoi( img_threshold, mr );
		if ( ValidateChar( auxRoi ) )
		{
			mr.x -= mr.x - 2.5 <= 0 ? 0 : 2.5;
			mr.y -= mr.y - 2.5 <= 0 ? 0 : 2.5;
			if ( mr.x + mr.width + 5 < img_threshold.size().width )
				mr.width += 5;
			if ( mr.y + mr.height + 5 < img_threshold.size().height )
				mr.height += 5;
			cv::Mat toprocess( img_threshold, mr );
			auxRoi = preprocessChar( toprocess );
			plate.AddChars( auxRoi, mr );
			rectangle( result, mr, cv::Scalar( 0, 125, 255 ) );
			if ( showSteps )
			{
				cv::imshow( "rectangle", result );
				cv::waitKey();
			}
		}
		++itc;
	}
}
