#include <iostream>
#include <map>
#include <vector>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <fstream>

using namespace std;

typedef char tLabel;
typedef double tData;
typedef pair<int,double> PAIR;
const int colLen = 2;
const int rowLen = 12;

ifstream fin;
ofstream fout;

class KNN 
{
private:
         tData dataSet[rowLen][colLen];
         tLabel labels[rowLen];
         tData testData[colLen];

         int k;
         map<int,double> map_index_dis;
         map<tLabel,int> map_label_freq;

         double get_distance(tData *t1,tData *d2);
public: 
    KNN(int k,char *datapath);
    void get_all_distance();
    void get_max_freq_label();

    struct CmpByValue 
    {
        bool operator()(const PAIR&lhs,const PAIR&rhs)
        {
            return lhs.second < rhs.second;
        }
    };

};

KNN::KNN(int k,char *datapath)
{
    this->k = k;
    fin.open(datapath);
    if(!fin)
    {
        cout << "can not open the passing data"<<endl;
        exit(1);
    }
     /* input the dataSet */ 
	for(int i=0;i<rowLen;i++)
	{
		for(int j=0;j<colLen;j++)
		{
			fin>>dataSet[i][j];
		}
		fin>>labels[i];
	}
 
	cout<<"please input the test data :"<<endl;
	/* inuput the test data */
	for(int i=0;i<colLen;i++)
		cin>>testData[i];

}
double KNN::get_distance(tData *d1,tData *d2)
{
    double sum = 0;
    for(int i=0;i<colLen;i++)
    {
        sum += pow((d1[i]-d2[i]),2);
    }
    return sqrt(sum);
}
void KNN::get_all_distance()
{
    double distance;
    int i;
    for(i=0;i<rowLen;i++)
    {
        distance = get_distance(dataSet[i],testData);
        //<key,value> => <i,distance>
        map_index_dis[i] = distance;
    }
    map<int,double>::const_iterator it=map_index_dis.begin();
    while(it != map_index_dis.end())
    {
        cout << "index = " << it->first << "distance = "<<it->second<<endl;
        it++;
    }
}
void KNN::get_max_freq_label()
{
    vector<PAIR>vec_index_dis(map_index_dis.begin(),map_index_dis.end());
    sort(vec_index_dis.begin(),vec_index_dis.end(),CmpByValue());
    for(int i=0;i<k;i++)
    {
		cout<<"the index = "<<vec_index_dis[i].first<<" the distance = " \
        <<vec_index_dis[i].second<<" the label = "                       \
        <<labels[vec_index_dis[i].first]                                 \
        <<" the coordinate ( "<<dataSet[ vec_index_dis[i].first ][0]     \
        <<","<<dataSet[ vec_index_dis[i].first ][1]<<" )"<<endl;
        map_label_freq[labels[vec_index_dis[i].first]]++;
    }
    map<tLabel,int>::const_iterator map_it = map_label_freq.begin();
    tLabel label;
    int max_freq = 0;
    while (map_it != map_label_freq.end())
    {
        if(map_it->second > max_freq)
        {
            max_freq = map_it->second;
            label = map_it->first;
        }
        map_it ++;
    }
    cout << "The test data belongs to the  " << label <<" "<< "label" <<endl;
}
int main(int argc,char **argv)
{
    int k;
    cout << "Please input the k value:"<<endl;
    cin>>k;
    KNN knn(k,argv[1]);
    knn.get_all_distance();
    knn.get_max_freq_label();
    system("pause");
    return 0;
}