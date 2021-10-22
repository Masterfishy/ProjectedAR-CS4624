// Import the aruco module in OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

cv::VideoCapture _capture;
cv::Ptr<cv::aruco::Dictionary> _dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_50);

double calculateDist(double x1, double y1, double x2, double y2);
cv::Point2f findMidpoint(cv::Point2f p1, cv::Point2f p2);

int main()
{
	_capture.open(0);

	while (_capture.grab())
	{
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

		// if at least one marker is detected
		if (ids.size() > 1)
		{
			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);

			cv::line(imageCopy, sortedCorners[0][0], sortedCorners[1][2], cv::Scalar(0, 0, 255), 1);
			double dist = calculateDist(sortedCorners[0][0].x, sortedCorners[0][0].y, sortedCorners[1][2].x, sortedCorners[1][2].y);
			std::cout << dist << "px" << std::endl;

			std::string distText = std::to_string(dist) + " px.";
			cv::Point2f midpoint = findMidpoint(sortedCorners[0][0], sortedCorners[1][2]);

			cv::putText(imageCopy, distText, midpoint, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1, true);
		}

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
	_capture.open(capturePort);
	if (!_capture.isOpened())
		return -1;

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
