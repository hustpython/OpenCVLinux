## C++实现SVM
## 数据集
wine_data
特征值:13个
样本个数:178个
## 输出
alpha
## 使用

- 编译

依赖库

jsoncpp,scons

(安装方法   : https://blog.csdn.net/tanswer_/article/details/73104931)

mkdir build

cd build

cmake ..

make

- 运行

./SVM ../resource/wine.mat

## 其他

[python 代码实现](src/python/)

[libsvm 代码实现](src/libsvmtest/)