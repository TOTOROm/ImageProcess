// engraved.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<opencv2/opencv.hpp>

void gray_range_rect(cv::Mat &src, cv::Mat &dst, int width, int height)
{
	int half_w = width / 2;
	int half_h = height / 2;
	dst = cv::Mat::zeros(src.size(), CV_8UC1);
	std::cout << "hello" << std::endl;
	for (int i = 0; i < src.cols - width; i++)
	{
		for (int j = 0; j < src.rows - height; j++)
		{
			cv::Rect rect = cv::Rect(i, j, width, height);
			cv::Mat roi = src(rect);
			cv::Point minLoc, maxLoc;
			double minVal, maxVal;
			cv::minMaxLoc(roi, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
			uchar diff = maxVal - minVal;
			dst.at<uchar>(j + half_h, i + half_w) = diff;
		}
	}

}


int main()
{
	cv::Mat src = cv::imread("2.bmp", 0);
	cv::Mat dst;
	gray_range_rect(src, dst, 10, 10);
	//imshow("res", dst);
	cv::imwrite("res.bmp", dst);
	std::system("pause");
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
