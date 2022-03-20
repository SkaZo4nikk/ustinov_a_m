## Работа 4. Детектирование границ документов на кадрах видео
автор: Устинов А.М.
дата: 2022-03-15T22:44:42

### Задание
0. текст, иллюстрации и подписи отчета придумываем самостоятельно
1. самостоятельно снимаем видео смартфоном
- объект съемки - купюры (рубли разного номинала), расправленные и лежащие на поверхности (проективно искаженны прямоугольник)
- количество роликов - от 5 шт.
- длительность - 5-7 сек
- условия съемки разные
2. извлекаем по 3 кадра из каждого ролика (делим кол-во кадров на 5 и берем каждый с индеком 2/5,3/5,4/5)
3. цветоредуцируем изображения
4. бинаризцем изображения
5. морфологически обрабатываем изображения
6. выделяем основную компоненту связности
7. руками изготавливаем маски (идеальная зона купюры)
8. оцениваем качество выделение зоны и анализируем ошибки

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

		//cv::threshold


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
