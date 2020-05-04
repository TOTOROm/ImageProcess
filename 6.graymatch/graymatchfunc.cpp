#include "graymatchfunc.h"

GrayMatch::GrayMatch()
{

}

void GrayMatch::rotateImage(cv::Mat srcImage, cv::Mat &dstImage, float Angle)
{
	cv::Point2f pt(srcImage.cols / 2., srcImage.rows / 2.);
	cv::Mat r = getRotationMatrix2D(pt, Angle, 1.0);

	warpAffine(srcImage, dstImage, r, srcImage.size());
}

double GrayMatch::match(cv::Mat searchImg, cv::Mat modelImg, cv::Point &location)
{
	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	cv::Mat map;
	cv::Mat mask = modelImg * 255;

	map.create(searchImg.cols - modelImg.cols + 1, searchImg.rows - modelImg.rows + 1, CV_32FC1);

	cv::matchTemplate(searchImg, modelImg, map, CV_TM_CCORR_NORMED, mask);
	cv::minMaxLoc(map, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

	location = maxLoc;
	return maxVal;

}

double GrayMatch::matchGrayModel(cv::Mat searchMat)
{
	PyramidTemplates *pyramidTemplates;

	std::vector<cv::Mat> pyDownImgArr;
	pyDownImgArr.clear();

	cv::Mat tempDownImg;
	searchMat.copyTo(tempDownImg);
	pyDownImgArr.push_back(tempDownImg);

	double max_score = 0;
	double angle;
	cv::Point pos;

	for (int i = 1; i <= m_pyramidnums; i++)
	{
		cv::Size size;
		if (tempDownImg.rows % 2 == 0)
			size.height = tempDownImg.rows >> 1;
		else
			size.height = floor(tempDownImg.rows >> 1) + 1;
		if (tempDownImg.cols % 2 == 0)
			size.width = tempDownImg.cols >> 1;
		else
			size.width = floor(tempDownImg.cols >> 1) + 1;

		cv::Mat pyDownImg;
		cv::pyrDown(tempDownImg, pyDownImg, size);
		pyDownImg.copyTo(tempDownImg);
		pyDownImgArr.push_back(pyDownImg);

	}

	for (int i = 0; i < m_anglenum; i++)
	{
		pyramidTemplates = angleTemplates[i].pyramidTemplates;

		cv::Rect searchRect = cv::Rect(0, 0, pyDownImgArr[m_pyramidnums].cols, pyDownImgArr[m_pyramidnums].rows);

		for (int k = m_pyramidnums; k >= 0; k--)
		{
			cv::Point location;

			cv::Mat searchImg = pyDownImgArr[k](searchRect);
			cv::Mat modelImg = cv::cvarrToMat(pyramidTemplates[k].pyramidTemplate);

			double score = match(searchImg, modelImg, location);

			location.x += searchRect.x;
			location.y += searchRect.y;

			searchRect.x = location.x * 2 - 6;
			searchRect.y = location.y * 2 - 6;
			searchRect.width = modelImg.cols * 2 + 12;
			searchRect.height = modelImg.rows * 2 + 12;

			if (score > max_score && k == 0)
			{
				max_score = score;
				pos = location;
				angle = m_anglestart + i * m_anglestep;
			}
		}
	}

	std::cout << max_score << " " << angle << std::endl;
	return 0;
}

int GrayMatch::CreateGrayModel(cv::Mat templateMat, int pyramidnums,
	double anglestart, double angleend, double anglestep)
{
	int anglenum = abs(angleend - anglestart) / anglestep + 1;
	angleTemplates = (AngleTemplates *)malloc(anglenum * sizeof(AngleTemplates));

	m_anglenum = anglenum;
	m_anglestart = anglestart;
	m_angleend = angleend;
	m_anglestep = anglestep;
	m_pyramidnums = pyramidnums;

	for (int i = 0; i < anglenum; i++)
	{
		angleTemplates[i].pyramidTemplates = (PyramidTemplates *)malloc((pyramidnums + 1) * sizeof(PyramidTemplates));
		angleTemplates[i].angle = anglestart + i * anglestep;

		PyramidTemplates *pyramidTemplates = angleTemplates[i].pyramidTemplates;

		cv::Mat angleImage;
		rotateImage(templateMat, angleImage, anglestart + i * anglestep);

		cv::Mat tempDownImg(cv::Size(round(angleImage.cols), round(angleImage.rows)), CV_8UC1);
		angleImage.copyTo(tempDownImg);

		pyramidTemplates[0].level = 0;
		pyramidTemplates[0].pyramidTemplate = cvCreateImage(cvSize(tempDownImg.cols, tempDownImg.rows), IPL_DEPTH_8U, 1);
		cvCopy(&IplImage(tempDownImg), pyramidTemplates[0].pyramidTemplate);

		for (int k = 1; k <= pyramidnums; k++)
		{
			pyramidTemplates[k].level = k;
			cv::Size size;
			if (tempDownImg.rows % 2 == 0)
				size.height = tempDownImg.rows >> 1;
			else
				size.height = floor(tempDownImg.rows >> 1) + 1;

			if (tempDownImg.cols % 2 == 0)
				size.width = tempDownImg.cols >> 1;
			else
				size.width = floor(tempDownImg.cols >> 1) + 1;

			cv::Mat pyDownImg;
			cv::pyrDown(tempDownImg, pyDownImg, size);
			pyDownImg.copyTo(tempDownImg);

			pyramidTemplates[k].pyramidTemplate = cvCreateImage(size, IPL_DEPTH_8U, 1);
			cvCopy(&IplImage(pyDownImg), pyramidTemplates[k].pyramidTemplate);
		}
	}
	return 1;
}
