#include "VTTI.h"

int main()
{
	// Read first images and display
	cv::Mat A_bgr = cv::imread("../Debug/image5.bmp"), A;
	cv::cvtColor(A_bgr, A, CV_BGR2GRAY);  //BGR to Gray
	cv::Mat B_bgr = cv::imread("../Debug/image6.bmp"), B;
	cv::cvtColor(B_bgr, B, CV_BGR2GRAY);  //BGR to Gray
	//cv::namedWindow("Image 1",0);
	//cv::imshow("Image A", A);
	//cv::namedWindow("Image B",0);
	//cv::imshow("Image B", B);
	////cv::waitKey();

	// Find the difference between the two images to find the object being moved (and display)
	cv::Mat Sub = B- A;
	cv::threshold(Sub, Sub, 100, 255, 0);
	//cv::imshow("Sub", Sub);
	//cv::waitKey();

	//Draw a rectangle around the region where the single object has moved
	cv::Point2f r1, r2;
	cv::Rect R = findRectangle(Sub, r1, r2);  // Finds the rectangular region where the difference in frames showed movement
	cv::rectangle(Sub, R, cv::Scalar(255,255,255));
	cv::putText(Sub, "Press Enter to continue", cv::Point(25,25), 0, 0.5, cv::Scalar(255,255,255));
	cv::namedWindow("Rectangular region where motion is detected", 0);
	cv::imshow("Rectangular region where motion is detected", Sub);
	cv::waitKey();

	// select the contours in the rectangular region
	std::vector<std::vector<cv::Point>> contoursA, contoursB, contoursA1, contoursB1;
	cv::Mat At, Bt;
	cv::threshold(A, At, 128, 255, 1);  //Gray scale image to Binary
	cv::threshold(B, Bt, 128, 255, 1);  //Gray scale image to binary
	cv::findContours(At, contoursA, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);  //finds all the contours in Image A
	cv::findContours(Bt, contoursB, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);  //finds all the contours in Image B
	cv::Mat A_bgr1 = A_bgr.clone();
	cv::Mat B_bgr1 = B_bgr.clone();
	// find Contours only in the rectangular region bounded by points r1 and r2
	for(int i = 0; i < contoursA.size(); i++)
	{
		std::vector<cv::Point> temp;
		for(int j = 0; j < contoursA[i].size(); j++)
		{
			int x = contoursA[i][j].x;
			int y = contoursA[i][j].y;
			if(( (x >= r1.x ) && (y >= r1.y) && (x <= r2.x) && (y <= r2.y) ))
			{
				temp.push_back(cv::Point(x,y));
			}
		}
		if(temp.data())
			contoursA1.push_back(temp);
	}
	for(int i = 0; i < contoursB.size(); i++)
	{
		std::vector<cv::Point> temp;
		for(int j = 0; j < contoursB[i].size(); j++)
		{
			int x = contoursB[i][j].x;
			int y = contoursB[i][j].y;
			if(( (x >= r1.x ) && (y >= r1.y) && (x <= r2.x) && (y <= r2.y) ))
			{
				temp.push_back(cv::Point(x,y));
			}
		}
		if(temp.data())
			contoursB1.push_back(temp);
	}
	A_bgr1 = cv::Mat(A.rows, A.cols, CV_8UC3, CV_RGB(0,0,0));
	cv::drawContours(A_bgr1, contoursA1, -1, cv::Scalar(0,0,255));
	cv::drawContours(A_bgr1, contoursB1, -1, cv::Scalar(255,0,0));
	cv::putText(A_bgr1, " Red -> Initial outline", cv::Point(25, 75), 0, 0.5, cv::Scalar(0,0,255));
	cv::putText(A_bgr1, " Blue -> Final Outline", cv::Point(25, 50), 0, 0.5, cv::Scalar(255,0,0));
	cv::putText(A_bgr1, "Press Enter to continue", cv::Point(25,25), 0, 0.5, cv::Scalar(255,255,255));
	cv::namedWindow("Initial and FInal Positions of the object", 0);
	cv::imshow("Initial and FInal Positions of the object", A_bgr1);
	cv::waitKey();
	
	// converting Matrix to a set of points
	contoursA = contoursA1;
	contoursB = contoursB1;
	std::vector<cv::Point2f> PointsA, PointsB;
	for(int i = 0; i < contoursA.size(); i++)
	{
		for(int j = 0; j < contoursA[i].size(); j++)
		{
			PointsA.push_back(contoursA[i][j]);
		}
	}
	for(int i = 0; i < contoursB.size(); i++)
	{
		for(int j = 0; j < contoursB[i].size(); j++)
		{
			PointsB.push_back(contoursB[i][j]);
		}
	}
	
	// Calculate Optical flow for Part 2 of the problem Statement
	std::vector<uchar> status;
	std::vector<float> err;
	cv::Mat C = B_bgr.clone();
	cv::calcOpticalFlowPyrLK(A, B, PointsA, PointsB, status, err); //Calculates an optical flow for a sparse feature set using the iterative Lucas-Kanade method with pyramids
	cv::Mat OP = B_bgr.clone();
	std::vector<float> D;
	float sum = 0;
	std::vector<motion> pointPositions;   // Stores the initial and final positions as a vector of pixels
	for(int i = 0; i < PointsA.size(); i++)
	{
		OP = B_bgr.clone();
		float distance = sqrt ( pow((PointsA[i].x - PointsB[i].x),2) + pow( (PointsA[i].y - PointsB[i].y),2) );
		int x1 = PointsA[i].x;		int y1 = PointsA[i].y;		int x2 = PointsB[i].x;		int y2 = PointsB[i].y;
		if ( (distance > 5) && (x1 >= r1.x ) && (y1 >= r1.y)  && (x1 <= r2.x) && (y1 <= r2.y) && (x2 >= r1.x ) && (y2 >= r1.y) && (x2 <= r2.x) && (y2 <= r2.y) )
		{
			D.push_back(distance);
			sum += distance;
			pointPositions.push_back(motion(PointsA[i], PointsB[i]));
			cv::line(OP, cv::Point(PointsA[i].x, PointsA[i].y), cv::Point(PointsB[i].x, PointsB[i].y), cv::Scalar(255,255,255));
			cv::circle(OP, PointsA[i], 2, cv::Scalar(0, 0, 255));
			cv::circle(OP, PointsB[i], 2, cv::Scalar(255, 0, 0));
			cv::putText(OP, " Red -> Initial outline", cv::Point(25, 25), 0, 0.5, cv::Scalar(0,0,255));
			cv::putText(OP, " Blue -> Final Outline", cv::Point(25, 50), 0, 0.5, cv::Scalar(255,0,0));
			cv::namedWindow("Output on Image B", 0);
			cv::imshow("Output on Image B", OP);
			cv::waitKey(25);
		}
	}
	std::cout << std::endl << "Average Distance = "  << sum/D.size();
	return 1;
}