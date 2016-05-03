#pragma once

class Image
{
public:
	Image();
	~Image();

private:
	cv::Mat image;
	std::vector<Plate> plates;
	
};