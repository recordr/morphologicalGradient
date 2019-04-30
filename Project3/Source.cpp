#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

int x_move[] = { 0,0,-1,1 }; //Сдвиг по x для доступа к соседним пикселям
int y_move[] = { -1,1,0,0 }; //Сдвиг по y для доступа к соседним пикселям (curpixel.y - y_mode[curpixel.y.index])
enum Colors {
	WHITE = 255,
	BLACK = 0
};

void myDilate(cv::Mat& result, cv::Mat input) {
	result = input.clone();
	for (int i = 0; i < input.rows; ++i) {
		for (int j = 0; j < input.cols; ++j) {
			int sum = 0;
			for (int z = 0; z < 4; ++z) {
				if (i + x_move[z] >= 0 && i + x_move[z] < input.rows && j + y_move[z] >= 0 && j + y_move[z] < input.cols) {
					sum += input.at<uint8_t>(i + x_move[z], j + y_move[z]);
				}
			}
			//Проверка, что хотя бы один соседний пиксель белый
			sum += input.at<uint8_t>(i, j);
			if (sum > 0) result.at<uint8_t>(i, j) = WHITE;
			else result.at<uint8_t>(i, j) = BLACK;
		}
	}
}

void myErode(cv::Mat& result, cv::Mat input) {
	result = input.clone();
	for (int i = 0; i < input.rows; ++i) {
		for (int j = 0; j < input.cols; ++j) {
			int sum = 0;
			for (int z = 0; z < 4; ++z) {
				if (i + x_move[z] >= 0 && i + x_move[z] < input.rows && j + y_move[z] >= 0 && j + y_move[z] < input.cols) {
					sum += input.at<uint8_t>(i + x_move[z], j + y_move[z]);
				}
			}
			//Проверка, что все пиксели вокруг белые
			sum += input.at<uint8_t>(i, j);
			if (sum == WHITE*5) result.at<uint8_t>(i, j) = WHITE;
			else result.at<uint8_t>(i, j) = BLACK;
		}
	}
}

void myMorphologicalGradient(cv::Mat& result, cv::Mat dilated, cv::Mat eroded) {
	result = dilated - eroded;
}

void myThreshold(cv::Mat& result, cv::Mat input, int threshold) { //Бинаризация
	result = input.clone();
	for (int i = 0; i < input.rows; ++i)
	{
		for (int j = 0; j < input.cols; ++j)
		{
			if (input.at<uint8_t>(i, j) <= threshold)
			{
				result.at<uint8_t>(i, j) = BLACK;
			}
			else
			{
				result.at<uint8_t>(i, j) = WHITE;
			}
		}
	}
}


int main()
{
	cv::Mat image;
	image = cv::imread("C:/Users/Xiaomi/source/repos/Project3/x64/Debug/tentative.png", cv::IMREAD_COLOR);
	cv::imshow("Source", image);

	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::imshow("Gray", gray);
	
	cv::Mat thresh;
	myThreshold(thresh, gray, 127);
	cv::imshow("Threshold", thresh);
	
	cv::Mat result;
	cv::Mat dilated;
	cv::Mat eroded;
	myDilate(dilated, thresh);
	myErode(eroded, thresh);
	myMorphologicalGradient(result, dilated, eroded);
	cv::imshow("Dilated", dilated);
	cv::imshow("Eroded", eroded);
	cv::imshow("Gradient", result);

	cv::waitKey(0);
	return 0;
}