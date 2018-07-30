#rm -rf build
if [ ! -d "build/" ];then
echo "创建build文件夹"
mkdir build
else
echo "build文件夹存在"
fi
cd build
cmake ..
if [ ! -d "bin/" ];then
echo "创建bin文件夹"
mkdir bin
else
echo "bin文件夹已经存在"
fi
make -j8
