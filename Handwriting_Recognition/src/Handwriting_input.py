#coding=utf-8

import preprocessImage as mImage
import os

testDir = '../test_data'

def getImageFiles():
    if os.path.exists(testDir) == False:
        break
    fileList = os.listdir(testDir)
    rList = []
    for eachFile in fileList:
        if os.path.isfile(eachFile) == False:
            rList.append(eachFile)
    return rList

def preprocessAImage(imageName):
    image = mImage(testDir,imageName)
    image.blur()
    # edge_detection
    # hough
    # getLineEquation
    cropImage = image.correctA4()
    return cropImage

def getSingleCharacter(cropImage):
    characters = cutPicInSinglePic(cropImage)
    return characters










