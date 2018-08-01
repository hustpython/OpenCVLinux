#include <iostream>
#include <map>
#include <vector>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <fstream>

using namespace std;
typedef double tData;
typedef string tLabel;
typedef pair<int,double> PAIR;
const int MaxColLen = 10;
const int MaxRowLen = 10000;
ifstream fin;
ofstream fout;

class KNN 
{
private:
       tData dataSet[MaxRowLen][MaxColLen];
       tLabel labels[MaxRowLen];
       tData testData[MaxColLen];
       int rowLen;
       int colLen;
       int k;
       int test_data_num;
       map<int,double>map_index_dis;
       map<tLabel,int>map_label_freq;
       double get_distance(tData *d1,tData *t2);
public:
       KNN(int k,int rowLen,int colLen,char * filename);
       void get_all_distance();
       tLabel get_max_freq_label();
       void auto_norm_data();
       void get_error_rate();
       struct CmpByValue 
       {
           bool operator() (const PAIR& lhs,const PAIR &rhs)
           {
               return lhs.second < rhs.second;
           }
       };
       ~KNN();
};
KNN::~KNN()
{
    fin.close();
    fout.close();
    map_index_dis.clear();
    map_index_dis.clear();
}
KNN::KNN(int k,int row,int col,char *filename)
{
    this->rowLen = row;
    this->colLen = col;
    this->k = k;
    test_data_num = 0;
    fin.open(filename);
    if(!fin || !fout)
    {
        cout << "can not open the file"<<endl;
        exit(0);
    }
    for(int i=0;i<rowLen;i++)
    {
        for(int j=0;j<colLen;j++)
        {
            fin>>dataSet[i][j];
            fout<<dataSet[i][j]<<" ";
        }
        fin>>labels[i];
        fout<<labels[i]<<endl;
    }
}
void KNN::get_error_rate()
{
    int i,j,count = 0;
    tLabel label;
    cout << "please input the num of test data: "<<endl;
    cin>>test_data_num;
    for(i=0;i<test_data_num;i++)
    {
        for(j=0;j<colLen;j++)
        {
            testData[j] = dataSet[i][j];
        }
        get_all_distance();
        label = get_max_freq_label();
        if(label != labels[i])
        {
            count++;
        }
        map_index_dis.clear();
        map_label_freq.clear();
    }
    cout<<"the error rate is = "<<(double)count/(double)test_data_num<<endl; 
}
double KNN:: get_distance(tData *d1,tData *d2)
{
	double sum = 0;
	for(int i=0;i<colLen;i++)
	{
		sum += pow( (d1[i]-d2[i]) , 2 );
	}
 
	//cout<<"the sum is = "<<sum<<endl;
	return sqrt(sum);
}
void KNN:: get_all_distance()
{
	double distance;
	int i;
	for(i=test_data_num;i<rowLen;i++)
	{
		distance = get_distance(dataSet[i],testData);
		map_index_dis[i] = distance;
	}
 
//	map<int,double>::const_iterator it = map_index_dis.begin();
//	while(it!=map_index_dis.end())
//	{
//		cout<<"index = "<<it->first<<" distance = "<<it->second<<endl;
//		it++;
//	}
 
}
tLabel KNN::get_max_freq_label()
{
    vector<PAIR>vec_index_dis(map_index_dis.begin(),map_index_dis.end());
    sort(vec_index_dis.begin(),vec_index_dis.end(),CmpByValue());
    for(int i=0;i<k;i++)
    {
        map_label_freq[labels[vec_index_dis[i].first]]++;
    }
    map<tLabel,int>::const_iterator map_it = map_label_freq.begin();
    tLabel label;
    int max_freq = 0;
    while(map_it != map_label_freq.end())
    {
        if(map_it -> second > max_freq)
        {
            max_freq = map_it->second;
            label = map_it->first;
        }
        map_it ++;
    }
    return label;
}
//每一组特征值都进行归一化:0 ~ 1
//公式:(当前值 - 该特征值列最小值) / (该特征值列最大值 - 该特征值列最小值)
void KNN::auto_norm_data()
{
    tData maxa[colLen];
    tData mina[colLen];
    tData range[colLen];

    int i,j;

    for(i=0;i<colLen;i++)
    {
        maxa[i] = max(dataSet[0][i],dataSet[1][i]);
        mina[i] = min(dataSet[0][i],dataSet[1][i]);
    }
    for(i=2;i<rowLen;i++)
    {
        for(j=0;j<colLen;j++)
        {
            if(dataSet[i][j]>maxa[j])
            {
                maxa[j] = dataSet[i][j];
            }
            else if(dataSet[i][j]<mina[j])
            {
                mina[j] = dataSet[i][j];
            }
        }
    }
    for(i=0;i<colLen;i++)
    {
        range[i] = maxa[i] - mina[i];
        testData[i] = (testData[i] - mina[i])/range[i];
    }
    for(i=0;i<rowLen;i++)
    {
        for(j=0;j<colLen;j++)
        {
            dataSet[i][j] = ( dataSet[i][j] - mina[j] )/range[j];
        }
    }
}
int main(int argc,char ** argv)
{
    int k,row,col;
    char *filename;
    if(argc!=5)
    {
        cout<<"The input should be like this:./a.out k row col filename"<<endl;
        exit(1);
    }
    k = atoi(argv[1]);
    row = atoi(argv[2]);
    col = atoi(argv[3]);
    filename = argv[4];

    KNN knn(k,row,col,filename);
    knn.auto_norm_data();
    knn.get_error_rate();
    //knn.get_all_distance();
    //cout<<"You will probably like this person : "<<knn.get_max_freq_label()<<endl;
    return 0;
}