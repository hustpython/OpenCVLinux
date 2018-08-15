# include <iostream>
# include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc,char ** argv)
{
    Mat image = imread(argv[1]);
    if(image.empty())
    {
        cout<<"图片读取错误"<<endl;
        exit(1);
    }
    // 定义hog描述符
    HOGDescriptor hog;
    // 设置SVM分类器
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    vector<Rect>regions;
    hog.detectMultiScale(image,regions,0,Size(8,8),Size(20,20));
    for(size_t i=0;i<regions.size();i++)
    {
        rectangle(image,regions[i],Scalar(0,255,0),1);
    }
    imshow("行人检测",image);
    imwrite("res.png",image);
    waitKey(0);
    return 0;

}
