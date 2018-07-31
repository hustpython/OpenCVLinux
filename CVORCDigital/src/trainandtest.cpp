#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml.hpp> 
using namespace cv;
using namespace cv::ml; 
int main(int argc,char ** argv)
{
    Mat thr,gray,con;
    Mat src = imread(argv[1],1);
    cvtColor(src,gray,CV_BGR2GRAY);
    threshold(gray,thr,200,255,THRESH_BINARY_INV);
    thr.copyTo(con);
    Mat sample;
    Mat response,tmp;
    FileStorage Data(argv[2],FileStorage::READ);
    Data["data"] >> sample;
    Data.release();

    FileStorage Label(argv[3],FileStorage::READ);
    Label["label"] >> response;
    Label.release();

    Ptr<KNearest> knn = KNearest::create(); 
    knn->train(sample,ROW_SAMPLE,response); 

    std::cout<<"Training completed ......!!!"<<std::endl;
    std::vector<std::vector <Point> >contours;
    std::vector < Vec4i> hierarchy;
    findContours(con,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
    Mat dst(src.rows,src.cols,CV_8UC3,Scalar::all(0));

    Mat contourdst = dst.clone();
    cv::drawContours(contourdst, contours, -1, cv::Scalar::all(255));
    cv::imshow("Contours", contourdst);
	cv::waitKey(0);
    
    for (int i = 0; i < contours.size(); i = hierarchy[i][0])
    {
        Rect r= boundingRect(contours[i]);
        Mat ROI = thr(r);
        Mat tmp1, tmp2;
        resize(ROI,tmp1, Size(10,10), 0,0,INTER_LINEAR );
        tmp1.convertTo(tmp2,CV_32FC1);
        Mat results;
        knn->findNearest(tmp2.reshape(1,1), 1,results); 
        char name[4];
        sprintf(name,"%d",int((results.at<float>(0,0)))); 
        putText( dst,name,Point(r.x,r.y+r.height) ,0,1, Scalar(0, 255, 0), 2, 8 );
}

imshow("src",src);
imshow("dst",dst);
imwrite("dest.jpg",dst);
waitKey();
    return 0;
}