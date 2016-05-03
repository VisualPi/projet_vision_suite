#pragma once

#include <opencvinlcudes.h>

namespace NeuralNetwork
{
	class KNN
	{
	public:
		KNN();
		~KNN();
		bool LoadTrainedData();
		bool SaveTrainData();
		float Predict(cv::Mat sample);
		void Train();
	private:
		cv::Ptr<cv::ml::KNearest> knn_;
		
	};
}