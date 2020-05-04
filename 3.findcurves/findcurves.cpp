// findcurves.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

//
bool findNextPoint(vector<Point> &neighbor_points, Mat &image, Point inpoint, int flag, Point& outpoint, int &outflag)
{
	int i = flag;
	int count = 1;
	bool success = false;

	while (count <= 7)
	{
		Point tmppoint = inpoint + neighbor_points[i];
		if (tmppoint.x > 0 && tmppoint.y > 0 && tmppoint.x < image.cols&&tmppoint.y < image.rows)
		{
			if (image.at<uchar>(tmppoint) == 255)
			{
				outpoint = tmppoint;
				outflag = i;
				success = true;
				image.at<uchar>(tmppoint) = 0;
				break;
			}
		}
		if (count % 2)
		{
			i += count;
			if (i > 7)
			{
				i -= 8;
			}
		}
		else
		{
			i += -count;
			if (i < 0)
			{
				i += 8;
			}
		}
		count++;
	}
	return success;
}

//寻找图像上的第一个点
bool findFirstPoint(Mat &inputimg, Point &outputpoint)
{
	bool success = false;
	for (int i = 0; i < inputimg.rows; i++)
	{
		uchar* data = inputimg.ptr<uchar>(i);
		for (int j = 0; j < inputimg.cols; j++)
		{
			if (data[j] == 255)
			{
				success = true;
				outputpoint.x = j;
				outputpoint.y = i;
				data[j] = 0;
				break;
			}
		}
		if (success)
			break;
	}
	return success;
}

//寻找曲线 
void findLines(Mat &_inputimg, vector<deque<Point>> &_outputlines)
{
	vector<Point> neighbor_points = { Point(-1,-1),Point(0,-1),Point(1,-1),Point(1,0),Point(1,1),Point(0,1),Point(-1,1),Point(-1,0) };
	Point first_point;
	while (findFirstPoint(_inputimg, first_point))
	{
		deque<Point> line;
		line.push_back(first_point);
		//由于第一个点不一定是线段的起始位置，双向找
		Point this_point = first_point;
		int this_flag = 0;
		Point next_point;
		int next_flag;
		while (findNextPoint(neighbor_points, _inputimg, this_point, this_flag, next_point, next_flag))
		{
			line.push_back(next_point);
			this_point = next_point;
			this_flag = next_flag;
		}
		//找另一边
		this_point = first_point;
		this_flag = 0;
		//cout << "flag:" << this_flag << endl;
		while (findNextPoint(neighbor_points, _inputimg, this_point, this_flag, next_point, next_flag))
		{
			line.push_front(next_point);
			this_point = next_point;
			this_flag = next_flag;
		}
		if (line.size() > 10)
		{
			_outputlines.push_back(line);
		}
	}
}
//随机取色 用于画线的时候
Scalar random_color(RNG& _rng)
{
	int icolor = (unsigned)_rng;
	return Scalar(icolor & 0xFF, (icolor >> 8) & 0xFF, (icolor >> 16) & 0xFF);
}
int main()
{
	Mat image = imread("contour.bmp",1);
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	vector<deque<Point>> lines;
	findLines(gray, lines);
	cout << lines.size() << endl;
	//draw lines
	Mat draw_img = image.clone();
	RNG rng(123);
	Scalar color;
	for (int i = 0; i < lines.size(); i++)
	{
		color = random_color(rng);
		for (int j = 0; j < lines[i].size(); j++)
		{
			draw_img.at<Vec3b>(lines[i][j]) = Vec3b(color[0], color[1], color[2]);
		}
	}
	imshow("draw_img", draw_img);
	imwrite("draw_img.bmp", draw_img);
	waitKey(0);
	system("pause");
	return 0;
}
