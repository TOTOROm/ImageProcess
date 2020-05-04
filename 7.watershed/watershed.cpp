// watershed.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<opencv2/opencv.hpp>

int main()
{
	cv::Mat src = cv::imread("2.bmp",0);
	cv::cvtColor(src, src, CV_GRAY2BGR);
	cv::imshow("原始图像", src);
	cv::Mat gray, binary, open, sure_bg, dist, sure_fg, shiffted, unkown;

	//当目标区域纹理丰富，可以采用均值漂漂移滤波进行平滑

	//该滤波的特点在于可以保留边缘
	pyrMeanShiftFiltering(src, shiffted, 21, 51);
	cv::imshow("平滑图像", shiffted);
	cv::cvtColor(shiffted, gray, CV_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::morphologyEx(binary, open, cv::MORPH_OPEN, element); //开运算，消除噪声

	//得到确定的背景
	cv::dilate(open, sure_bg, element, cv::Point(-1, -1), 3); //膨胀
	cv::imshow("确定背景", sure_bg);

	//距离变换用于分割粘连
	cv::distanceTransform(open, dist, cv::DIST_L2, 3, CV_32F);
	normalize(dist, dist, 0, 1, cv::NORM_MINMAX);
	cv::imshow("距离变换", dist);

	//得到确定的前景(种子)
	cv::threshold(dist, sure_fg, 0.7, 255, cv::THRESH_BINARY);
	cv::imshow("确定前景", sure_fg);

	//得到未知区域
	sure_fg.convertTo(sure_fg, CV_8U);
	cv::subtract(sure_bg, sure_fg, unkown);
	cv::imshow("未知区域", unkown);
	std::vector<std::vector<cv::Point>> contours;
	findContours(sure_fg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	//创建markers
	cv::Mat markers = cv::Mat::zeros(src.size(), CV_32SC1);
	markers.setTo(1); // 背景区域置1
	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(markers, contours, static_cast<int>(i), cv::Scalar::all(static_cast<int>(i) + 2), -1); // 填充轮廓
	}
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			if (unkown.at<uchar>(i, j) == 255)
			{
				markers.at<int>(i, j) = 0;
			}
		}
	}
	cv::imshow("输入makers", markers * 1000);
	watershed(src, markers);
	cv::imshow("输出makers", markers * 1000);
	cv::waitKey(0);
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
