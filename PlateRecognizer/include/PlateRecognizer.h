#pragma once
#include <opencvincludes.h>
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

	void Process();
	bool Detection(const cv::Mat& input_image, std::vector<Plate>& plates);
	void CharactersDetection( Plate& plate );
	cv::Mat features( cv::Mat in, int sizeData );
	cv::Mat preprocessChar(const cv::Mat& in);
	int classify( cv::Mat f );
	bool Validate(const std::vector<cv::Point>& cont);
	bool ValidateChar(const cv::Mat& in);
	void train( cv::Mat TrainData, cv::Mat classes, int nlayers );
	bool TrainMLP();

private:
	std::vector<Image> images_;
	cv::Ptr<cv::ml::KNearest> knn_;
	//NeuralNetwork::KNN knn;
};