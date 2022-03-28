## Работа 5. Детектирование границ документов на кадрах видео
автор: Полевой Д.В.
дата: 2022-03-28T12:18:11

### Задание
0. текст, иллюстрации и подписи отчета придумываем самостоятельно
1. используем данные из лаб. 4
2. выделяем границы и находим внешнюю рамку документа
3. руками изготавливаем векторную разметку (идеальная рамка купюры)
4. оцениваем качество и анализируем ошибки

### Результаты

### Текст программы

```cpp
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

void func(std::string Path) {
	cv::VideoCapture video(Path + ".mp4");
	if (!video.isOpened()) {
		std::cout << "Error opening video stream or file" << std::endl;
	}

	Path = Path.substr(14);
	
	int nframes =  video.get(cv::CAP_PROP_FRAME_COUNT);
	cv::Mat frame[3];
	int FrameNumber;

	for (int i = 0; i < 3; i++) {
		FrameNumber = nframes * (i + 2) / 5;
		video.set(cv::CAP_PROP_POS_FRAMES, FrameNumber);
		video >> frame[i];
		cv::imwrite("photo/" + Path + "_Initial" + "_" + std::to_string(i + 1) + ".png", frame[i]);
		
		cv::Mat GrayFrame = frame[i];
		cv::cvtColor(GrayFrame, GrayFrame, cv::COLOR_BGR2GRAY);
		cv::imwrite("photo/" + Path + "_Gray" + "_" + std::to_string(i + 1) + ".png", GrayFrame);

		cv::Mat Binarization;
		cv::threshold(GrayFrame, Binarization, 120, 255, cv::THRESH_BINARY);

		double PercentWhitePixel = 0.0;
		for (int j = 0; j < Binarization.rows; j++) {
			for (int k = 0; k < Binarization.cols; k++) {
				if (Binarization.at<uint8_t>(j, k) == 255) {
					PercentWhitePixel++;
				}
			}
		}
		
		PercentWhitePixel = PercentWhitePixel / (Binarization.cols * Binarization.rows) * 100;
		//std::cout << Path + " " << PercentWhitePixel << std::endl;

		if (PercentWhitePixel >= 50) {
			cv::threshold(Binarization, Binarization, 70, 255, cv::THRESH_BINARY_INV);
		}
		cv::imwrite("photo/" + Path + "_Binarization" + "_" + std::to_string(i + 1) + ".png", Binarization);

		cv::Mat Morphology;
		cv::morphologyEx(Binarization, Morphology, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
		cv::morphologyEx(Morphology, Morphology, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
		
		cv::Mat LabelImage(Morphology.size(), CV_32S), stats, centroids;
		int nLabels = cv::connectedComponentsWithStats(Morphology, LabelImage, stats, centroids, 8, CV_32S);
		//std::cout << nLabels << std::endl;

		if (nLabels > 30) {
			cv::morphologyEx(Morphology, Morphology, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10)));
			cv::morphologyEx(Morphology, Morphology, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10)));
			cv::morphologyEx(Morphology, Morphology, cv::MORPH_ERODE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 15)));
			cv::morphologyEx(Morphology, Morphology, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(50, 25)));
		}
		cv::morphologyEx(Morphology, Morphology, cv::MORPH_DILATE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 20)));
		cv::imwrite("photo/" + Path + "_Morhology" + "_" + std::to_string(i + 1) + ".png", Morphology);

		nLabels = cv::connectedComponentsWithStats(Morphology, LabelImage, stats, centroids, 8, CV_32S);
		
		/*for (int j = 0; j < nLabels; j++) {
			std::cout << j << " " << stats.at<int>(j, cv::CC_STAT_AREA) << std::endl;
		}
		*/

		int MaxArea = 0;
		int MaxLabel = 1;
		
		//MaxComponents
		for (int j = 1; j < nLabels; j++) {
			if (MaxArea < stats.at<int>(j, cv::CC_STAT_AREA)) {
				MaxArea = stats.at<int>(j, cv::CC_STAT_AREA);
				MaxLabel = j;
			}
		}

		cv::Mat Components(Morphology.size(), CV_8UC1);
		Components = 0;

		for (int j = 0; j < Components.rows; j++) {
			for (int k = 0; k < Components.cols; k++) {
				if (MaxLabel == LabelImage.at<int>(j, k)) {
					Components.at<uint8_t>(j, k) = 255;
				}
			}
			
		}
		cv::imwrite("photo/" + Path + "_Components" + "_" + std::to_string(i + 1) + ".png", Components);

		cv::Mat Morphology2;
		cv::morphologyEx(Components, Morphology2, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(200, 150)));
		cv::imwrite("photo/" + Path + "_Morphology2" + "_" + std::to_string(i + 1) + ".png", Morphology2);

		cv::Mat EtalonMask;
		EtalonMask = cv::imread("../../../data/EtalonMask/" + Path + "_EtalonMask_" + std::to_string(i + 1) + ".png");
		cv::cvtColor(EtalonMask, EtalonMask, cv::COLOR_BGR2GRAY);
		
		double Count = 0;

		for (int j = 0; j < EtalonMask.rows; j++) {
			for (int k = 0; k < EtalonMask.cols; k++) {
				if (EtalonMask.at<uint8_t>(j, k) == Morphology2.at<uint8_t>(j, k)) {
					Count++;
				}
		
			}
		}
		
		//std::cout << EtalonMask.type();
		//std::cout << Morphology2.type();

		std::cout << i + 1 << ") " << "Similarity of " + Path + " " << " = " << Count / (Morphology2.rows * Morphology2.cols) * 100 << "%" << std::endl;

		cv::Mat MaskOverOrg(Morphology2.size(), CV_8UC3);

		for (int j = 0; j < Morphology2.rows; j++) {
			for (int k = 0; k < Morphology2.cols; k++) {
				if ((EtalonMask.at<uint8_t>(j, k) == 0) && (Morphology2.at<uint8_t>(j, k) == 0)) {
					MaskOverOrg.at<cv::Vec3b>(j, k) = cv::Vec3b(0, 0, 0);
				}
				else if ((EtalonMask.at<uint8_t>(j, k) == 0) && (Morphology2.at<uint8_t>(j, k) == 255)) {
					MaskOverOrg.at<cv::Vec3b>(j, k) = cv::Vec3b(0, 0, 255);
				}
				else if ((EtalonMask.at<uint8_t>(j, k) == 255) && (Morphology2.at<uint8_t>(j, k) == 0)) {
					MaskOverOrg.at<cv::Vec3b>(j, k) = cv::Vec3b(0, 255, 0);
				}
				else if ((EtalonMask.at<uint8_t>(j, k) == 255) && (Morphology2.at<uint8_t>(j, k) == 255)) {
					MaskOverOrg.at<cv::Vec3b>(j, k) = cv::Vec3b(255, 255, 255);
				}
			}
		}

		cv::addWeighted(frame[i], 0.5, MaskOverOrg, 0.5, 0.0, MaskOverOrg);
		cv::imwrite("photo/" + Path + "_MaskOverOrg_" + std::to_string(i + 1) + ".png", MaskOverOrg);
	}
}

int main() {
	func("../../../data/10rubls");
	func("../../../data/50rubls");
	func("../../../data/100rubls");
	func("../../../data/1000rubls");
	func("../../../data/5000rubls");
}




```
