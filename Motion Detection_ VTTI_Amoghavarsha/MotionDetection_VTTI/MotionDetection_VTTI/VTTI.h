#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>
#include <vector>

class motion
{
	// This class is meant to hold the initial and final locations of the pixel
	cv::Point initial;
	cv::Point final;
public:
	motion() {}
	motion(cv::Point i, cv::Point f)
	{
		initial = i;
		final = f;
	}
	void getValues(cv::Point &i, cv::Point &f)
	{
		i = initial;
		f = final;
	}
};

cv::Rect findRectangle(cv::Mat Sub,
					   cv::Point2f &r1, 
					   cv::Point2f &r2);