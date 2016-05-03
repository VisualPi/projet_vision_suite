#pragma once
#include <opencvinlcudes.h>
#include "Plate.h"
class Image
{
public:
	Image();
	Image( const cv::Mat& image );
	Image( const cv::Mat& image, const std::vector<Plate>& plates );
	Image( const Image& image );
	~Image();

	cv::Mat GetImage() const { return image_; };
	std::vector<Plate>& GetPlates() { return plates_; };
	void SetPlates( const std::vector<Plate>& plates ) { plates_ = plates; }
private:
	cv::Mat image_;
	std::vector<Plate> plates_;
	
};