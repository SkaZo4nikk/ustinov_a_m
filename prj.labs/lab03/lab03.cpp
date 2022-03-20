#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

void NotTrivial(cv::Mat image) {
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
			image.at<uint8_t>(j, i) = std::pow(image.at<uint8_t>(j, i), 2) / 510;

}

void FuncNotTrivial(int* arr) {
	for (int i = 0; i < 256; i++) {
		arr[i] = std::pow(i, 2) / 510;
	}
}

int main() {
	
	cv::Mat imgPng = cv::imread("../../../data/cross_0256x0256.png");

	if (imgPng.empty()) {
		std::cout << "Path not found" << std::endl;
	}
	else {
		std::cout << "Path found" << std::endl;
	}

	cv::imwrite("lab03_rgb.png", imgPng);
	cv::imshow("lab03_rgb.png", imgPng);

	cv::Mat Gray = imgPng.clone();
	cv::cvtColor(imgPng, Gray, cv::COLOR_BGR2GRAY);
	cv::imwrite("lab03_gre.png", Gray);

	cv::Mat greyscale = Gray.clone();
	NotTrivial(greyscale);
	cv::imwrite("lab03_gre_res.png", greyscale);

	cv::Mat channels[3];
	cv::split(imgPng, channels);
	NotTrivial(channels[0]);
	NotTrivial(channels[1]);
	NotTrivial(channels[2]);
	cv::Mat mergeImage;
	cv::merge(channels, 3, mergeImage);
	cv::imwrite("lab03_rgb_res.png", mergeImage);

	cv::Mat graph(512, 512, CV_8SC3, cv::Scalar(255, 255, 255));
	int LUT[256];
	FuncNotTrivial(LUT);
	for (int i = 0; i < 255; i++) {
		cv::line(graph, cv::Point(2 * i, graph.rows - 2 * LUT[i]), cv::Point(2 * (i + 1), graph.rows - 2 * LUT[i + 1]), cv::Scalar(0, 0, 0));
	}
	cv::imwrite("lab03_viz_func.png", graph);

	cv::waitKey(0);
	}