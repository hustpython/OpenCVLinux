#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;
namespace machinelearning
{
  class DecisionTree 
  {
    public:
      struct TreeNode 
      {
        string attribute; //此节点所对应的属性
        bool LeafNode;//如果是叶子节点，此值反应分类结果
        vector<TreeNode*>children;//孩子节点的地址
        map<string,TreeNode*>AttributeLinkChildren;
        //属性指向孩子节点，也就是对应的树形结构中的树枝 
      };
      struct Attr //每一列的属性
      {
        int　colIndex;
        string Attribute;
        int typeNum;// 属性取值的个数
        vector<string>AttributeValue;
        map<string,unsigned char> typeMap;
      };
  private:
    struct MatInfo 
    {
      int cols;
      int rows;
    };
    struct entropyInfo
    {
      vector<int>labelvalue;
      vector<int>labelvalueNum;
    };
  public:
    Mat trainDataMat;
    vector<vector<string> >predictedDataMat;
    MatInfo trainMatrixInfo;
    TreeNode *root;
    vector<Attr> vectorAttr;
    DecisionTree();
    int ReadTrainDataFile(string fileAddress);
    TreeNode * BuildTree(Mat &data,vector<Attr> &dataAttr,string AlgorithmName);
    //specify the Algorithm Name
    vector <vector<string> > ReadPredictedDataFile(string fileAddress);
    vector <string> predicted(TreeNode *root,vector<vector<string> > &pData);



  };
}

#endif