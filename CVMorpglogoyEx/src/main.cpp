#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;
Mat g_src,g_dst;
int g_nOpenCloseNum = 0;
int g_nErodeDilateNum = 0;
int g_nTopBlackNum = 0;
int g_nMaxNum = 0;
Mat element;
//static void on_nOpenCloseSlide(int,void *);
static void on_nErodeDilateSlide(int,void *);
static void on_nTopBlackSlide(int,void *);

int g_nShapeNum = MORPH_RECT;
int main()
{
    g_src = imread("snow.jpeg");
    namedWindow("腐蚀/膨胀");
    namedWindow("顶帽/黑帽");
    namedWindow("开/闭");

    //createTrackbar("Size-10","开/闭",& g_nOpenCloseNum,g_nMaxNum*2+1,on_nOpenCloseSlide);

    createTrackbar("Size-110","腐蚀/膨胀",&g_nErodeDilateNum,g_nMaxNum*2+1,on_nErodeDilateSlide);
	createTrackbar("Size-120","顶帽/黑帽",&g_nTopBlackNum,g_nMaxNum*2+1,on_nTopBlackSlide);
    Mat element = getStructuringElement(0, Size(3, 3), Point(-1, -1));
	morphologyEx(g_src,g_dst,MORPH_OPEN,element);
	//morphologyEx(g_src,g_dst,MORPH_CLOSE,element);
	imshow("开/闭",g_dst);
    waitKey(0);
    return 0;
}
static void on_nErodeDilateSlide(int,void*)
{
	erode(g_src,g_dst,element);
	imshow("腐蚀/膨胀",g_dst);
}
static void on_nTopBlackSlide(int,void*)
{
	morphologyEx(g_src,g_dst,MORPH_BLACKHAT,element);
	imshow("顶帽/黑帽",g_dst);
}