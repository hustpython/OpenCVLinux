from math import log

class DecisonTree:
    trainData = []
    trainLabel = []
    featureValus = {} #每个特征所有可能的取值

    def __init__(self, trainData, trainLabel, threshold):
        self.loadData(trainData, trainLabel)
        self.threshold = threshold
        self.tree = self.createTree(range(0,len(trainLabel)), range(0,len(trainData[0])))


    #加载数据
    def loadData(self, trainData, trainLabel):
        if len(trainData) != len(trainLabel):
            raise ValueError('input error')
        self.trainData = trainData
        self.trainLabel = trainLabel

        #计算 featureValus
        for data in trainData:
            for index, value in enumerate(data):
                if not index in self.featureValus.keys():
                    self.featureValus[index] = [value]
                if not value in self.featureValus[index]:
                    self.featureValus[index].append(value)
        # 统计每一个属性对应的特征类别
        # 比如年龄有三个类别:青年，中年，老年->0:[0,1,2]
    #计算信息熵
    # H(x) = -sum(pi*log(pi))
    # 这里是 0 和 1 
    def caculateEntropy(self, dataset):
        labelCount = self.labelCount(dataset)
        size = len(dataset)
        result = 0
        for i in labelCount.values():
            pi = i / float(size)
            result -= pi * (log(pi) /log(2))
        return result

    #计算信息增益
    # 特征AA对数据集DD的经验条件熵H(D|A)
    # ID3
    def caculateGain(self, dataset, feature):
        # 若特征为年龄
        values = self.featureValus[feature] #特征feature 所有可能的取值
        result = 0
        for v in values:
            # 年龄中的 青年 中年 老年
            subDataset = self.splitDataset(dataset=dataset, feature=feature, value=v)
            # 返回属性中某一个的特征
            result += len(subDataset) / float(len(dataset)) * self.caculateEntropy(subDataset)
        return self.caculateEntropy(dataset=dataset) - result
    # C4.5
    def calculateCondition(self,dataset,feature):
        values = self.featureValus[feature]
        res = [ 0  for i in range(len(values))]
        for i in dataset:
            indexfea = values.index(self.trainData[i][feature])
            res[indexfea] += 1
        returnres = 0
        for j in res:
            pi = j/float(len(dataset))
            returnres -= pi * (log(pi) /log(2))
        return returnres 
    def caculateGainratio(self,dataset,feature):
        # 若特征为年龄
        values = self.featureValus[feature] #特征feature 所有可能的取值
        result = 0
        for v in values:
            # 年龄中的 青年 中年 老年
            subDataset = self.splitDataset(dataset=dataset, feature=feature, value=v)
            # 返回属性中某一个的特征
            result += len(subDataset) / float(len(dataset)) * self.caculateEntropy(subDataset)
        calculateCondition = self.calculateCondition(dataset,feature)
        return (self.caculateEntropy(dataset=dataset) - result) / calculateCondition

    #计算数据集中，每个标签出现的次数
    # {0:6,1:9}
    def labelCount(self, dataset):
        labelCount = {}
        for i in dataset:
            if trainLabel[i] in labelCount.keys():
                labelCount[trainLabel[i]] += 1
            else:
                labelCount[trainLabel[i]] = 1
        return labelCount

    '''
    dataset:数据集
    features:特征集
    '''
    def createTree(self, dataset, features):

        labelCount = self.labelCount(dataset)
        #如果特征集为空，则该树为单节点树
        #计算数据集中出现次数最多的标签
        if not features:
            return max(list(labelCount.items()),key = lambda x:x[1])[0]

        #如果数据集中，只包同一种标签，则该树为单节点树
        if len(labelCount) == 1:
            return (list(labelCount.keys())[0])

        #计算特征集中每个特征的信息增益
        # features中的每一个x [0,1,2,3]
        # C4.5
        l = map(lambda x : [x, self.caculateGainratio(dataset=dataset, feature=x)], features)
        # ID3
        # l = map(lambda x : [x, self.caculateGain(dataset=dataset, feature=x)], features)
        #选取信息增益最大的特征
        feature, gain = max(l, key = lambda x: x[1])
        #如果最大信息增益小于阈值，则该树为单节点树
        #
        if self.threshold > gain:
            return max(list(labelCount.items()),key = lambda x:x[1])[0]

        tree = {}
        #选取特征子集
        subFeatures = filter(lambda x : x != feature, features)
        tree['feature'] = feature
        #构建子树
        for value in self.featureValus[feature]:
            subDataset = self.splitDataset(dataset=dataset, feature=feature, value=value)

            #保证子数据集非空
            if not subDataset:
                continue
            tree[value] = self.createTree(dataset=subDataset, features=subFeatures)
        return tree

    def splitDataset(self, dataset, feature, value):
        reslut = []
        for index in dataset:
            if self.trainData[index][feature] == value:
                reslut.append(index)
        return reslut

    def classify(self, data):
        def f(tree, data):
            if type(tree) != dict:
                return tree
            else:
                return f(tree[data[tree['feature']]], data)
        return f(self.tree, data)

if __name__ == '__main__':

    trainData = [
        [0, 0, 0, 0],
        [0, 0, 0, 1],
        [0, 1, 0, 1],
        [0, 1, 1, 0],
        [0, 0, 0, 0],
        [1, 0, 0, 0],
        [1, 0, 0, 1],
        [1, 1, 1, 1],
        [1, 0, 1, 2],
        [1, 0, 1, 2],
        [2, 0, 1, 2],
        [2, 0, 1, 1],
        [2, 1, 0, 1],
        [2, 1, 0, 2],
        [2, 0, 0, 0],
    ]

    trainLabel = [0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0]

    tree = DecisonTree(trainData=trainData, trainLabel=trainLabel, threshold=0)
    testdata = [2, 1, 0, 2]
    print(tree.tree)
    print(tree.classify(testdata))