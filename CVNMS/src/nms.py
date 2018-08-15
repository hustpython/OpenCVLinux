# nms python
import cv2 as cv
import numpy as np
import sys
def py_nms(dets, thresh):
    """Pure Python NMS baseline."""
    #x1、y1、x2、y2、以及score赋值
    x1 = dets[:, 0]
    y1 = dets[:, 1]
    x2 = dets[:, 2]
    y2 = dets[:, 3]
    scores = dets[:, 4]

    #每一个候选框的面积
    areas = (x2 - x1 + 1) * (y2 - y1 + 1)
    #order是按照score降序排序的
    order = scores.argsort()[::-1]
    keep = []
    while order.size > 0:
        i = order[0]
        keep.append(i)
        #计算当前概率最大矩形框与其他矩形框的相交框的坐标
        xx1 = np.maximum(x1[i], x1[order[1:]])
        yy1 = np.maximum(y1[i], y1[order[1:]])
        xx2 = np.minimum(x2[i], x2[order[1:]])
        yy2 = np.minimum(y2[i], y2[order[1:]])

        #计算相交框的面积
        w = np.maximum(0.0, xx2 - xx1 + 1)
        h = np.maximum(0.0, yy2 - yy1 + 1)
        inter = w * h
        #计算重叠度IOU：重叠面积/（面积1+面积2-重叠面积）
        ovr = inter / (areas[i] + areas[order[1:]] - inter)

        #找到重叠度不高于阈值的矩形框索引
        inds = np.where(ovr <= thresh)[0]
        #将order序列更新，由于前面得到的矩形框索引要比矩形框在原order序列中的索引小1，所以要把这个1加回来
        order = order[inds + 1]
    return keep

def genreateproposal(rect,proposal,n):
    for i in range(1,n+1):
        x1 = rect[0]+i*10
        y1 = rect[1]+i*10
        x2 = rect[2]+i*5
        y2 = rect[3]+i*5
        score = (x2 - x1 + 1) * (y2 - y1 + 1)
        proposal.append([x1,y1,x2,y2,score])
    return proposal
def prepareproposal(path):
    img =  cv.imread(path)
    imgmultiproposals = img.copy()
    proposals = []
    init_react1 = [10,10,200,300]
    # 4 个
    proposals = genreateproposal(init_react1,proposals,4)

    init_react2 = [100,120,300,500]

    proposals = genreateproposal(init_react2,proposals,5)

    proposals = np.array(proposals)

    for rec in proposals:
        cv.rectangle(imgmultiproposals,(rec[0],rec[1]), (rec[2],rec[3]),(0,0,255)) 
    cv.imshow("before",imgmultiproposals)

    keepindex = py_nms(proposals,0.1)
    imgnms = img.copy()
    for i in keepindex:
        rec = proposals[i]
        cv.rectangle(imgnms,(rec[0],rec[1]), (rec[2],rec[3]),(0,0,255)) 
    cv.imshow("after",imgnms)

    cv.waitKey(0)
    cv.destroyAllwindows()

if __name__ == "__main__":
    prepareproposal(sys.argv[1])