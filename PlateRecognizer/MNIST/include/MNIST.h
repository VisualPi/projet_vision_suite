#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "opencv2/opencv.hpp"

class MNIST
{
public:
    class Dataset
    {
    public:
        cv::Mat m_images;
        cv::Mat m_labels;

        Dataset();
        void Fill(uchar** imgs, uchar* lbls, int nbImg, int sizeX = 28, int sizeY = 28);
    };

    MNIST();

    void readDataset(const std::string& imagesFile, const std::string& labelsFile, int numberOfImages, int sizeX, int sizeY);
    void train()
    {
        if (m_isFilled)
        {
            cv::Ptr<cv::ml::TrainData> trainingData;
            cv::Ptr<cv::ml::KNearest>  kclassifier = cv::ml::KNearest::create();
            
            trainingData = cv::ml::TrainData::create(
                m_dataset.m_images,
                cv::ml::ROW_SAMPLE,
                m_dataset.m_labels);
            
            kclassifier->setIsClassifier(true);
            kclassifier->setAlgorithmType(cv::ml::KNearest::BRUTE_FORCE);
            kclassifier->setDefaultK(1);

            bool b = kclassifier->train(trainingData);
            //kclassifier->findNearest(matSample, kclassifier->getDefaultK(), matResults);
            
            
            
            std::cout << "train returns : " << b << std::endl;
        }
        else
        {
            std::cerr << "Read dataset before training /!\\" << std::endl;
        }
    }

    Dataset m_dataset;
    cv::Mat m_trainedData;

private:
    uchar** readImages(std::string full_path, int& number_of_images, int& image_size);
    uchar*  readLabels(std::string full_path, int& number_of_labels);
    
    bool    m_isFilled;
};

