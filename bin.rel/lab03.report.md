## Работа 3. Яркостные преобразования
автор: Полевой Д.В.
дата: 2022-05-16T15:49:15

<!-- url: https://gitlab.com/2021-misis-spring/polevoy_d_v/-/tree/master/prj.labs/lab03 -->

### Задание
1. В качестве тестового использовать изображение data/cross_0256x0256.png
2. Сгенерировать нетривиальную новую функцию преобразования яркости (не стоит использовать слишком простые и слишком простые функции).
3. Сгенерировать визуализацию функцию преобразования яркости в виде изображения размером 512x512, черные точки а белом фоне.
4. Преобразовать пиксели grayscale версии тестового изображения при помощи LUT для сгенерированной функции преобразования.
4. Преобразовать пиксели каждого канала тестового изображения при помощи LUT для сгенерированной функции преобразования.
5. Результы сохранить для вставки в отчет.

### Результаты

![](lab03_rgb.png)
Рис. 1. Исходное тестовое изображение

![](lab03_gre.png)
Рис. 2. Тестовое изображение greyscale

![](lab03_gre_res.png)
Рис. 3. Результат применения функции преобразования яркости для greyscale

![](lab03_rgb_res.png)
Рис. 4. Результат применения функции преобразования яркости для каналов

![](lab03_viz_func.png)
Рис. 5. Визуализация функции яркостного преобразования

### Текст программы

```cpp
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
```
