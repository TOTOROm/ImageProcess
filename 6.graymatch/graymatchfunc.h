#pragma once
#ifndef GRAYMATCH_H
#define GRAYMATCH_H
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>

struct PyramidTemplates
{
	int level;
	IplImage *pyramidTemplate;
};

struct AngleTemplates
{
	PyramidTemplates *pyramidTemplates;
	double angle;
};

struct search_region
{
	int StartX;
	int STartY;
	int EndX;
	int EndY;
};

class GrayMatch
{
public:
	GrayMatch();
	AngleTemplates *angleTemplates;
	void rotateImage(cv::Mat srcImage, cv::Mat &dstImage, float Angle);
	int CreateGrayModel(cv::Mat templateMat, int pyramidnums, double anglestart, double angleend, double anglestep);
	double matchGrayModel(cv::Mat searchMat);
	double match(cv::Mat searchImg, cv::Mat modelImg, cv::Point &location);

private:
	int m_anglenum;
	double m_anglestart;
	double m_angleend;
	double m_anglestep;
	int m_pyramidnums;
};

#endif // GRAYMATCH_H
