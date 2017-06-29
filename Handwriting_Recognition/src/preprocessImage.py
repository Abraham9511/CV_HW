# -*- coding: UTF-8 -*-

from PIL import Image, ImageFilter
from math import sin, cos
from skimage import feature
import numpy as np
import adboost

mnistSize = 28

class mImage:
    def __init__(self, filepath, filename):
        #self.BLUR = 5;
        self.filepath = filepath
        self.filename = filename
        self.originPhoto = Image.open(filepath)
        self.originPhotoArr = np.array(self.originPhoto)
        self.greyPhoto = self.originPhoto.convert('L')
        self.greyPhotoArr = np.array(self.greyPhoto)
        self.width = self.originPhoto.width
        self.height = self.originPhoto.height
        self.A4Width = 210
        self.A4Height = 297

    def blur(self, BLUR = 5):
        self.bImage = self.originPhoto.filter(ImageFilter.GaussianBlur(BLUR))

    def edge_detection(self, sigma = 5, low_threshold = 256*0.1, high_threshold = 256*0.2):
        imageArr = np.array(self.bImage)
        edgePhotoBool = feature.canny(imageArr, sigma, low_threshold, high_threshold)
        edgePhotoArr = np.uint8(edgePhotoBool)
        """
        for i in range(self.width):
            for j in range(self.height):
                if edgePhotoArr[i][j] == 1:
                    edgePhotoArr[i][j] = 1
        """
        return edgePhotoArr

    def hough(self):
        h = dotDistance(self.width, self.height)
        w = 360
        hough = np.zeros((w, h))
        edgePhotoArr = edge_detection()

        for i in range(self.width):
            for j in range(self.height):
                if edgePhotoArr[i][j] == 1:
                    for ii in range(width):
                        p = int(i* cos(ii/np.pi) + j * sin(ii/np.pi))
                        if p < h and p >= 0:
                            hough[ii][p] += 1

        return hough

    def getLineEquation(self):
        temp = list(hough())
        w = hough.shape[0]
        h = hough.shape[1]
        size = hough.shape[0] * hough.shape[1]
        temp = np.reshape((1, size))
        index = np.argsort(temp)
        top4 = [-1,-1,-1,-1]
        for i in range(size):
            if index[0][i] < 4 and index[0][i] >= 0:
                top4[index[0][i]] = i

        print(top4)

        foruLine = []
        for item in top4:
            s = item%h
            theta = item/w
            b = s/sin(theta/np.pi)
            if theta == 0:
                k = None
            else:
                k = -1/tan(theta/np.pi)
            coor = {k: k, b: b}
            foruLine.push(coor)
        return fourLine

    def getIntersections(self, fourLine, kthreshold):
        intersections = []
        for i in range(4):
            for j in range(i+1, 4):
                if fourLine[i].k == None and fourLine[j].k == None:
                    continue
                else if fourLine[i].k == None:
                    x = fourLine[i].b
                    y = fourLine[j].k * x + fourLine[j].b
                else if fourLine[j].k == None:
                    x = fourLine[j].b
                    y = fourLine[i].k * x + fourLine[i].b
                else if abs(fourLine[i].k - fourLine[j].k) < kthreshold:
                    continue
                else:
                    x = -(b1-b2)/(k1-k2)
                    y = (b1*k2-b2*k1)/(k2-k1)
                intersections.push({x:x, y:y})

        print(intersections)
        sortIntersections(intersections)
        print(intersections)
        return intersections

    def correctA4(self, intersections):
        H = squareToQuadrilateral(intersections)
        print(H)

        x1 = intersections[0].x
        y1 = intersections[0].y
        x2 = intersections[1].x
        y2 = intersections[1].y
        x3 = intersections[2].x
        y3 = intersections[2].y
        x4 = intersections[3].x
        y4 = intersections[3].y

        d12 = dotDistance(np.array([x1, y1]), np.array([x2,y2]))
        d34 = dotDistance(np.array([x3, y3]), np.array([x4,y4]))
        d14 = dotDistance(np.array([x1, y1]), np.array([x4,y4]))
        d23 = dotDistance(np.array([x3, y3]), np.array([x2,y2]))
        
        width = d12 > d34 ? d12 : d34
        height = d14 > d23 ? d14 : d23

        if width > height:
            height = width/297*210
        else:
            height = width/210*297

        result = np.zeros((self.width, self.height, 3))
        for x in range(self.width):
            for y in range(self.height):
                _x = x / width
                _y = y / height
                denominator = H[0,2] * _x + H[1,2] * _y + H[2,2]
                tx = (H[0,0] * _x + H[1,0] * _y + H[2,0]) /denominator
                ty = (H[0,1] * _x + H[1,1] * _y + H[2,1]) /denominator
                if tx >= 0 and tx < self.width and ty >= 0 and ty < self.height:
                    result[x,y,0] = self.originPhotoArr[tx,ty,0]
                    result[x,y,1] = self.originPhotoArr[tx,ty,1]
                    result[x,y,2] = self.originPhotoArr[tx,ty,2]
        return result


    def train():
        self.classifiers = []
        # load 10 catagories data, 0-9，此处需要读如数据，然后将label上面的

        for i in range(10):
            self.classifiers.append(adaBoostTrainDS(dataArr, classLabels[i]))

    def predict(dataArr):
        classifiers = self.classifiers.copy()
        retArray = np.array(np.zeros(dataArr.shape[0],1))
        m = np.shape(np.mat(dataArr))[0]
        for i in range(m):
            for j in range(10):
                result = adaClassify(dataArr)
                if result[0][0] == 1:
                    retArray[i][0] = j
                    break
        return retArray

    def squareToQuadrilateral(self, intersections):
        x1 = intersections[0].x
        y1 = intersections[0].y
        x2 = intersections[1].x
        y2 = intersections[1].y
        x3 = intersections[2].x
        y3 = intersections[2].y
        x4 = intersections[3].x
        y4 = intersections[3].y

        dx3 = x1 - x2 + x3 - x4
        dy3 = y1 - y2 + y3 - y4

        if (dx3 == 0.0 && dy3 == 0.0) 
            result = np.array([[x2-x1, y2-y1, 0], [x3-x2, y3-y2, 0], [x1, y1, 1]])
        else 
            dx1 = x2-x3
            dy1 = y2-y3
            dx2 = x4-x3
            dy2 = y4-y3
            denominator = dx1 * dy2 - dy2 * dy1
            a13 = (dx3 * dy2 - dx2 * dy3) / denominator
            a23 = (dx1 * dy3 - dy3 * dy1) / denominator
            result = np.array([[x2-x1+a13*x2, y2-y1+a13*y2, a13], [x4-x1+a23*x4, y4-y1+a23*y4, a23], [x1, y1, 1]])
        return result

    # 分割图片的数字成一个个数字
    def cutPicInSiglePic(self, imageArr):
        w = imageArr.shape()[0]
        h = imageArr.shape()[1]
        horizontalArr = horizontalCut(imageArr)
        
        tempHPicArr = [] 
        for i in range(0, len(horizontalArr)):
            start = horizontalArr[i].start
            end = horizontalArr[i].end
            tempHPicArr.append(imageArr[start:end,])

        singlePicArr = []
        tempVPicArr = []
        for i in range(0, len(tempPicArr)):
            tempVPicArr = verticalCut(tempPicArr[i])
            for j in range(0, len(tempVPicArr)):
                start = tempVPicArr[i].start
                end = tempVPicArr[i].end
                temp = tempPicArr[i][,start:end]
                temp = np.array(Image.fromarray(temp).resize((mnistSize, mnistSize), Image.ANTIALIAS))
                singlePicArr.append(temp)  
        return singlePicArr
            
    # 根据y轴分割        
    def horizontalCut(self, imageArr, padding = 4):
        w = imageArr.shape()[0]
        h = imageArr.shape()[1]

        # 矩阵中水平求和
        horizontalLine = np.sum(imageArr, axis = 1) 

        in_block = False
        start = 0
        end = 0
        horizontalArr = []
        for i in range(0, h):
            if in_block == False and horizontalLine[i] != 0:
                in_block = True
                start = i
            else if in_block == True and (horizontalLine[i] == 0 or i+1 == h):
                in_block = False
                end = i
                start = start-padding if start-padding >= 0 else start = 0
                end = end+padding if end+padding < h else end = h-1
                lines = {start:start, end:end}
                horizontalArr.append(lines)
        return horizontalArr

    # 根据x轴分割
    def verticalCut(self, imageArr, padding = 4):
        w = imageArr.shape()[0]
        h = imageArr.shape()[1]

        # 矩阵中垂直方向求和
        verticalArr = np.sum(imageArr, axis = 0)

        in_block = False
        start = 0
        end = 0
        verticalArr = []
        for i in range(0, w):
            if in_block == False and verticalArr[i] != 0:
                in_block = True
                start = i
            else if in_block == True and (verticalArr[i] == 0 or i+1 == w):
                in_block = False
                end = i
                start = start-padding if start-padding >= 0 else start = 0
                end = end+padding if end+padding < w else end = w-1
                lines = {start:start, end:end}
                verticalArr.append(lines)
        return verticalArr

    def sortIntersections(self, intersections):
        intersections.sort()
        temp = intersections[0]
        intersections[0] = intersections[1]
        intersections[1] = temp

    def dotDistance(self, coords1, coords2):
        return np.sqrt(np.sum((coords1-coords2)**2))


