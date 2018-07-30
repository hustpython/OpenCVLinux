#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/ml.h>

using namespace cv;
using namespace cv::ml;

int main()
{
    int width = 512;
    int height = 512;
    Mat image = Mat::zeros(height,width,CV_8UC3);

    int labels[8] = {1,1,0,0,1,1,0,0};
    Mat labelsMat(8,1,CV_32SC1,labels);//signed long -2^31 ~ (2^31-1)
    float traingingData[8][2] =    {{10,10},
                                {10,50},
                                {501,255},
                                {500,501},
                                {40,30},
                                {70,60},
                                {300,300},
                                {60,500}};
    Mat trainingDataMat(8,2,CV_32FC1,traingingData);
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    Ptr<TrainData>td = TrainData::create(trainingDataMat,ROW_SAMPLE,labelsMat);
    svm->train(td);

    Vec3b green(0,255,0),blue(255,0,0);
    Mat sampleMat(1,2,CV_32F);
    float response;
    for(int i=0;i<image.rows;++i)
    {
        for(int j =0;j<image.cols;++j)
        {
            sampleMat.at<float>(0,0) = i;
            sampleMat.at<float>(0,1) = j;
            response = svm->predict(sampleMat);
            if(response == 1)
            {
                image.at<Vec3b>(i,j) = green;
            }
            else if(response == 0)
            {
                image.at<Vec3b>(i,j) = blue;
            }
        }
    }
    int thickness = -1;
    int lineType = 8;
    int x,y;
    Scalar s;
    for(int i=0;i<8;i++)
    {
        if(labels[i])
        {
            s = Scalar(255,0,255);
        }
        else 
        {
            s = Scalar(255,255,0);
        }
        x = traingingData[i][0];
        y = traingingData[i][1];
        circle(image,Point(x,y),5,s,thickness,lineType);
    }
    imshow("SVM Simple Example",image);
    waitKey(0);
    return 0;
}