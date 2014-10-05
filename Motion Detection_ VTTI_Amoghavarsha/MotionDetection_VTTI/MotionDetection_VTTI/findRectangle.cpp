#include "VTTI.h"

cv::Rect findRectangle(cv::Mat Sub,
					   cv::Point2f &r1, 
					   cv::Point2f &r2)
{
	cv::Point2f top, bottom, left, right;
	//top most point
	bool set = false;
	for(int i = 0; i < Sub.rows; i++)
	{
		for(int j = 0; j < Sub.cols; j++)
		{
			if( Sub.at<uchar>(i,j) != 0)
			{
				top = cv::Point2f(j,i);
				set = true;
				break;
			}
		}
		if(set == true)
			break;
	}

	// bottom most point
	set = false;
	for(int i = Sub.rows -1; i > 0; i--)
	{
		for(int j = Sub.cols -1; j > 0; j--)
		{
			if( Sub.at<uchar>(i,j) != 0)
			{
				bottom = cv::Point2f(j,i);
				set = true;
				break;
			}
		}
		if(set == true)
			break;
	}

	//right most point
	set = false;
	for(int i = Sub.cols-1; i > 0; i--)
	{
		for(int j = Sub.rows-1; j > 0; j--)
		{
			if( Sub.at<uchar>(j,i) != 0)
			{
				right = cv::Point2f(i,j);
				set = true;
				break;
			}
		}
		if(set == true)
			break;
	}

	//left most point
	set = false;
	for(int i = 0; i < Sub.cols; i++)
	{
		for(int j = 0; j < Sub.rows; j++)
		{
			if( Sub.at<uchar>(j,i) != 0)
			{
				left = cv::Point2f(i,j);
				set = true;
				break;
			}
		}
		if(set == true)
			break;
	}

	r1 = cv::Point2f(left.x, top.y);
	r2 = cv::Point2f(right.x, bottom.y);
	return cv::Rect(r1,r2);
}