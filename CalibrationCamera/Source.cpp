// Import OpenCV modules
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <algorithm>
#include <cmath>

// Methods
double calculateDist(double x1, double y1, double x2, double y2);
cv::Point2f findMidpoint(cv::Point2f p1, cv::Point2f p2);
cv::Point2f findArucoCenter(std::vector<cv::Point2f> marker);

// Static variables
cv::VideoCapture _capture;
cv::Ptr<cv::aruco::Dictionary> _dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_50);
int marker = 16;

int main()
{
	try {
		_capture.open(0);
	}
	catch (const std::exception e) {
		return 1;
	}

	while (_capture.grab())
	{
		cv::Mat image, imageCopy;
		_capture.retrieve(image); 
		image.copyTo(imageCopy);

		std::vector<int> ids;
		std::vector<std::vector<cv::Point2f>> corners;
		cv::aruco::detectMarkers(image, _dictionary, corners, ids);

		// if at least one marker is detected

		if (ids.size() > 0)
		{
			// See if markers 0, 1, and 16 are being seen
			auto itr0 = std::find(ids.begin(), ids.end(), 0);
			auto itr1 = std::find(ids.begin(), ids.end(), 1);
			auto itr16 = std::find(ids.begin(), ids.end(), 16);

			if (itr0 != ids.end() && itr1 != ids.end() && itr16 != ids.end())
			{
				int indexOfMarker0 = itr0 - ids.begin();
				int indexOfMarker1 = itr1 - ids.begin();
				int indexOfMarker16 = itr16 - ids.begin();

				std::vector<cv::Point2f> marker0 = corners[indexOfMarker0];
				std::vector<cv::Point2f> marker1 = corners[indexOfMarker1];
				std::vector<cv::Point2f> marker16 = corners[indexOfMarker16];

				cv::Point2f topMidpoint = findMidpoint(marker0[1], marker1[1]);
				cv::Point2f midLineMidpoint = findMidpoint(marker0[0], marker1[2]);
				cv::Point2f botMidpoint = findMidpoint(marker0[3], marker1[3]);

				cv::line(imageCopy, marker0[0], marker1[2], cv::Scalar(0, 0, 255), 1);
				cv::line(imageCopy, marker0[1], marker1[1], cv::Scalar(0, 0, 255), 1);
				cv::line(imageCopy, marker0[3], marker1[3], cv::Scalar(0, 0, 255), 1);

				cv::circle(imageCopy, topMidpoint, 3, cv::Scalar(0, 255, 0), 0);
				cv::circle(imageCopy, midLineMidpoint, 3, cv::Scalar(0, 255, 0));
				cv::circle(imageCopy, botMidpoint, 3, cv::Scalar(0, 255, 0), 0);

				// Distance between the top and bottom midpoints
				double physicalDistance = calculateDist(topMidpoint.x, topMidpoint.y, botMidpoint.x, botMidpoint.y);

				// Distance between the top right and bottom left corners of marker 16
				double virtualDistance = calculateDist(marker16[1].x, marker16[1].y, marker16[3].x, marker16[3].y);

				std::cout << marker16 << std::endl;

				double diff = physicalDistance - virtualDistance;
				std::cout << "Phys Dist: " << physicalDistance << "\nVirt Dist: " << virtualDistance << "\nDiff: " << diff << std::endl;
			}

			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
			//0 to 3
			/*
			* Calculate movement on X
			* 
			* Bottom right marker and top right marker
			*
			* moveXamt = abs(topRightId0.x - topRightId3.x)
			*/

			/*cv::Point2f topRightId0 = sortedCorners[0][1];
			cv::Point2f topRightId3 = sortedCorners[3][1];

			cv::line(imageCopy, topRightId0, topRightId3, cv::Scalar(0, 0, 255), 1);
			dist = calculateDist(topRightId3.x, topRightId3.y, topRightId0.x, topRightId0.y);
			std::cout << dist << "px" << std::endl;*/

			

			/*
			* Calculate movement on Y
			* 
			* Top left marker and top right marker
			* 
			* moveYamt = abs(bottomRightId1.y - bottomRightId3.y)
			*/
			
			/*cv::Point2f bottomRightId1 = sortedCorners[1][2];
			cv::Point2f bottomRightId3 = sortedCorners[3][2];

			cv::line(imageCopy, bottomRightId1, bottomRightId3, cv::Scalar(0, 0, 255), 1);
			dist = calculateDist(bottomRightId3.x, bottomRightId3.y, bottomRightId1.x, bottomRightId1.y);
			std::cout << dist << "px" << std::endl;*/

			
			/*std::string distText = std::to_string(dist) + " px.";
			cv::Point2f midpoint = findMidpoint(topRightId0, topRightId3);

			cv::putText(imageCopy, distText, midpoint, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1, true);*/


			//cv::Point2f topLeftId0 = sortedCorners[0][0];
			//cv::Point2f topLeftId3 = sortedCorners[3][0];

			//cv::line(imageCopy, topLeftId0, topLeftId3, cv::Scalar(0, 0, 255), 1);
			//dist = calculateDist(topLeftId3.x, topLeftId3.y, topLeftId0.x, topLeftId0.y);
			//std::cout << dist << "px" << std::endl;

			/*distText = std::to_string(dist) + " px.";
			midpoint = findMidpoint(topLeftId0, topLeftId3);

			cv::putText(imageCopy, distText, midpoint, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1, true);*/


			/*
			* Top left marker and top right marker
			*
			* moveYamt = abs(bottomRightId1.y - bottomRightId3.y)
			*/
			//cv::Point2f topRightId1 = sortedCorners[1][1];
			////cv::Point2f topRightId3 = sortedCorners[3][1];

			//cv::line(imageCopy, topRightId1, topRightId3, cv::Scalar(0, 0, 255), 1);
			//dist = calculateDist(topRightId3.x, topRightId3.y, topRightId1.x, topRightId1.y);
			//std::cout << dist << "px" << std::endl;

			/*distText = std::to_string(dist) + " px.";
			midpoint = findMidpoint(topRightId1, topRightId3);*/



			/*cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);

			cv::line(imageCopy, sortedCorners[0][0], sortedCorners[1][2], cv::Scalar(0, 0, 255), 1);
			dist = calculateDist(sortedCorners[0][0].x, sortedCorners[0][0].y, sortedCorners[1][2].x, sortedCorners[1][2].y);
			std::cout << dist << "px" << std::endl;

			std::string distText = std::to_string(dist) + " px.";
			cv::Point2f midpoint = findMidpoint(sortedCorners[0][0], sortedCorners[1][2]);

			cv::putText(imageCopy, distText, midpoint, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1, true);*/
		}
		/*bool aruco0and1 = std::find(ids.begin(), ids.end(), 0) != ids.end() && std::find(ids.begin(), ids.end(), 1) != ids.end();

		double dist = -1.0;
		if (ids.size() > 1 && aruco0and1)
		{
			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);

			cv::line(imageCopy, sortedCorners[0][0], sortedCorners[1][2], cv::Scalar(0, 0, 255), 1);
			double dist = calculateDist(sortedCorners[0][0].x, sortedCorners[0][0].y, sortedCorners[1][2].x, sortedCorners[1][2].y);
			std::cout << dist << "px" << std::endl;

			std::string distText = std::to_string(dist) + " px.";
			cv::Point2f midpoint = findMidpoint(sortedCorners[0][0], sortedCorners[1][2]);

			cv::putText(imageCopy, distText, midpoint, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1, true);
		}*/

		cv::imshow("out", imageCopy);
		char key = (char)cv::waitKey(0);
		if (key == 20)
			break;
	}
}

double calculateDist(double x1, double y1, double x2, double y2)
{
	double dist = sqrt(pow((x2 - x1), 2.0) + pow((y2 - y1), 2.0));
	return dist;
}

cv::Point2f findArucoCenter(std::vector<cv::Point2f> marker)
{
	cv::Point2f	center(0, 0);

	for (const auto& corner : marker)
	{
		center += corner;
	}

	center /= 4.f;

	return center;
}

cv::Point2f findMidpoint(cv::Point2f p1, cv::Point2f p2)
{
	return (p1 + p2) * 0.5;
}

extern "C" int __declspec(dllexport) __stdcall Init(int& capturePort)
{
	try {
		_capture.open(capturePort);
	}
	catch (const std::exception e) {
		return 1;
	}

	if (!_capture.isOpened())
		return 2;

	return 0;
}

extern "C" void __declspec(dllexport) __stdcall  Close()
{
	_capture.release();
}

extern "C" void __declspec(dllexport) __stdcall Detect()
{
	_capture.grab();

	cv::Mat image, imageCopy;
	_capture.retrieve(image);
	image.copyTo(imageCopy);

	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f>> corners;
	cv::aruco::detectMarkers(image, _dictionary, corners, ids);

	// if at least one marker is detected
	if (ids.size() > 0)
	{
		// See if markers 0 and 1 are being seen
		auto itr0 = std::find(ids.begin(), ids.end(), 0);
		auto itr1 = std::find(ids.begin(), ids.end(), 1);
		if (itr0 != ids.end() && itr1 != ids.end())
		{
			int indexOfMarker0 = itr0 - ids.begin();
			int indexOfMarker1 = itr1 - ids.begin();

			std::vector<cv::Point2f> marker0 = corners[indexOfMarker0];
			std::vector<cv::Point2f> marker1 = corners[indexOfMarker1];

			cv::Point2f topLineMidpoint = findMidpoint(marker0[1], marker1[1]);
			cv::Point2f middleLineMidpoint = findMidpoint(marker0[0], marker1[2]);
			cv::Point2f bottomLineMidpoint = findMidpoint(marker0[3], marker1[3]);

			cv::line(imageCopy, marker0[0], marker1[2], cv::Scalar(0, 0, 255), 1);
			cv::line(imageCopy, marker0[1], marker1[1], cv::Scalar(0, 0, 255), 1);
			cv::line(imageCopy, marker0[3], marker1[3], cv::Scalar(0, 0, 255), 1);

			cv::circle(imageCopy, topLineMidpoint, 3, cv::Scalar(0, 255, 0), 0);
			cv::circle(imageCopy, middleLineMidpoint, 3, cv::Scalar(0, 255, 0));
			cv::circle(imageCopy, bottomLineMidpoint, 3, cv::Scalar(0, 255, 0), 0);

			// Check to see if we see marker 16
			auto itr16 = std::find(ids.begin(), ids.end(), 16);
			if (itr16 != ids.end())
			{
				int indexOfMarker16 = itr16 - ids.begin();
				std::vector<cv::Point2f> marker16 = corners[indexOfMarker16];

				cv::Point2f center = findArucoCenter(marker16);
				cv::circle(imageCopy, center, 3, cv::Scalar(255, 0, 0));

				double xOffset = (double)center.x - (double)middleLineMidpoint.x;
				double yOffset = (double)center.y - (double)middleLineMidpoint.y;
			}
		}
		cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
	}

	cv::imshow("out", imageCopy);
}

extern "C" double __declspec(dllexport) __stdcall xDistance()
{
	_capture.grab();

	cv::Mat image, imageCopy;
	_capture.retrieve(image);
	image.copyTo(imageCopy);

	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f>> corners;
	cv::aruco::detectMarkers(image, _dictionary, corners, ids);

	std::vector<std::vector<cv::Point2f>> sortedCorners{
		std::vector<cv::Point2f>(4),
		std::vector<cv::Point2f>(4),
		std::vector<cv::Point2f>(4),
		std::vector<cv::Point2f>(4)
	};

	bool sees0 = false, sees1 = false, sees2 = false, sees3 = false;

	for (int i = 0; i < corners.size(); i++) {
		if (ids[i] == 0) {
			sortedCorners[0] = corners[i];
			sees0 = true;
		}

		if (ids[i] == 1) {
			sortedCorners[1] = corners[i];
			sees1 = true;
		}

		if (ids[i] == 2) {
			sortedCorners[2] = corners[i];
			sees2 = true;
		}

		if (ids[i] == 3) {
			sortedCorners[3] = corners[i];
			sees3 = true;
		}
	}

	if (sees0 && sees3)
	{
		return (double)sortedCorners[3][1].x - (double)sortedCorners[0][1].x;
	}

	return 0;
}

extern "C" double __declspec(dllexport) __stdcall yDistance()
{
	_capture.grab();

	cv::Mat image, imageCopy;
	_capture.retrieve(image);
	image.copyTo(imageCopy);

	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f>> corners;
	cv::aruco::detectMarkers(image, _dictionary, corners, ids);

	std::vector<std::vector<cv::Point2f>> sortedCorners{
		std::vector<cv::Point2f>(4),
		std::vector<cv::Point2f>(4),
		std::vector<cv::Point2f>(4),
		std::vector<cv::Point2f>(4)
	};

	bool sees0 = false, sees1 = false, sees2 = false, sees3 = false;

	for (int i = 0; i < corners.size(); i++) {
		if (ids[i] == 0) {
			sortedCorners[0] = corners[i];
			sees0 = true;
		}

		if (ids[i] == 1) {
			sortedCorners[1] = corners[i];
			sees1 = true;
		}

		if (ids[i] == 2) {
			sortedCorners[2] = corners[i];
			sees2 = true;
		}

		if (ids[i] == 3) {
			sortedCorners[3] = corners[i];
			sees3 = true;
		}
	}

	if (sees1 && sees3)
	{
		return (double)sortedCorners[3][2].y - (double)sortedCorners[1][2].y;
	}

	return 0;
}

/**
 * Find the x offset of the center of ArUco marker 16 from the midpoint of the
 * line between ArUco marker 0 and 1.
 */
extern "C" double __declspec(dllexport) __stdcall xOffset()
{
	_capture.grab();

	cv::Mat image, imageCopy;
	_capture.retrieve(image);
	image.copyTo(imageCopy);

	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f>> corners;
	cv::aruco::detectMarkers(image, _dictionary, corners, ids);

	// If at least one marker is detected
	if (ids.size() > 0)
	{
		// See if markers 0, 1, and 16 are being seen
		auto itr0 =  std::find(ids.begin(), ids.end(), 0);
		auto itr1 =  std::find(ids.begin(), ids.end(), 1);
		auto itr16 = std::find(ids.begin(), ids.end(), 16);
		
		if (itr0 != ids.end() && itr1 != ids.end() && itr16 != ids.end())
		{
			int indexOfMarker0 =  itr0 - ids.begin();
			int indexOfMarker1 =  itr1 - ids.begin();
			int indexOfMarker16 = itr16 - ids.begin();

			std::vector<cv::Point2f> marker0 =  corners[indexOfMarker0];
			std::vector<cv::Point2f> marker1 =  corners[indexOfMarker1];
			std::vector<cv::Point2f> marker16 = corners[indexOfMarker16];

			cv::Point2f midpoint = findMidpoint(marker0[0], marker1[2]);
			cv::Point2f center = findArucoCenter(marker16);

			return (double)center.x - (double)midpoint.x;
		}
	}
	
	return 1;
}

/**
 * Find the y offset of the center of ArUco marker 16 from the midpoint of the
 * line between ArUco marker 0 and 1.
 */
extern "C" double __declspec(dllexport) __stdcall yOffset()
{
	_capture.grab();

	cv::Mat image, imageCopy;
	_capture.retrieve(image);
	image.copyTo(imageCopy);

	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f>> corners;
	cv::aruco::detectMarkers(image, _dictionary, corners, ids);

	// If at least one marker is detected
	if (ids.size() > 0)
	{
		// See if markers 0, 1, and 16 are being seen
		auto itr0 = std::find(ids.begin(), ids.end(), 0);
		auto itr1 = std::find(ids.begin(), ids.end(), 1);
		auto itr16 = std::find(ids.begin(), ids.end(), 16);

		if (itr0 != ids.end() && itr1 != ids.end() && itr16 != ids.end())
		{
			int indexOfMarker0 = itr0 - ids.begin();
			int indexOfMarker1 = itr1 - ids.begin();
			int indexOfMarker16 = itr16 - ids.begin();

			std::vector<cv::Point2f> marker0 = corners[indexOfMarker0];
			std::vector<cv::Point2f> marker1 = corners[indexOfMarker1];
			std::vector<cv::Point2f> marker16 = corners[indexOfMarker16];

			cv::Point2f midpoint = findMidpoint(marker0[0], marker1[2]);
			cv::Point2f center = findArucoCenter(marker16);

			return (double)center.y - (double)midpoint.y;
		}
	}

	return 1;
}

extern "C" double __declspec(dllexport) __stdcall ScaleDifference()
{
	_capture.grab();

	cv::Mat image, imageCopy;
	_capture.retrieve(image);
	image.copyTo(imageCopy);

	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f>> corners;
	cv::aruco::detectMarkers(image, _dictionary, corners, ids);

	// If at least one marker is detected
	if (ids.size() > 0)
	{
		// See if markers 0, 1, and 16 are being seen
		auto itr0 = std::find(ids.begin(), ids.end(), 0);
		auto itr1 = std::find(ids.begin(), ids.end(), 1);
		auto itr16 = std::find(ids.begin(), ids.end(), 16);

		if (itr0 != ids.end() && itr1 != ids.end() && itr16 != ids.end())
		{
			int indexOfMarker0 = itr0 - ids.begin();
			int indexOfMarker1 = itr1 - ids.begin();
			int indexOfMarker16 = itr16 - ids.begin();

			std::vector<cv::Point2f> marker0 = corners[indexOfMarker0];
			std::vector<cv::Point2f> marker1 = corners[indexOfMarker1];
			std::vector<cv::Point2f> marker16 = corners[indexOfMarker16];

			cv::Point2f topMidpoint = findMidpoint(marker0[1], marker1[1]);
			cv::Point2f botMidpoint = findMidpoint(marker0[3], marker1[3]);

			// Distance between the top and bottom midpoints
			double physicalDistance = calculateDist(topMidpoint.x, topMidpoint.y, botMidpoint.x, botMidpoint.y);
			
			// Distance between the top right and bottom left corners of marker 16
			double virtualDistance = calculateDist(marker16[1].x, marker16[1].y, marker16[3].x, marker16[3].y);

			return physicalDistance - virtualDistance;
		}
	}

	return 0;
}

extern "C" double __declspec(dllexport) __stdcall TopCornerOffset()
{
	_capture.grab();

	cv::Mat image, imageCopy;
	_capture.retrieve(image);
	image.copyTo(imageCopy);

	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f>> corners;
	cv::aruco::detectMarkers(image, _dictionary, corners, ids);

	// If at least one marker is detected
	if (ids.size() > 0)
	{
		// See if markers 0, 1, and 16 are being seen
		auto itr0 = std::find(ids.begin(), ids.end(), 0);
		auto itr1 = std::find(ids.begin(), ids.end(), 1);
		auto itr16 = std::find(ids.begin(), ids.end(), 16);

		if (itr0 != ids.end() && itr1 != ids.end() && itr16 != ids.end())
		{
			int indexOfMarker0 = itr0 - ids.begin();
			int indexOfMarker1 = itr1 - ids.begin();
			int indexOfMarker16 = itr16 - ids.begin();

			std::vector<cv::Point2f> marker0 = corners[indexOfMarker0];
			std::vector<cv::Point2f> marker1 = corners[indexOfMarker1];
			std::vector<cv::Point2f> marker16 = corners[indexOfMarker16];

			cv::Point2f midpoint = findMidpoint(marker0[1], marker1[1]);

			return (double)marker16[1].y - (double)midpoint.y;
		}
	}

	return 0;
}

extern "C" double __declspec(dllexport) __stdcall BotCornerOffset()
{
	_capture.grab();

	cv::Mat image, imageCopy;
	_capture.retrieve(image);
	image.copyTo(imageCopy);

	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f>> corners;
	cv::aruco::detectMarkers(image, _dictionary, corners, ids);

	// If at least one marker is detected
	if (ids.size() > 0)
	{
		// See if markers 0, 1, and 16 are being seen
		auto itr0 = std::find(ids.begin(), ids.end(), 0);
		auto itr1 = std::find(ids.begin(), ids.end(), 1);
		auto itr16 = std::find(ids.begin(), ids.end(), 16);

		if (itr0 != ids.end() && itr1 != ids.end() && itr16 != ids.end())
		{
			int indexOfMarker0 = itr0 - ids.begin();
			int indexOfMarker1 = itr1 - ids.begin();
			int indexOfMarker16 = itr16 - ids.begin();

			std::vector<cv::Point2f> marker0 = corners[indexOfMarker0];
			std::vector<cv::Point2f> marker1 = corners[indexOfMarker1];
			std::vector<cv::Point2f> marker16 = corners[indexOfMarker16];

			cv::Point2f midpoint = findMidpoint(marker0[3], marker1[3]);

			return (double)midpoint.y - (double)marker16[3].y;
		}
	}

	return 0;
}

extern "C" int __declspec(dllexport) __stdcall GetSeenId()
{
	_capture.grab();

	cv::Mat image, imageCopy;
	_capture.retrieve(image);
	image.copyTo(imageCopy);

	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f>> corners;
	cv::aruco::detectMarkers(image, _dictionary, corners, ids);

	int min = 29; //shotgun markers go from 4 to 28
	for (const auto id : ids)
	{
		if (id >= 4 && id < min) {
			min = id;
		}
		if (id == 16) {
			return 16;
		}
	}
	if (min == 29) { //didn't find any markers
		return marker;
	}
	marker = min;
	return min;
}
