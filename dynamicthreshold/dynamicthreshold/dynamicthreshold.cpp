// dynamicthreshold.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>


void dyn_threshold(cv::Mat src, cv::Mat mean, cv::Mat &dst, int thresh)
{
	cv::Mat diff_mat = cv::Mat::zeros(src.size(), CV_8UC1);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			int diff = src.at<uchar>(i, j) - mean.at<uchar>(i, j);
			if (diff > thresh)
			{
				diff_mat.at<uchar>(i, j) = 255;
			}
		}
	}
	diff_mat.copyTo(dst);
}

int main()
{
	cv::Mat src = cv::imread("2.bmp", 0);
	cv::Mat large, largeDilate;
	cv::threshold(src, large, 110, 255, CV_THRESH_BINARY_INV);
	cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7.5, 7.5));
	cv::morphologyEx(large, largeDilate, cv::MORPH_ERODE, element);
	cv::Mat dst;
	cv::Mat mean;
	cv::blur(src, mean, cv::Size(31, 31));
	dyn_threshold(src, mean, dst, 3);
	/*dst = dst & largeDilate;
	element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4));
	cv::morphologyEx(dst, dst, cv::MORPH_OPEN, element);*/
	cv::imshow("src", src);
	cv::imshow("dst", dst);
	cv::imshow("mean", mean);
	cv::waitKey(0);
	system("Pause");
	return 0;
}

