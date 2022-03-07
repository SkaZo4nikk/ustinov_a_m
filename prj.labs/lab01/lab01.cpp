#include <opencv2/opencv.hpp>
#include <math.h>
#include <chrono>

int main() {
	cv::Mat i_1(60, 768, CV_8UC1);
	// draw dummy image
	i_1 = 0;
	int color = 0;
	for (size_t i = 0; i < 768; i++)
	{
		for (size_t j = 0; j < 60; j++)
		{
			i_1.at<uint8_t>(j, i) = color;
		}
		if (i % 3 == 0) color++;
	}

	cv::Mat g_1;
	i_1.convertTo(g_1, CV_32F, 1.0 / 255.0);
	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		cv::pow(g_1, 2.3, g_1);
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "Elapsed Time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[ms]" << std::endl;
	}

	cv::Mat g_2 = i_1.clone();
	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		color = 0;
		int new_color = color;
		for (size_t i = 0; i < 768; i++)
		{
			for (size_t j = 0; j < 60; j++)
			{
				g_2.at<uint8_t>(j, i) = new_color;
			}
			if (i % 3 == 0) {
				color++;
				new_color = std::pow(color, 2.2) / (255 * std::pow(2.2, 2));
			}
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "Elapsed Time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[ms]" << std::endl;
	}

	cv::Mat ans(180, 768, CV_8UC1);
	g_1.convertTo(g_1, CV_8UC1, 255.0);
	i_1.copyTo(ans(cv::Rect(0, 0, 768, 60)));
	g_1.copyTo(ans(cv::Rect(0, 60, 768, 60)));
	g_2.copyTo(ans(cv::Rect(0, 120, 768, 60)));
	cv::imshow("Result", ans);
	cv::waitKey(0);
	cv::imwrite("lab01.png", ans);
}