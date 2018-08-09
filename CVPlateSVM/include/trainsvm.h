#ifndef TRAINSWVM_H_
#define TRAINSWVM_H_

#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;
using namespace cv::ml;
//先不进行特征提取
//typedef void (*svmCallback)(const Mat& image, Mat& features);

class TrainSvm 
{
public:
  TrainSvm(const string plates_folder,const string svmxml,int width,int height);
  void getFiles(const string plates_folder_ , vector<string> &files);
  void train();
  void test();
private: 
  const string plates_folder_;
  const string svmxml_;
  int platewidth;
  int plateheight;
  string possuffix;
  string negsuffix;
  Ptr<TrainData> tdata();
  Ptr<SVM> svm_;
  
  //svmCallback extractFeature;
};

#endif