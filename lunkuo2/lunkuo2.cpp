/**************************
文件名:lunkuo2.cpp
功  能：所包含的头文件
************************/
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include"iostream"
#include"cstdio"

#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

using namespace std;
using namespace cv;
class point
{
public:
	int m, x, y;//坐标点
};
Mat src;
/***************************************************
函数功能：显示二值化图像，轮廓坐标输出，画轮廓
注意事项：载入原始图，且必须以二值图模式载入
***************************************************/
void drawline()
{
	
	
	Mat M = src;// =imread("C:\\Users\\Administrator\\Desktop\\picture\\pic.bmp", 0);
	imshow("二值图", M);
	waitKey(500); //等待5000ms后窗口自动关闭
	Mat dstImage = Mat::zeros(M.rows, M.cols, CV_8UC3);//初始化结果图
	//M提取阈值小于250的部分
	M = M<250;
	imshow("阈值", M);
	waitKey(50);
	//定义轮廓和层次结构
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(M, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);
	//迭代器输出
	/*for (vector<vector<Point>>::iterator it=contours.begin();it!=contours.end();++it)
	{
	for (vector<Point>::iterator inner_it=it->begin();inner_it!=it->end();++inner_it)
	{
	cout<<*inner_it<<endl;
	}
	}
	*/
	/******************下标输出,将生成的n个轮廓分别打印到n个文件夹中****************/
	char str[100] = "D:/code/坐标轮廓线%d.txt";
	vector<Moments> mu(contours.size());
	vector<Point2f> mc(contours.size());
	for (int i = 0; i<contours.size(); i++)
	{
		for (int j = 0; j<contours[i].size(); j++)
		{
			cout << contours[i][j].x << "	" << contours[i][j].y << endl;
			ofstream f;
			sprintf_s(str, "D:/code/坐标轮廓线%d.txt", i);
			f.open(str, ios::out | ios::app);
			f << contours[i][j].x << "	" << contours[i][j].y << endl;
		}
		/*******************************************************
		说明：area=轮廓面积 arcLength=轮廓周长
		contour:是一个向量，二维点，可以是vector或Mat类型
		oriented:有默认值false，面向区域标识符，如果为true，
		该函数返回一个带符号的面积，其正负取决于轮廓的方向(顺时针还是逆时针)。
		根据这个特性可以根据面积的符号来确定轮廓的位置。如果是默认值false，
		则面积以绝对值的形式返回.
		arcLength() 
		用于计算封闭轮廓的周长或曲线的长度
		******************************************************/
		mu[i] = moments(contours[i], false);
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		cout << "area"<<i<<"=" << contourArea(contours[i], false) << "  arcLength" <<i<<"="<< arcLength(contours[i], true) << endl<< endl;
		cout <<"轮廓"<<i<<"中心坐标为["<< mc[i].x << ", " << mc[i].y<<"]" << endl<< endl;
		sprintf_s(str, "D:/code/轮廓%d中心坐标.txt", i);
		ofstream pf;
		pf.open(str, ios::out | ios::app);
		pf << mc[i].x << " " << mc[i].y << endl;
	}

	//遍历顶层轮廓，以随机颜色绘制出每个连接组件颜色
	int index = 0;
	for (; index >= 0; index = hierarchy[index][0])
	{
		Scalar color(rand() % 255, rand() % 255, rand() % 255);
		drawContours(dstImage, contours, index, color, 1, 8, hierarchy);
		//circle(drawing, mc[i], 4, color, -1, 8, 0);
	}
	imshow("轮廓图", dstImage);
	//waitKey(5000); //等待5000ms后窗口自动关闭
	//getchar();
}

/******************************************************
函数名称：On_mouse()
函数功能：点击鼠标返回相应坐标位置并存到txtpoints2文件中
操作方法：每次点击左键，将将当前点坐标存储到
		  txt文件中，并在相应位置画黑点
注意事项：此函数可以用来检测所取轮廓点是否准确
*********************************************************
*/
#define WINDOW "source image"
Mat drc;
void On_mouse(int event, int x, int y, int flags, void*);
Point recent_Point;
ofstream file("D:/code/point2.txt", ios::out | ios::binary);
void On_mouse(int event, int x, int y, int flags, void*)
{

	if (event == EVENT_LBUTTONDOWN)
	{    
		recent_Point = Point(x, y);
		
		file<< endl<< recent_Point.x << " " << recent_Point.y<<" "<<endl;
		circle(drc, recent_Point, 3, Scalar(0, 0, 255), -1);
		imshow(WINDOW, drc);

	}
}



/////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	IplImage * img = NULL;
	IplImage * pic = NULL;
	IplImage * pic2 = NULL;
	IplImage * pic3 = NULL;
	IplImage * pic4 = NULL;


	cvNamedWindow("原图", 1);
	cvNamedWindow("灰度图", 1);
	//cvNamedWindow("多边形近似", 1);
	img = cvLoadImage("C:\\Users\\Administrator\\Desktop\\picture\\2222.bmp");
	/***************************************************************************
	描述及语法
	函数cvCreateImage创建图像首地址，并分配存储空间。
	IplImage* cvCreateImage(CvSize cvSize(int width, int height), int depth, int channels);
	参数说明
	cvsize
	size中的图像的宽度和高度。（宽度和高度的顺序需要注意）
	depth
	图像像素的位深度，值为可以为下面一种：
	IPL_DEPTH_8U - 8位无符号整数
	***********************************************************/
	pic = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	pic2 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	pic3 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

	/*********************************************************************
	函数形式： void cvCvtColor( const CvArr* src, CvArr* dst, int code );
	功能：函数cvCvtColor实现色彩空间转换。
	参数列表：
	src：源图像（输入的 8-bit , 16-bit 或 32-bit 单倍精度浮点数影像）
	dst：目标图像（输入的 8-bit , 16-bit 或 32-bit 单倍精度浮点数影像）
	code：RGB <---> Gray：CV_RGB2GRAY、CV_GRAY2RGB、CV_RGBA2GRAY、CV_GRAY2RGBA
	**********************************************************************/
	cvCvtColor(img, pic, CV_BGR2GRAY);//处理为灰度图像

	int height = pic->height;//读取数据基本信息
	int width = pic->width;
	int step = pic->widthStep / sizeof(uchar);
	uchar *data = (uchar *)pic->imageData;
	uchar *data2 = (uchar *)pic2->imageData;
	uchar *data3 = (uchar *)pic3->imageData;
	int i, j = 0;
	for (i = 0; i<height*width; i++)
	{
		data2[i] = 255;
	}
	//大津二值化
	int grey[256];
	//灰度统计
	for (i = 0; i<256; i++)
	{
		grey[i] = 0;
	}

	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			grey[data[i*width + j]]++;
		}
	}

	//总像素数
	int Num = 0;
	for (i = 0; i<256; i++)
	{
		Num = Num + grey[i];
	}

	//各灰度值所占比例
	double p[256];
	for (i = 0; i<256; i++)
	{
		p[i] = double(grey[i]) / double(Num);
	}

	//图像均值
	double u = 0.0;
	for (i = 0; i<256; i++)
	{
		u = u + double(i)*p[i];
	}

	//大津阈值法求阈值T
	/*
	大津法（OTSU）是一种确定图像二值化分割阈值的算法，由日本学者大津于1979年提出。
	从大津法的原理上来讲，该方法又称作最大类间方差法，因为按照大津法求得的阈值进行
	图像二值化分割后，前景与背景图像的类间方差最大（何为类间方差？原理中有介绍）。
	OTSU算法
	OTSU算法也称最大类间差法，有时也称之为大津算法，由大津于1979年提出，被认为是图像
	分割中阈值选取的最佳算法，计算简单，不受图像亮度和对比度的影响，因此在数字图像处理
	上得到了广泛的应用。它是按图像的灰度特性,将图像分成背景和前景两部分。因方差是灰度分布
	均匀性的一种度量,背景和前景之间的类间方差越大,说明构成图像的两部分的差别越大,当部分前景
	错分为背景或部分背景错分为前景都会导致两部分差别变小。因此,使类间方差最大的分割意味着错分
	概率最小
	*/
	int T = 0;
	double max = 0;
	for (i = 1; i<256; i++)
	{
		double wt = 0.0;
		for (j = 0; j<i; j++)
		{
			wt = wt + p[j];
		}

		double ut = 0.0;
		for (j = 0; j<i; j++)
		{
			ut = ut + double(j)*p[j];
		}

		double var = (u*wt - ut)*(u*wt - ut) / (wt*(1 - wt));
		if (var>max)
		{
			max = var;
			T = i;
		}
		//	T = 100;
	}

	//二值化处理
	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			if (data[i*width + j] >= T)
				data2[i*width + j] = 255;
			else
				data2[i*width + j] = 0;
		}
	}
	/***************************************************************
	开运算闭运算
	Erode腐蚀，Dilate膨胀，这两个形态学函数总是成对出现，前者可以消除较小独点如噪音，后者可以
	使不连通的图像合并成块。
	void cvErode( const CvArr* src, CvArr* dst, IplConvKernel* element=NULL, int iterations=1 )
	void cvDilate( const CvArr* src, CvArr* dst, IplConvKernel* element=NULL, int iterations=1 );
	这两个函数的参数相同，是一对“相反”的函数(这里的相反并不是指后一个函数可以将前一个函数处理的图像恢复，
	而是一个缩小图像团块，一个放大图像的团块)
	src输入图像.dst输出图像.element用于膨胀的结构元素。
	若为 NULL, 则使用 3×3 长方形的结构元素
	iterations 膨胀的次数
	膨胀可以重复进行 (iterations) 次. 对彩色图像，每个彩色通道单独处理。 
	***************************************************************/

	IplConvKernel *element = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT);
	cvErode(pic2, pic2, element, 1);
	cvDilate(pic2, pic2, element, 1);
	cvDilate(pic2, pic2, element, 1);
	cvErode(pic2, pic2, element, 1);
	//画轮廓

	//for (i = 0; i<height - 1; i++)
	//{
	//	for (j = 0; j<width - 1; j++)
	//	{
	//		/*if(data2[i*width+j]+data2[i*width+j+1]+data2[(i+1)*width+j]+data2[(i+1)*width+j+1]!=0&&
	//		data2[i*width+j]*data2[i*width+j+1]*data2[(i+1)*width+j]*data2[(i+1)*width+j+1]==0)
	//		{
	//		data[i*width+j]=255;
	//		data3[i*width+j]=255;
	//		}
	//		else data3[i*width+j]=0;*/
	//		data3[i*width + j] = data2[i*width + j] - data2[(i + 1)*width + j];
	//	}
	//}


	//for (i = 0; i<width - 1; i++)
	//{
	//	for (j = 0; j< height- 1; j++)
	//	{
	//		/*if(data2[i*width+j]+data2[i*width+j+1]+data2[(i+1)*width+j]+data2[(i+1)*width+j+1]!=0&&
	//		data2[i*width+j]*data2[i*width+j+1]*data2[(i+1)*width+j]*data2[(i+1)*width+j+1]==0)
	//		{
	//		data[i*width+j]=255;
	//		data3[i*width+j]=255;
	//		}
	//		else data3[i*width+j]=0;*/
	//		data4[i*width + j] = data2[i*width + j] - data2[(i + 1)*width + j];
	//	}
	//}

	//多边形近似
	point a[10];
	for(j=0;j<width;j++)
	{
	for(i=0;i<height;i++)
	{
	if(data3[i*width+j]==255)
	{
	a[0].m=0;
	a[0].x=j;
	a[0].y=i;
	}
	}
	}
	

	//显示阈值
	cout << "阈值为 " << T;
	cout << "\n高值为 " << height;
	cout << "\n宽值为 " << width;
	cout << "\n轮廓线坐标如下： " << endl;
	cvShowImage("原图", img);
	cvShowImage("灰度图", pic);

	//Mat src;//将IplImage转为Mat格式
	src = cvarrToMat(pic2);
	//imshow("src", src);
	//waitKey(0);
	//cvReleaseImage(&pic2);

	drawline();
	//cvShowImage("多边形近似", pic3);
	//if (cvWaitKey(-1) >= 0)
	//{
	//	cvReleaseImage(&pic2);
	//	cvDestroyWindow("Image");
	//	cvReleaseImage(&pic2);
	//	cvDestroyWindow("FFT");
	//}
	drc = src;
	//	imread("abm.jpg"); 
	namedWindow(WINDOW, WINDOW_NORMAL);
	setMouseCallback(WINDOW, On_mouse);
	//while (1)
	imshow(WINDOW, drc);//点击的开始
	waitKey(200000);
	file.close();
	return 0;
}
