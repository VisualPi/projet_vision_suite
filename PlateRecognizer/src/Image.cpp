#include <Image.h>

Image::Image()
{
	plates_ = std::vector<Plate>();
}
Image::Image( const cv::Mat& image ) : image_(image), plates_(std::vector<Plate>())
{}
Image::Image( const cv::Mat& image, const std::vector<Plate>& plates ) : image_(image), plates_(plates)
{}
Image::Image( const Image& image ) : image_(image.image_), plates_(image.plates_)
{}
Image::~Image()
{}

