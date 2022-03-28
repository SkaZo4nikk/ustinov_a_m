# Работа 4. Детектирование границ документов на кадрах видео
автор: Устинов А.М.
дата: 2022-03-20T19:14:05

Github: https://github.com/SkaZo4nikk/ustinov_a_m/tree/main/prj.labs/lab04

## Задание
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

## Объекты исследований

| ![](photo/10rubls_Initial_1.png) | ![](photo/50rubls_Initial_1.png) | ![](photo/100rubls_Initial_1.png) | ![](photo/1000rubls_Initial_1.png) | ![](photo/5000rubls_Initial_1.png) |
|-------------------------------|-------------------------------|------------------------------|------------------------------|------------------------------|
| Рис. 1 Оригинальная купюра         | Рис. 2 Оригинальная купюра         | Рис. 3 Оригинальная купюра        | Рис. 4 Оригинальная купюра       | Рис. 5 Оригинальная купюра        |

## Ход работы

### Описание методов

1. Для начала бинаризируем изображение с помощью метода cv::threshold() и параметра cv::THRESH_BINARY 

   - Пороговым значением бинаризации было выбрано thresh = 120
   - Был реализован подсчет белых пикселей на изображении, если значение больше или равно 50%, то бинаризованное изображение инвертируется, с помощью параметра cv::THRESH_BINARY_INV

2. Далее была проведена морфологическая обработка изображения

   -  cv::MORPH_CLOSE RECT (5, 5)
   -  cv::MORPH_OPEN RECT (5, 5)  

3. После этих преобразований происходит подсчет количества компонент связанности, после чего, в случае если их количество больше 30, то выполняется дополнительная морфолоническая обработка

   - cv::MORPH_OPEN RECT (10, 10)
   - cv::MORPH_CLOSE RECT (10, 10)
   - cv::MORPH_ERODE RECT (30, 15)
   - cv::MORPH_OPEN RECT (50, 25)

4. Далее ко всем изображениям применяется метод морфологической обработки:
   - cv::MORPH_DILATE RECT (30, 20)
5. В итоге выделяется основная компонента связаности и к ней уже применяется морфологическая обработка с параметром cv::MORPH_CLOSE RECT (200, 150)


### Пример работы алгоритма

| ![](photo/5000rubls_Initial_1.png) | ![](photo/5000rubls_Binarization_1.png) | ![](photo/5000rubls_Morhology_1.png) | ![](photo/5000rubls_Morphology2_1.png) |
|------------------------------|-----------------------------|-------------------------------|----------------------------|
| Рис. A: Оригинальное изображение | Рис. B: Бинаризованное изображение | Рис. C: Изображение после морфологической обработки | Рис. D: Изображение после выделения основной компоненты связности и дополнительной морфологической обработки |

### Описание критериев качества 

Для того, чтобы оценить качество был разработан алгоритм, который подсчитывает % совпадения пикселей полученной маски и исходной 

### Результаты 

| ![](photo/10rubls_MaskOverOrg_1.png) | ![](photo/50rubls_MaskOverOrg_1.png) | ![](photo/100rubls_MaskOverOrg_1.png) | ![](photo/1000rubls_MaskOverOrg_1.png) | ![](photo/5000rubls_MaskOverOrg_1.png) |
|--------------------------------|--------------------------------|-------------------------------|-------------------------------|-------------------------------|
| Рис. Ⅰ Similarity of 10rubls = 96.795% | Рис. ⅠⅠ Similarity of 50rubls = 97.419% | Рис. ⅠⅠⅠ Similarity of 100rubls = 96.639%  | Рис. Ⅳ Similarity of 1000rubls = 98.846%  | Рис. Ⅴ Similarity of 5000rubls = 99.047% |

- Зелыным цветом указана область, где не хватает пикселей маски
- Красным цветом указана область, где присутствуют лишнии пиксили маски

### Интересные замечения

#### Купюра номиналом 1000 рублей

| ![](photo/1000rubls_Components_2.png) | ![](photo/1000rubls_Morphology2_2.png) | ![](photo/1000rubls_Components_1.png) |![](photo/1000rubls_Morphology2_1.png) |
|------------------------------|-----------------------------|-----------------------------|-----------------------------|
| Рис. a Маска после выделения основной компоненты связности  | Рис. b Маска после выделения основной компоненты связности и дополнительной морфологической обработки | Рис. c Маска после выделения основной компоненты связности | Рис. d Маска после выделения основной компоненты связности и дополнительной морфологической обработки|

Скорее всего повлияло освещение, из-за чего морфологическая обработка сработала некорректно, вследствие этого у купюры пропал уголок маски

#### Купюра номиналом 100 рублей

| ![](photo/100rubls_Components_2.png) | ![](photo/100rubls_Morphology2_2.png) | ![](photo/100rubls_Components_3.png) |![](photo/100rubls_Morphology2_3.png) |
|------------------------------|-----------------------------|-----------------------------|-----------------------------|
| Рис. e Маска после выделения основной компоненты связности  | Рис. f Маска после выделения основной компоненты связности и дополнительной морфологической обработки | Рис. g Маска после выделения основной компоненты связности | Рис. h Маска после выделения основной компоненты связности и дополнительной морфологической обработки|

Из-за неровного освещения фоновой поверхности на одном из кадров возникла ошибка выделения основной компоненты связанности 

## Текст программы

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
