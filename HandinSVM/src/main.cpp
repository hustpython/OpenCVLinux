#include <svm.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

typedef double tLabel;
typedef double tData;

const int fn = 13;//特征数目
const int sn1 = 59;//类标1的数目
const int sn2 = 71;//类标2的数目
const int sn3 = 48;//类标3的数目
const int sn = 178;//总的样本数目
ifstream fin;

int readData(tData data[],tLabel label[],char *datapath)
{
    //全部数据 data->(sn*fn) * 1;label->sn*1
    fin.open(datapath);
    if(!fin)
    {
        cout << "can not open the passing data"<<endl;
        exit(1);
    }
    int i,j;
    string line;
    size_t comma = 0;
    size_t comma2 = 0;
    for(i=0;i<sn;i++)
    {
        getline(fin,line);
        comma = line.find(',',0);
        int tmplabel = atof(line.substr(0,comma).c_str());
        label[i] = tmplabel;
        //cout <<"Label is "<< label[i]<<endl;
        while (comma < line.size() && j != fn)
        {
            comma2 = line.find(',',comma + 1);
            double tmpdata = atof(line.substr(comma + 1,comma2-comma-1).c_str());
            data[i*fn+j] = tmpdata;
            //cout<<"data is :"<<data[i*fn+j]<<endl;
            ++j;
            comma = comma2;
        }
        j = 0;
    }
    fin.close();
    
}

int main(int argc,char **argv)
{
    if(argc !=2)
    {
        cout << "you should use like this:./a.out ../resource/test.data"<<endl;
        exit(1);
    }
    char *datapath = argv[1];
    tData *data = new tData[fn*sn];

    tLabel *label = new tLabel[sn];
    readData(data,label,datapath);
    int i,j;
    double *d1,*d2,*d3;
    double *l1,*l2,*l3;
    // 第1组数据集与第2组数据集 预处理
    l1 = new double[sn1 + sn2];
    d1 = new double[(sn1+sn2)*fn];
    for(i = 0;i<sn1+sn2;i++)
    {
        if(fabs(label[i] -2) < 1e-3) 
        {
            l1[i] = -1;//2 为 -1 类
        }
        else 
        {
            l1[i] = 1;//1 为 1 类
        }
    }
    for(i= 0;i<sn1+sn2;i++)
    {
        for(j=0;j<fn;j++)
        {
            d1[i*fn+j] = data[i*fn+j];
        }
    }
    // 第2组数据集与第三组数据集 
    l2 = new double[sn2 + sn3];
    d2 = new double[(sn2+sn3)*fn];
    for(i = 0;i<sn2+sn3;i++)
    {
        if(fabs(label[i+sn1] - 3) < 1e-3) 
        {
            l2[i] = -1;//3 为 -1 类
        }
        else 
        {
            l2[i] = 1;//2 为 1 类
        }
    }
    for(i= 0;i<sn2+sn3;i++)
    {
        for(j=0;j<fn;j++)
        {
            d2[i*fn+j] = data[(i+sn1)*fn+j];
        }
    }
    // 第3组数据集与第一组数据集
    l3 = new double[sn1 + sn3];
    d3 = new double[(sn1+sn3)*fn];

    for(i = 0;i<sn1;i++)
    {
       l3[i] = -1;//1 为 -1 类
    }

    for(i = 0;i<sn3;i++)
    {
        l3[i+sn1] = 1;//3 为 1 类
    }
    for(i= 0;i<sn1;i++)
    {
        for(j=0;j<fn;j++)
        {
            d3[i*fn+j] = data[i*fn+j];
        }
    }
    for(i= 0;i<sn3;i++)
    {
        for(j=0;j<fn;j++)
        {
            d3[(i+sn1)*fn+j] = data[(i+sn1+sn2)*fn+j];
        }
    }
    
//////////////////////////////////////////////Test
    SVM svm1(d1, l1, sn1+sn2, fn); 
    // cout << "正在计算12组数据..."<<endl;  
    // svm1.SMO();
    // svm1.saveparams(1);

    SVM svm2(d2,l2,sn2+sn3,fn);
    // cout << "正在计算23组数据..."<<endl;  
    // svm2.SMO();
    // svm2.saveparams(2);

    SVM svm3(d3,l3,sn3+sn1,fn);
    // cout << "正在计算31组数据..."<<endl;  
    // svm3.SMO();
    // svm3.saveparams(3);
// 测试三分类的准确率
    
    double **testdata;
    int *testlabel;
    testdata = new double*[sn];
    testlabel = new int[sn];
    for(int i=0;i<sn;i++)
    {
        testdata[i] = new double[fn];
        for (int j=0;j<fn;j++)
        {
            testdata[i][j] = data[i*fn+j];
        }
        testlabel[i] = int(label[i]);
    }
    //int cumputed01[sn];
    int errorcount = sn;
    for(int i=0;i<sn1;i++)
    {
        svm1.loadparams(1);
        int res1 = svm1.predict(testdata[i]);
        svm2.loadparams(2);
        int res2 = svm2.predict(testdata[i]);
        svm3.loadparams(3);
        int res3 = svm3.predict(testdata[i]);
        if(res1==1 && res3==-1)
        {
            errorcount --;
        } 
    }
    for(int i=sn1;i<sn1+sn2;i++)
    {
        svm1.loadparams(1);
        int res1 = svm1.predict(testdata[i]);
        svm2.loadparams(2);
        int res2 = svm2.predict(testdata[i]);
        svm3.loadparams(3);
        int res3 = svm3.predict(testdata[i]);
        if(res2==1 && res1==-1)
        {
            errorcount --;
        } 
    }
    for(int i=sn1+sn2;i<sn;i++)
    {
        svm1.loadparams(1);
        int res1 = svm1.predict(testdata[i]);
        svm2.loadparams(2);
        int res2 = svm2.predict(testdata[i]);
        svm3.loadparams(3);
        int res3 = svm3.predict(testdata[i]);
        if(res3==1 && res2 == -1)
        {
            errorcount --;
        } 
    }
    cout<<errorcount<<endl;
    //svm.loadparams(1);
    //cout << "数据集1和数据集2"<<endl;  
    //svm.show(); 
    //double testdata1[] = {14.13,4.1,2.74,24.5,96,2.05,.76,.56,1.35,9.2,.61,1.6,560};
    //svm3.loadparams(3);
    //cout<<svm3.predict(testdata1)<<endl;
    delete l1;  
    delete d1; 
    delete l2;  
    delete d2; 
    delete l3;  
    delete d3; 
    delete [] testdata;
    delete testlabel;
    //getchar();

    return 0;
}