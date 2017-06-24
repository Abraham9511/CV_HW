# -*- coding: UTF-8 -*-

from PIL import Image, ImageFilter
from math import sin, cos
from skimage import feature
import numpy as np

class mImage:
    def __init__(self, filepath, filename):
        #self.BLUR = 5;

        self.filepath = filepath
        self.filename = filename
        self.originPhoto = Image.open(filepath)
        self.originPhotoArr = np.matr(self.originPhoto.getdata())
        self.greyPhoto = self.originPhoto.convert('L')
        self.greyPhotoArr = np.array(self.greyPhoto.getdata())
        self.width = self.originPhoto.width
        self.height = self.originPhoto.height

    def blur(self, image, BLUR = 5):
        return image.filter(ImageFilter.GaussianBlur(BLUR))

    def edge_detection(self, imageArr, sigma = 5, low_threshold = 256*0.1, high_threshold = 256*0.2):
        edgePhotoBool = feature.canny(imageArr, sigma, low_threshold, high_threshold)
        edgePhotoArr = np.uint8(edgePhotoBool)
        for i in range(self.width):
            for j in range(self.height):
                if edgePhotoArr[i][j] == 1:
                    edgePhotoArr[i][j] = 255
        return edgePhotoArr

    def hough(self, imageArr):
        h = dotDistance(self.width, self.height)
        w = 360
        hough = np.zeros((w, h))
        edgePhotoArr = edge_detection(self.imageArr)

        for i in range(self.width):
            for j in range(self.height):
                if edgePhotoArr[i][j] == 255:
                    for ii in range(width):
                        p = int(i* cos(ii/np.pi) + j * sin(ii/np.pi))
                        if p < h and p >= 0:
                            hough[ii][p] += 1

        temp = list(hough)
        sorted(temp)
        print(temp[0:4])

    def getLineEquation(self, hough) {

    }


    def dotDistance(self, coords1, coords2):
        return np.sqrt(np.sum((coords1-coords2)**2))


