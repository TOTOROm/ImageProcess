// gammatransform.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include<iostream>
#include<highgui\highgui.hpp>
#include<core/core.hpp>  
#include<math.h>
using namespace cv;
using namespace std;
// get Gamma transformation look up table
void GetGammaTransLUT(uchar *pLUT, float Gamma, int iLUTLen)
{
	for (int i = 0; i < iLUTLen; i++)
	{
		pLUT[i] = (uchar)(pow((float)i / 255.0, Gamma) * 255);
	}
}
void GammaTrans(uchar *pSrc, uchar *pDst, const int iHeight,
	const int iWidth, float Gamma)
{
	uchar *pLUT = new uchar[256];
	GetGammaTransLUT(pLUT, Gamma, 256);
	for (int i = 0; i < iHeight*iWidth; i++)
	{
		pDst[i] = (uchar)pLUT[pSrc[i]];
	}
	delete[]pLUT;
}
int main()
{
	Mat image = imread("2.bmp", 0);
	Mat image_Dst = imread("2.bmp", 0);
	const int iHeight = image.rows;
	const int iWidth = image.cols;
	uchar* pSrc = image.data;//new uchar[iHeight*iWidth];
	uchar* pDst = image_Dst.data;//new uchar[iHeight*iWidth];
	GammaTrans(pSrc, pDst, iHeight, iWidth, 2);
	//namedWindow("Origin",1);
	imshow("Origin", image);
	//创建一个名字为“Lena”的图像显示窗口，（不提前声明也可以）  
	//namedWindow("Gamma Trans",1);  
	//显示图像  
	imshow("Gamma Trans", image_Dst);
	//等待按键  
	waitKey();
	return 0;
}

