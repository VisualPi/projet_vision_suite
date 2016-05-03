#pragma once
#include <opencvinlcudes.h>
#include <Util.hpp>
#include <Plate.h>
#include <Image.h>
#include <NeuralNetwork.h>

class PlateRecognizer
{
public:
	PlateRecognizer();
	PlateRecognizer( const std::string& path);
	PlateRecognizer( const PlateRecognizer& plateRecognizer );
	~PlateRecognizer();

	bool Process();
	bool Detection( cv::Mat& input_image, std::vector<Plate>&);
	void CharactersDetection( Plate& plate );
	cv::Mat features( cv::Mat in, int sizeData );
	cv::Mat preprocessChar( cv::Mat in );
	int classify( cv::Mat f );
	bool Validate( std::vector<cv::Point>& cont );
	bool ValidateChar( cv::Mat in );
	void train( cv::Mat TrainData, cv::Mat classes, int nlayers );
	bool TrainMLP();
private:
	std::vector<Image> images_;
	cv::Ptr<cv::ml::KNearest> knn_;
	NeuralNetwork::KNN knn;
};