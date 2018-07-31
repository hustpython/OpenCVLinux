## Exmaple 1
A , B 分类
./classfiAB ../resource/data/classfidata.txt
## Example 2
特征值:飞行里程,玩游戏时间比例,吃冰激凌比例
标签:人名 
- task1:
对1000组数据,抽取一定比例进行验证error_rate

 ./classDating 7 1000 3 ../resources/data/datingTestSet.txt
- task2:
输入一组数据(3个特征)
预测其标签

./classDatingTest 7 1000 3 ../resources/data/datingTestSet.txt 

## 来源
https://blog.csdn.net/lavorange/article/details/16924705
