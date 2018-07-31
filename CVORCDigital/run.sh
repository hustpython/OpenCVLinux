cd bin
if [  -f LabelData.yml ];then
./TestData resources/PiTest.png TrainingData.yml LabelData.yml
else
echo "没有发现数据文件"
fi
