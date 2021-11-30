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
			// See if markers 0, 1, and 3 are being seen
			auto itr0 = std::find(ids.begin(), ids.end(), 0);
			auto itr1 = std::find(ids.begin(), ids.end(), 1);
			auto itr3 = std::find(ids.begin(), ids.end(), 3);

			if (itr0 != ids.end() && itr1 != ids.end() && itr3 != ids.end())
			{
				int indexOfMarker0 = itr0 - ids.begin();
				int indexOfMarker1 = itr1 - ids.begin();
				int indexOfMarker3 = itr3 - ids.begin();

				std::vector<cv::Point2f> marker0 = corners[indexOfMarker0];
				std::vector<cv::Point2f> marker1 = corners[indexOfMarker1];
				std::vector<cv::Point2f> marker3 = corners[indexOfMarker3];

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
				double virtualDistance = calculateDist(marker3[1].x, marker3[1].y, marker3[3].x, marker3[3].y);

				std::cout << marker3 << std::endl;

				double diff = physicalDistance - virtualDistance;
				std::cout << "Phys Dist: " << physicalDistance << "\nVirt Dist: " << virtualDistance << "\nDiff: " << diff << std::endl;
			}

			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);

			cv::imshow("out", imageCopy);
			char key = (char)cv::waitKey(0);
			if (key == 20)
				break;
		}
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


