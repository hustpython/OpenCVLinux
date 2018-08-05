# 将非1/-1类标转化为 1,-1
# 将以逗号为特征分割符的数据转化为 index:data 的libxvm标准数据格式
file1 = "libsvmwine31.txt"
fileout = "libsvmwine31"
d=file(fileout, "w")
with open(file1) as f:
    lines = f.readlines()
    for line in lines:
        splitline = line.split(",")
        if splitline[0] == "3":
            splitline[0] == "1"
        else:
            splitline[0] == "-1"
        for i in range(13):
            splitline[i+1]= " %d:"%(i+1)+splitline[i+1]
        a = ""
        for j in splitline:
            a += j 
        d.write(a)
                