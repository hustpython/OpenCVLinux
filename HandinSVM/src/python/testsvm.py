# coding=utf-8
from collections import Counter
import ConfigParser
from svm import *
import numpy as np
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

# ==========将训练得到的参数写入配置文件中========
class Test():
    def __init__(self):
        self.a = []
        self.b = []
        self.C = 0.1
        self.inter = 1000
        self.kernel = "Line"
        #self.kernel = ['Gauss',1]
        self.conf = ConfigParser.ConfigParser()
        if "param1" not in self.conf.sections():
           self.conf.add_section("param1")
        if "param2" not in self.conf.sections():
           self.conf.add_section("param2")
        if "param3" not in self.conf.sections():
           self.conf.add_section("param3")

    def readconfig(self):

        self.conf.read('alphaconfig.ini')
        res1a = self.conf.get("param1", "alpha")
        res1b = self.conf.get("param1", "b")

        res2a = self.conf.get("param2", "alpha")
        res2b = self.conf.get("param2", "b")

        res3a = self.conf.get("param3", "alpha")
        res3b = self.conf.get("param3", "b")

        return[[res1a,res1b],[res2a,res2b],[res3a,res3b]]

    def writeconf(self,i): 
        self.conf.read('alphaconfig.ini')
        self.conf.set("param"+str(i), "alpha", self.a)
        self.conf.set("param"+str(i),"b",self.b)
        self.conf.write(open('alphaconfig.ini', 'w'))
    def train(self,data,testdata,testlabel):
            for k ,traindata in enumerate(data):
                print("正在训练数据！")
                error_rate = 1.1
                #for i in range(10):
                if k == 1:
                    while(error_rate>0.15):
                        svmtrain=SVM(traindata,self.kernel,self.inter,self.C,0.001)
                        svmtrain.train()
                        self.a = svmtrain.a 
                        self.b = svmtrain.b
                        print(error_rate)
                        test_error_rate = self.get_error(testdata[k],testlabel[k],[traindata],k,train=True)
                        if  test_error_rate < error_rate:
                            error_rate = test_error_rate 
                            self.writeconf(k+1)
                else:
                    for i in range(3):
                        svmtrain=SVM(traindata,self.kernel,self.inter,self.C,0.001)
                        svmtrain.train()
                        self.a = svmtrain.a 
                        self.b = svmtrain.b
                        test_error_rate = self.get_error(testdata[k],testlabel[k],[traindata],k,train=True)
                        if  test_error_rate < error_rate:
                            error_rate = test_error_rate 
                            self.writeconf(k+1)
                print(error_rate)

    def showres(self,data,prelist):
        svmtest=SVM(data,self.kernel,self.inter,self.C,0.001)
        svmtest.a = self.a
        svmtest.b = self.b
        res = svmtest.predict(prelist)
        return res[0]

# 找到多分类中频率最高的类标
    def findmaxfre(self,reslist):
        return Counter(reslist).most_common(1)[0][0]

# 验证错误率
    def get_error(self,data,label,svmdata,dataindex,train=False):
        count = 0
        for i,predictdata in enumerate(data):
            res = self.predict(predictdata,svmdata,dataindex,train)       
            if res != label[i]:
                count += 1
            #print(i+1,res,label[i])
        error_rate = float(float(count)/float(len(data)))
        #print("count:",str(count))
        #print("error_rate:",str(error_rate))
        return error_rate

#　返回预测结果
    def predict(self,predictlist,svmdata,dataindex,train):
        reslabel = []
        for j,data in enumerate(svmdata):
            if not train:
                self.a = [float(f) for f in eval(self.readconfig()[j][0])]
                self.b = float(self.readconfig()[j][1])
            res = self.showres(data,predictlist)
            labelindex = [1,-1].index(res)
            if dataindex <=0:
               reslabel.append(labellist[j][labelindex])
            else:
               reslabel.append(labellist[dataindex][labelindex])
        return self.findmaxfre(reslabel)

# 单独预测某一组数据
    def test(self,predictlist,svmdata,dataindex):
        print("正在预测！")
        res = self.predict(predictlist,svmdata,dataindex,False)
        print(str(res)+"类")

# 主函数　原来验证错误率和预测数据
if __name__ == '__main__':
    testdata = [[float("%.2f"%f) for f in sample[1:]] for sample in sn_data]
    testdatalabel = [int(sample[0]) for sample in sn_data]
    testclass = Test()
    traindata = [data12,data23,data31]
    testdatalist = [testdata[:sn1+sn2],testdata[sn1:],testdata[:sn1]+testdata[sn1+sn2:]]
    testdatalabellist = [testdatalabel[:sn1+sn2],testdatalabel[sn1:],testdatalabel[:sn1]+testdatalabel[sn1+sn2:]]
    #testclass.train(traindata,testdatalist,testdatalabellist)
    threeclass_rate = testclass.get_error(testdata,testdatalabel,traindata,-1)
    print(1-threeclass_rate)
    #prelist = [14.13, 4.1, 2.74, 24.5, 96.0, 2.05, 0.76, 0.56, 1.35, 9.2, 0.61, 1.6, 560.0] # 1类
    #testclass.test(prelist,traindata,-1)