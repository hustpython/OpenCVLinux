# coding=utf-8
from svm import *
import numpy as np
from collections import Counter
import csv
# global var
# ========================
datafilepath = "wine.csv"
sn1 = 59
sn2 = 71
sn3 = 48
sn_data = np.loadtxt(datafilepath,delimiter=',') 
data12 = [[[float("%.2f"%f) for f in sample[1:]],1 if int(sample[0]) == 1 else -1] for sample in sn_data[:sn1+sn2]]
data23 = [[[float("%.2f"%f) for f in sample[1:]],1 if int(sample[0]) == 2 else -1] for sample in sn_data[sn1:]]
data31 = [[[float("%.2f"%f) for f in sample[1:]],1 if int(sample[0]) == 3 else -1] for sample in np.concatenate([sn_data[:sn1],sn_data[sn1+sn2:]],axis=0)]

labellist = [[1,2],[2,3],[3,1]]

#============================
# 简单的测试数据
# data = [
#         [[1,1],1],
#         [[2,1],1],
#         [[1,0],1],
#         [[3,7],-1],
#         [[4,8],-1],
#         [[4,10],-1],
#        ]
#　如果为gauss核的话  ['Gauss',标准差]

# 获取预测结果 1/-1
def showres(data,prelist):
    svm=SVM(data,'Line',1000,0.02,0.001)
    svm.train()
    #print("train done")
    res = svm.predict(prelist)
    # print(svm.a)
    # print(svm.w)
    # print(svm.b)
    return res[0]

# 找到多分类中频率最高的类标
def findmaxfre(reslist):
    return Counter(reslist).most_common(1)[0][0]

# 验证错误率
def get_error(data,label):
    count = 0
    print("正在验证错误率!")
    for i,predictdata in enumerate(data):
        res = predict(predictdata)       
        if res != label[i]:
            count += 1
        print(predictdata,i+1,res,label[i])
    print("count:",str(count))
    print("error_rate:",str(float(float(count)/float(len(data)))))

#　返回预测结果
def predict(predictlist):
    reslabel = []
    for j,data in enumerate([data12,data23,data31]):
        res = showres(data,predictlist)
        labelindex = [1,-1].index(res)
        reslabel.append(labellist[j][labelindex])
    return findmaxfre(reslabel)

# 单独预测某一组数据
def test(predictlist):
    print("正在预测！")
    res = predict(predictlist)
    print(str(res)+"类")

# 主函数　原来验证错误率和预测数据
if __name__ == '__main__':
    testdata = [[float("%.2f"%f) for f in sample[1:]] for sample in sn_data]
    testdatalabel = [int(sample[0]) for sample in sn_data]
    get_error(testdata,testdatalabel)
    prelist = [14.13, 4.1, 2.74, 24.5, 96.0, 2.05, 0.76, 0.56, 1.35, 9.2, 0.61, 1.6, 560.0] # 1类
    #test(prelist)