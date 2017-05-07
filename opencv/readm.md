### OpenCV 计算机视觉库


### Install
[官网有](http://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html#linux-installation)

### 文档
官方文档相当好

### 对极几何 Epipolar Geometry
描述的是两幅视图之间的内在射影关系，与外部场景无关，只依赖于摄像机内参数和这两幅试图之间的的相对姿态
用来表述双视觉(立体视觉)的重要工具


### 功能
* 图片处理 
  机器视觉的基础

* 简单绘图,文字显示

* 直方图均衡化
  一种简介增强图片对比度得放方法
   直方图均衡化处理的“中心思想”是把原始图像的灰度直方
   图从比较集中的某个灰度区间变成在全部灰度范围内的均匀分布。
   直方图均衡化就是对图像进行非线性拉伸，重新分配图像像素值，
   使一定灰度范围内的像素数量大致相同

* 点检测,线检测

* 边缘检测
  通过滤波器完成,通过使用模板进行二位卷积
  常用方法,通过检测亮度的不连续, 计算亮度的一阶,二阶导数来检测边缘
  它们在恒定亮度区域中的值为零,而且它们的 值与可变亮度区域中的亮度变化程度有关
    ● 寻找亮度的一阶导数的幅度比指定阈值大的地方。
    ● 寻找亮度的二阶导数中有零交叉的位置。

  * 梯度向量

  * 通过滤器,模板各项的和为0
   [[-1, -1, -1],
    [-1,  8, -1],
    [-1, -1, -1]])

  * 低通过滤.
    均值滤波


  * 高斯滤波

  * 中值滤波
    使用邻域中统计中值作为该点的输出
    降噪效果很好

  * 双边滤波 bilateral filter
    由两个函数构成:
      1. 几何空间距离(与高斯一样)
      2. 像素差值距离(基于像素值相似性的高斯函数),中心点和邻域中每个点的相似度


  * Canny 轮廓发现
    基于图像梯度计算技术
    Canny算子的原理是首先在x,y方向求一阶导数，
    然后组合为4个方向的导数。这些方向的导数达到局部最大值的点就是组成边缘的候选点。
    1. 彩色图像转换为灰度图像
    2. 对图像进行高斯模糊
      过滤掉噪音
    3. 计算图像梯度，根据梯度计算图像边缘幅值与角度
    4. 非最大信号压制处理 Non-maxinum supperession(NMS)
       目的是消除多余的框,（边缘细化）
    5. 双阈值边缘连接处理
    6. 二值化图像输出结果


  API Laplacian sobel scharr

* 霍夫变换 Hough tranforms
    要用来从图像中分离出具有某种相同特征的几何
    形状（如，直线，圆等）。霍夫变换寻找直线与圆的方法相比与其它方法可以更好的减少噪
    声干扰。经典的霍夫变换常用来检测直线，圆，椭圆等




* 高斯模糊 Baussian Blur
  通常来减少图像噪声和低层次细节
  模板是根据高斯分布计算的
  磨砂玻璃效果


* 边缘增

* 锐化 sharpen

* NMS
 非极大值抑制(NMS)可以看做是局部最大值的搜索问题
 

### 卷积滤镜 convolution filter
一个模板, 用来决定周围像素如果决定当前像素


### 基本组件
* core
* highgui
  包含的大量的窗口交互功能


### 扩展模块
3版本以后需要独立安装扩展模块


### C++接口
* 所有的函数和定义都在 cv 命名空间下
* calcHist 得到图片的像素值分布直方图
*


### Python 接口
* 摄像头
```py
cameraCapture	=	cv2.VideoCapture(0)
fps	=	30	#	an	assumption
size	=	(int(cameraCapture.get(cv2.CAP_PROP_FRAME_WIDTH)),
								int(cameraCapture.get(cv2.CAP_PROP_FRAME_HEIGHT)))
videoWriter	=	cv2.VideoWriter(
				'MyOutputVid.avi',	cv2.VideoWriter_fourcc('I','4','2','0'),	fps,	size)
success,	frame	=	cameraCapture.read()
numFramesRemaining	=	10	*	fps	-	1
while	success	and	numFramesRemaining	>	0:
				videoWriter.write(frame)
				success,	frame	=	cameraCapture.read()
				numFramesRemaining	-=	1
cameraCapture.release()
```



