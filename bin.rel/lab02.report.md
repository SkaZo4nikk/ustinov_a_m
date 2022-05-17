## Работа 2. Исследование каналов и JPEG-сжатия
автор: Полевой Д.В.
дата: 2022-05-16T15:49:15

<!-- url: https://gitlab.com/2021-misis-spring/polevoy_d_v/-/tree/master/prj.labs/lab02 -->

### Задание
1. В качестве тестового использовать изображение data/cross_0256x0256.png
2. Сохранить тестовое изображение в формате JPEG с качеством 25%.
3. Используя cv::merge и cv::split сделать "мозаику" с визуализацией каналов для исходного тестового изображения и JPEG-версии тестового изображения
- левый верхний - трехканальное изображение
- левый нижний - монохромная (черно-зеленая) визуализация канала G
- правый верхний - монохромная (черно-красная) визуализация канала R
- правый нижний - монохромная (черно-синяя) визуализация канала B
4. Результы сохранить для вставки в отчет
5. Сделать мозаику из визуализации гистограммы для исходного тестового изображения и JPEG-версии тестового изображения, сохранить для вставки в отчет.

### Результаты

![](cross_0256x0256_025.jpg)
Рис. 1. Тестовое изображение после сохранения в формате JPEG с качеством 25%

![](cross_0256x0256_png_channels.png)
Рис. 2. Визуализация каналов исходного тестового изображения

![](cross_0256x0256_jpg_channels.png)
Рис. 3. Визуализация каналов JPEG-версии тестового изображения

![](cross_0256x0256_hists.png)
Рис. 3. Визуализация гистограм исходного и JPEG-версии тестового изображения

### Текст программы

```cpp
#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat makeMosaic(cv::Mat src) {
    std::vector<cv::Mat> channels;
    cv::split(src, channels);
    cv::Mat salt = cv::Mat::zeros(src.size(), CV_8U);

    cv::Mat B, G, R;
    cv::merge(std::vector<cv::Mat>{channels[0], salt, salt}, B);
    cv::merge(std::vector<cv::Mat>{salt, channels[1], salt}, G);
    cv::merge(std::vector<cv::Mat>{salt, salt, channels[2]}, R);

    cv::hconcat(src, R, src);
    cv::hconcat(G, B, G);
    cv::vconcat(src, G, src);
    return src;
}

cv::Mat makeHist(const cv::Mat& src) {
    cv::Mat gray;
    std::vector<int> brightness(256, 0);
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    for (int i = 0; i < gray.cols; i++) {
        for (int j = 0; j < gray.rows; j++) {
            ++brightness[gray.at<uint8_t>(j, i)];
        }
    }

    int max_point = *std::max_element(brightness.begin(), brightness.end());
    cv::Mat ans(256, 512, CV_8U, cv::Scalar(255));
    for (int i = 0; i < 256; i++) {
        int h = brightness[i] * 250 / max_point;
        cv::Point pt1(i * 2, 255 - h);
        cv::Point pt2(i * 2 + 1, 255);
        cv::rectangle(ans, pt1, pt2, 0);
    }
    return ans;
}

int main() {
    const std::string path = "C:/ustinov_a_m/data/cross_0256x0256.png";
    cv::Mat cross_0256x0256 = cv::imread(path);
    cv::Mat resized_image;
    std::vector<cv::Mat> channels, resized_channels;

    cv::imwrite("cross_0256x0256_025.jpg", cross_0256x0256, { cv::IMWRITE_JPEG_QUALITY, 25 });
    resized_image = cv::imread("cross_0256x0256_025.jpg");

    cv::Mat mosaic = makeMosaic(cross_0256x0256), resized_mosaic = makeMosaic(resized_image);
    cv::Mat hists = makeHist(cross_0256x0256);
    cv::hconcat(hists, makeHist(resized_image), hists);

    cv::imwrite("./source_mosaic.png", mosaic);
    cv::imwrite("./resized_mosaic.png", resized_mosaic);
    cv::imwrite("./resized_source.png", resized_image);
    cv::imwrite("./hists.png", hists);
    cv::waitKey(0);
}
```
