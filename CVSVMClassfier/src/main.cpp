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
    //带标签的训练数据
    float traingingData[8][2] = {{10,10},
                                {10,50},
                                {501,255},
                                {500,501},
                                {40,30},
                                {70,60},
                                {300,300},
                                {60,500}};
    Mat trainingDataMat(8,2,CV_32FC1,traingingData);
    //std::cout << "taringDataMat:\n"<<trainingDataMat << std::endl;
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);//超平面可以不完美,SVM::C_SVC 类型，该类型可以用于n-类分类问题 (n >= 2)。
                             //它可以处理非完美分类的问题 (及训练数据不可以完全的线性分割)。
    svm->setKernel(SVM::LINEAR);//线性可分
    svm->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6));//SVM的迭代训练过程的终止条件
    Ptr<TrainData>td = TrainData::create(trainingDataMat,ROW_SAMPLE,labelsMat);
    svm->train(td);
    
    //imshow("Source Example",image);

    Vec3b green(0,255,0),blue(255,0,0);
    Mat sampleMat(1,2,CV_32F);
    float response;
    for(int i=0;i<image.rows;++i)
    {
        for(int j =0;j<image.cols;++j)
        {
            sampleMat.at<float>(0,0) = i;
            sampleMat.at<float>(0,1) = j;
            //sampleMat : [0,0] [0,1] [0,2]...[0,511]...[511,0],[511,1]...[511,511]
            response = svm->predict(sampleMat);
            //std::cout << response << std::endl;
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
    imwrite("doc/images/res.png",image);
    imshow("SVM Simple Example",image);
    waitKey(0);
    return 0;
}