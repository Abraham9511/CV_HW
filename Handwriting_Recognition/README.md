# 手写体识别
***

## 要求

1. 校正图像为标准的A4纸
2. adaboost或者svm训练一个手写体数字的分类器
3. 切割字符
4. 识别并且输出连串数字
5. 完整的报告

## 使用的编程语言
* python（优先）
* c++

## 我的计划

1.1 边缘检测 ok
	https://cyroforge.wordpress.com/2012/01/21/canny-edge-detection
1.2 hough变换 ok
	参考内容
		1.2.1 http://blog.csdn.net/abcjennifer/article/details/7448513

1.3 求得A4纸边缘方程再而获得四个交点 ok
1.4 Wraping变成普通A4纸张
	1.4.1 http://www.voidcn.com/blog/xiaowei_cqu/article/p-120336.html
	？？ 透视变换的原理
	http://www.voidcn.com/blog/Simba_Scorpio/article/p-5768073.html

2.1 了解什么是adaboost ok
	2.1.1 [Adaboost](https://zh.wikipedia.org/wiki/AdaBoost)
	2.1.2 [Adaboost介绍](http://blog.csdn.net/v_july_v/article/details/40718799)
	2.1.3 [Mnist](http://www.jianshu.com/p/81f8ca1b722f)
2.2 写训练器 ok
2.3 写预测器 ok
2.4 写组装函数train

3.1 切割字符？？ 
	沿着xy遍历切割图片找出最小的框框住数字
	3.1.1 [](http://www.cs.dartmouth.edu/reports/TR98-341.pdf)
	3.1.2 [](http://yobin.sinaapp.com/topic/2643/%E7%94%A8python%E8%BF%9B%E8%A1%8C%E7%AE%80%E5%8D%95%E7%9A%84%E5%9B%BE%E7%89%87%E8%AF%86%E5%88%AB-1)
	3.1.3 [](https://www.zhihu.com/question/26085954)
	3.1.4 [](http://blog.csdn.net/liuuze5/article/details/45791353)
4. 使用训练器预测输出 ok

5. 报告

??
cross-entropy

采用deep_cnn去训练模型
cross-entropy between the target and the softmax activation function applied to the model's prediction

One should generally initialize weights with a small amount of noise for symmetry breaking, and to prevent 0 gradients

