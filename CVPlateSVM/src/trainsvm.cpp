#include "trainsvm.h"
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
using namespace std;
using namespace cv;
using namespace cv::ml;

TrainSvm::TrainSvm(const string plates_folder,const string svmxml,int width ,int height):plates_folder_(plates_folder),svmxml_(svmxml)
{
    possuffix = "pos";
    negsuffix = "neg";
    platewidth = width;
    plateheight = height;
}
void TrainSvm::getFiles(const string folder,vector<string>&files)
{
    struct dirent *ptr;
    DIR *dir;
    dir=opendir(folder.c_str());
    while((ptr=readdir(dir))!=NULL)
    {
        if(ptr->d_name[0] == '.')
            continue;
        files.push_back(ptr->d_name);
        //files.push_back(folder+ptr->d_name);
    }
}
void TrainSvm::train()
{

    const string train_pos_plates_folder = plates_folder_+"/train/"+possuffix;
    const string train_net_plates_folder = plates_folder_+"/train/"+negsuffix;

    assert(train_net_plates_folder.data());
    assert(train_pos_plates_folder.data());
    assert(svmxml_.data());

    vector<string> train_hasplates;
    vector<string> train_noplates;
    getFiles(train_pos_plates_folder,train_hasplates);
    getFiles(train_net_plates_folder,train_noplates);
    int train_hasplatesnum = train_hasplates.size();
    int train_noplatesnum = train_noplates.size();
    int traningImagesnum = train_hasplatesnum + train_noplatesnum;
    Mat trainingImages;
    vector<int>traingingLabels;

    for(auto imgsuffix:train_hasplates)
    {
        Mat img = imread(train_pos_plates_folder+"/"+imgsuffix,0);
        resize(img,img,Size(platewidth,plateheight),(0, 0), (0, 0), INTER_LINEAR);
        img = img.reshape(1,1);
        trainingImages.push_back(img);
        traingingLabels.push_back(1);
    }
    for(auto imgsuffix:train_noplates)
    {
        Mat img = imread(train_net_plates_folder+"/"+imgsuffix,0);
        resize(img,img,Size(136,36),(0, 0), (0, 0), INTER_LINEAR);
        img = img.reshape(1,1);
        trainingImages.push_back(img);
        traingingLabels.push_back(0);
    }
    Mat SVM_Classes;
    Mat SVM_TraingingData;
    Mat(trainingImages).copyTo(SVM_TraingingData);
    SVM_TraingingData.convertTo(SVM_TraingingData,CV_32FC1);
    Mat(traingingLabels).copyTo(SVM_Classes);

    Ptr<TrainData>tdata = TrainData::create(SVM_TraingingData,ROW_SAMPLE,SVM_Classes);
    svm_ = SVM::create();
    svm_->setType(SVM::C_SVC);
    svm_->setKernel(SVM::RBF);
    svm_->setDegree(0.1);
    svm_->setGamma(0.1);
    svm_->setCoef0(0.1);
    svm_->setC(1);
    svm_->setP(0.1);
    svm_->setNu(0.1);
    svm_->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 20000, 0.0001));
    svm_->train(tdata);
    svm_->save("plates.xml");
}
void TrainSvm::test()
{
    const string test_pos_plates_folder = plates_folder_+"/test/"+possuffix;
    const string test_net_plates_folder = plates_folder_+"/test/"+negsuffix;
    assert(test_net_plates_folder.data());
    assert(test_pos_plates_folder.data());

    getFiles(test_pos_plates_folder,train_hasplates);
    getFiles(test_net_plates_folder,train_noplates);
    int train_hasplatesnum = train_hasplates.size();
    int train_noplatesnum = train_noplates.size();
    int traningImagesnum = train_hasplatesnum + train_noplatesnum;
    Mat trainingImages;
    vector<int>traingingLabels;

    for(auto imgsuffix:train_hasplates)
    {
        Mat img = imread(train_pos_plates_folder+"/"+imgsuffix,0);
        resize(img,img,Size(platewidth,plateheight),(0, 0), (0, 0), INTER_LINEAR);
        img = img.reshape(1,1);
        trainingImages.push_back(img);
        traingingLabels.push_back(1);
    }
}
int main()
{
    const string folder  = "/home/mxq/Codes/CVPlateSVM/resource/data";
    const string xml = "savexml.xml";
    TrainSvm svm(folder,xml,136,36);
    svm.train();
    return 0;
}