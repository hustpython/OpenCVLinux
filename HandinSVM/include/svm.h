// 用支持向量机求解二分类问题
// 分离超平面:w'.x + b = 0
// 分类决策函数:f(x) = sign(w'.x + b)
#include <iostream>
using namespace std;

class SVM 
{
private: 
       int sampleNum;//样本数目
       int featureNum;//特征数
       double **data;//存放样本 行:样本 列:特征
       double *label;//存放类标
       double *alpha;

       double b;
       double *gx;

       double s_max(double,double);
       double s_min(double,double);
       int secondAlpha(int);
       void computeGx();
       double kernel(int,int);
       void update(int,int,double,double);
       bool isConvergence();
       bool takeStep(int,int);

public: 
       ~SVM();
       //初始化数据
       SVM(double *,double *,int,int);
       //序列最小化优化算法 
       void SMO();
       double objFun(int);
       void show();
};
