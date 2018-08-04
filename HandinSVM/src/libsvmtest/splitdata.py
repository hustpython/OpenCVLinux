file1 = "livsvmwine.txt"
fileout = "livsvmwinenolabel"
d=file(fileout, "w")
with open(file1) as f:
    lines = f.readlines()
    for line in lines:
        splitline = line.split(" ")
        for i in range(13):
            splitline[0]=''
            splitline[i+1]= " %d:"%(i+1)+splitline[i+1]
        a = ""
        for j in splitline[1:]:
            a += j 
        d.write(a)
                