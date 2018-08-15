#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

// iou 两个矩形的交叉面积所占比例
// 0 - 1; 0 无交叉, 1 完全重叠

double IOU(const Rect& r1, const Rect& r2)
{
    int x1 = std::max(r1.x, r2.x);
    int y1 = std::max(r1.y, r2.y);
    int x2 = std::min(r1.x+r1.width, r2.x+r2.width);
    int y2 = std::min(r1.y+r1.height, r2.y+r2.height);
    int w = std::max(0, (x2-x1+1));
    int h = std::max(0, (y2-y1+1));
    double inter = w * h;
    double o = inter / (r1.area() + r2.area() - inter);
    return (o >= 0) ? o : 0;
}

void nms(vector<Rect>& proposals, const double nms_threshold)
{
    vector<int> scores;
    // 传入scores ,这里为了检测算法，矩形的面积当做scores
    for(auto i : proposals) scores.push_back(i.area());
    //矩形索引 {0,1,2,3,4,...}
    vector<int> index;
    for(int i = 0; i < scores.size(); ++i){
        index.push_back(i);
    }
    //根据scores从大到小排序后的矩形索引
    //{12,4,5,9,...}
    sort(index.begin(), index.end(), [&](int a, int b){
        return scores[a] > scores[b];
    }); 
    //每一个矩形进行标记是否要被删除
    vector<bool> del(scores.size(), false);
    for(size_t i = 0; i < index.size(); i++){
        if( !del[index[i]]){
            //选择score最高的一个(一般不会被删除)
            for(size_t j = i+1; j < index.size(); j++){
                //从剩余（i+1)个矩形中，依次和选择的矩形计算交叉面积比;
                //如果交叉面积的比值小于等于 设置的阈值 则证明两个矩形的不像似可以保留
                //否则被删除,被删除的矩形的index值被标记为true
                if(IOU(proposals[index[i]], proposals[index[j]]) > nms_threshold){
                    del[index[j]] = true;
                }
            }
        }
    }
    //将非true的index的矩形为新的备选矩形
    vector<Rect> new_proposals;
    for(const auto i : index){
        if(!del[i]) new_proposals.push_back(proposals[i]);
    }
    proposals = new_proposals;
}

void test_nms(char *path)
{
    Mat img = imread(path);
    Mat img2 = img.clone();

    // prepare data
    vector<Rect> proposals;
    Point2i origin(20, 20);
    int w = 100;
    int h = 200;
    for (auto i : {0,1,2,3,4,5}) proposals.push_back(Rect(origin.x + 10*i, origin.y + 20*i, w - 5*i, h-7*i));
    origin.x = 200;
    origin.y = 400;
    w = 300;
    h = 120;
    for (auto i : {0,1,2,3,4,5}) proposals.push_back(Rect(origin.x + 10*i, origin.y + 20*i, w - 5*i, h-7*i));

    proposals.push_back(Rect(400, 200, 386, 239));
    proposals.push_back(Rect(200, 120, 186, 209));
    proposals.push_back(Rect(310, 376, 286, 139));

    for(auto rect : proposals) rectangle(img, rect, Scalar(0, 255, 255), 2);
    imshow("before", img);
    // nms
    nms(proposals, 0.1);
    for(auto rect : proposals) rectangle(img2, rect, Scalar(0, 255, 255), 2);
    imshow("after", img2);
    waitKey(0);
    destroyAllWindows();
}
int main(int argc ,char **argv)
{
    test_nms(argv[1]);
    return 0;
}