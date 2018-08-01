//#include <svm.h>
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
    // for(i=0;i<sn;i++)
    // {
    //     fin>>label[i];
    //     cout<<"label is : "<<label[i]<<endl;
    //     for(j=0;j<fn;j++)
    //     {
    //         fin>>data[i*sn+j];
    //         cout <<"data is : "<<data[i*sn+j]<<endl;
    //     }
    // }
    size_t comma = 0;
    size_t comma2 = 0;
    for(i=0;i<sn;i++)
    {
        getline(fin,line);
        comma = line.find(',',0);
        int tmplabel = atof(line.substr(0,comma).c_str());
        label[i] = tmplabel;
        cout <<"Label is "<< label[i]<<endl;
        while (comma < line.size() && j != fn)
        {
            comma2 = line.find(',',comma + 1);
            double tmpdata = atof(line.substr(comma + 1,comma2-comma-1).c_str());
            data[i*fn+j] = tmpdata;
            cout<<"data is :"<<data[i*fn+j]<<endl;
            ++j;
            comma = comma2;
        }
        cout<<endl;
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
    double *d;
    double *l;
    // 第1组数据集与第2组数据集 预处理
    l = new double[sn1 + sn2];
    d = new double[(sn1+sn2)*fn];
    for(i = 0;i<sn1+sn2;i++)
    {
        if(fabs(label[i] -2) < 1e-3) l[i] = -1;//2 为 -1 类
        else l[i] = 1;//1 为 1 类
    }
    for(i=0;j<fn;i++)
    {
        
    }

}