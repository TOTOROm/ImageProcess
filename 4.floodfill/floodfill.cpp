// floodfill.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"

#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat src, mask;
int g_nFillMode = 1;//漫水填充的模式
int g_nConnectivity = 4;//表示floodFill函数标识符低八位的连通值
int g_nNewMaskVal = 255;//新的重新绘制的像素值

static void onMouse(int event, int x, int y, int, void*)
{
	if (event != CV_EVENT_LBUTTONDOWN)
		return;
	Point seed = Point(x, y);
	//标识符的0~7位为g_nConnectivity，8~15位为g_nNewMaskVal左移8位的值，16~23位为CV_FLOODFILL_FIXED_RANGE或者0。
	int flags = g_nConnectivity + (g_nNewMaskVal << 8) +
		(g_nFillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);

	Rect rect;
	int area;
	area = floodFill(src, mask, seed, cv::Scalar(255, 255, 255), &rect, Scalar(20, 20, 20), Scalar(20, 20, 20), flags);
	imshow("mask", mask);
	imshow("src", src);
}





int main()
{
	src = imread("2.bmp", 1);
	if (!src.data) 
	{
		return false;
	}
	mask.create(src.rows + 2, src.cols + 2, CV_8UC1);//利用image0的尺寸来初始化掩膜mask
	mask = Scalar::all(0);
	namedWindow("src", CV_WINDOW_AUTOSIZE);
	setMouseCallback("src", onMouse, 0);
	imshow("src", src);
	waitKey(0);
	return 0;
}



