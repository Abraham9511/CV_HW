# Panorama Image Stitching（拼接全景图片）
***

## 采用的代码实现
C++

## 实现的步骤
1. 使用三脚架或者徒手拍摄照片
2. Wrap照片到球形坐标
3. 提取特征 
4. 使用RANSAC赋值相邻对
5. 写出相邻变换
6. 对图像进行修正
7. 读取图像并且拼接
8. 收获结构并且输出

## 流程
1. 将测试图片转换成bmp格式并且重命名为1，2，3...（2017.4.28）
2. 阅读课件，以及论文 （！订正：后来采取了上网找博客笔记，理清做的思路，然后再阅读论文）
3. 写代码：  
	3.1 设计.h文件  
	3.2 实现.cc文件  
	3.3 迭代测试效果

### 作法
拼接的步骤包括配准和融合
配准的方法主要有基于特征点和模版的匹配
融合的方法有
1. 线性过渡
2. 拉普拉斯金字塔融合（多波段）
3. 小波变换
4. 柏松图像编辑融合



## 参考资料
1. [华盛顿大学的课程](http://courses.cs.washington.edu/courses/cse576/08sp/projects/project2/project2.html)
2. A Multiresolution Spline With Application to Iamge Mosaics
3. [机器视觉：SIFT Matching with RANSAC](http://yongyuan.name/blog/SIFT(ASIFT)-Matching-with-RANSAC.html) 重点放在匹配算法SIFT的讲解上，以及优化效果ASIFT算法
4. [opencv实现1](http://www.voidcn.com/blog/pi9nc/article/p-4789003.html)
5. [opencv实现2](http://blog.csdn.net/masibuaa/article/details/9246493)
6. [用广义霍夫变换筛选sift特征点](http://www.voidcn.com/blog/u010278305/article/p-3389866.html)
7. [sift & ransac](http://www.voidcn.com/blog/wangezhu/article/p-368877.html)
8. 

