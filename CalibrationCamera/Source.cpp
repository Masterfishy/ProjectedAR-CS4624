// Import OpenCV modules
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <algorithm>
#include <cmath>

cv::VideoCapture _capture;
cv::Ptr<cv::aruco::Dictionary> _dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_50);

struct DataHolder {
	DataHolder() {}
	double xDiff, yDiff;
};

double calculateDist(double x1, double y1, double x2, double y2);
cv::Point2f findMidpoint(cv::Point2f p1, cv::Point2f p2);

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

		// if at least one marker is detected

		if (ids.size() > 0)
		{

			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
			double xDiff = abs(sortedCorners[0][1].x - sortedCorners[3][1].x);
			double yDiff = abs(sortedCorners[1][2].y - sortedCorners[3][2].y);

			std::cout << "X Diff: " << xDiff << ", Y Diff: " << yDiff << std::endl;

			std::cout << "ID " << 0 << ", top right: " << sortedCorners[0][1] << std::endl;
			std::cout << "ID " << 3 << ", top right: " << sortedCorners[3][1] << std::endl;
			std::cout << "ID " << 1 << ", bot right: " << sortedCorners[1][2] << std::endl;
			std::cout << "ID " << 3 << ", bot right: " << sortedCorners[3][2] << std::endl;

			std::cout << "ID " << 0 << ": " << sortedCorners[0] << std::endl;
			std::cout << "ID " << 1 << ": " << sortedCorners[1] << std::endl;
			std::cout << "ID " << 2 << ": " << sortedCorners[2] << std::endl;
			std::cout << "ID " << 3 << ": " << sortedCorners[3] << std::endl;

			std::cout << "ID " << 0 << ", arc length: " << cv::arcLength(sortedCorners[0], true) << std::endl;
			std::cout << "ID " << 1 << ", arc length: " << cv::arcLength(sortedCorners[1], true) << std::endl;
			std::cout << "ID " << 2 << ", arc length: " << cv::arcLength(sortedCorners[2], true) << std::endl;
			std::cout << "ID " << 3 << ", arc length: " << cv::arcLength(sortedCorners[3], true) << std::endl;

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

	return 1;
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
	}

	return 1;
}

extern "C" void __declspec(dllexport) __stdcall MeasureArucoDistance(DataHolder* outHolder)
{
	_capture.grab();

	cv::Mat image, imageCopy;
	_capture.retrieve(image);
	image.copyTo(imageCopy);

	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f>> corners;
	cv::aruco::detectMarkers(image, _dictionary, corners, ids);

	std::vector<std::vector<cv::Point2f>> sortedCorners{
		std::vector<cv::Point2f>(),
		std::vector<cv::Point2f>(),
		std::vector<cv::Point2f>(),
		std::vector<cv::Point2f>()
	};

	for (int i = 0; i < corners.size(); i++) {
		if (ids[i] == 0) {
			sortedCorners[0] = corners[i];
		}

		if (ids[i] == 1) {
			sortedCorners[1] = corners[i];
		}

		if (ids[i] == 2) {
			sortedCorners[2] = corners[i];
		}

		if (ids[i] == 3) {
			sortedCorners[3] = corners[i];
		}
	}

	// If at least one marker is detected
	if (ids.size() > 0) {
		cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
	}

	bool aruco0and1 = std::find(ids.begin(), ids.end(), 0) != ids.end() && std::find(ids.begin(), ids.end(), 1) != ids.end() && std::find(ids.begin(), ids.end(), 3) != ids.end();
		
	if (ids.size() > 2 && aruco0and1)
	{

		cv::Point2f topRightId0 = sortedCorners[0][1];
		cv::Point2f topRightId3 = sortedCorners[3][1];

		double x = std::abs(topRightId0.x - topRightId3.x);
		cv::line(imageCopy, topRightId0, topRightId3, cv::Scalar(0, 0, 255), 1);

		cv::Point2f bottomRightId1 = sortedCorners[1][2];
		cv::Point2f bottomRightId3 = sortedCorners[3][2];

		double y = std::abs(bottomRightId1.y - bottomRightId3.y);
		cv::line(imageCopy, bottomRightId1, bottomRightId3, cv::Scalar(0, 0, 255), 1);

		outHolder->xDiff = x;
		outHolder->yDiff = y;
	}

	cv::imshow("yo whats up guys, smash that like and subscribe button and turn on notifications for more calibration content! 8)", imageCopy);
}
