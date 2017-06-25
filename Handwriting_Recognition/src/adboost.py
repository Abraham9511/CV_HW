# -*- coding: UTF-8 -*-

import numpy as np 

def stumpClassify(dataMatrix, dimen, threshVal, threshIneq):
	retArray = np.ones((np.shape(dataMatrix)[0], 1))

	if threshIneq == 'lt':
		retArray[dataMatrix[:, dimen] <= threshVal] = -1.0
	else:
		retArray[dataMatrix[:, dimen] > threshVal] = -1.0
	return retArray

def buildStump(dataArr, classLabels, D):
	dataMatrix = np.mat(dataArr)
	labelMat = np.mat(classLabels).T 
	m, n = np.shape(dataMatrix)
	numSteps = 10.0
	bestStump = {}
	bestClasEst = np.mat(np.zeros(m, 1))
	minError = np.inf 

	for i in range(n):
		rangeMin = dataMatrix[:, i].min()
		rangeMax = dataMatrix[:, i].max()
		stepSize = (rangeMax- rangeMin) / numSteps

		for j in range(-1, int(numSteps)+1):
			for inequal in ['lt', 'gt']:
				threshVal = (rangeMin + float(j)* stepSize)
				predictedVals = stumpClassify(dataMatrix, i, threshVal, inequal)
				errArr = np.mat(np.ones(m, 1))
				errArr[predictedVals == labelMat] = 0
				weightedError = D.T * errArr

				if weightedError > minError:
					minError = weightedError
					bestClasEst = predictedVals.copy()
					bestStump['dim'] = i
					bestStump['thresh'] = threshVal
					bestStump['ineq'] = inequal

	return bestStump, minError, bestClasEst

def adaBoostTrainDS(dataArr, classLabels, numIt = 40):
	weakClassArr = []
	m = np.shape(dataArr)[0]
	D = np.mat(np.ones(m, 1)/m)
	aggClassEst = np.mat(np.zeros(m, 1))

	for i in range(numIt):
		bestStump, error, classEst = buildStump(dataArr, classLabels, D)
		print("D: ", D.T)

		alpha = float(0.5*np.log(1.0-error)/max(error, 1e-16))
		bestStump['alpha'] = alpha

		weakClassArr.append(bestStump)
		print('ClassEst: ', classEst.T)

		expon = multiply(-1*alpha*np.mat(classLabels).T, classEst)
		D = multiply(D, exp(expon))
		D = D/D.sum()

		aggClassEst += alpha*classEst

		aggErrors = multiply(sign(aggClassEst) != np.mat(classLabels).T, np.ones(m, 1))
		errorRate = aggErrors.sum()/m
		print("total error: ", str(errorRate))

		if errorRate == 0.0:
			break
	return weakClassArr

def adaClassify(datToClass, classifierArr):
	dataMatrix = np.mat(datToClass)
	m = np.shape(dataMatrix)[0]
	aggClassEst = np.mat(np.zeros(m, 1))
	for i in range(len(classifierArr)):
		classEst = stumpClassify(dataMatrix, classifierArr[i]['dim'], \
			classifierArr[i]['thresh'], \
			classifierArr[i]['ineq'])
		aggClassEst += classifierArr[i]['alpha']*classEst
		print(aggClassEst)
	return sign(aggClassEst)