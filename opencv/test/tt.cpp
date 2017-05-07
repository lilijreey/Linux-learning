/**
 * @file     tt.cpp
 *
 */
#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using std::cout;

//鼠标事件回调
void onMouse(int evt, int x, int y, int flags, void *param) {
  cv::Mat &img = *reinterpret_cast<cv::Mat*>(param);

  switch (evt) {
  case CV_EVENT_LBUTTONDOWN: //left button down
    printf("left button down x:y(%d:%d)\n", x,y);
    break;
  }
}

int main(int argc, char *argv[])
{
  ///EE 创建一个空图片对象
  cv::Mat img;
  //cout << "this image size " << img.rows << "x" << img.cols << std::endl;
  

  ///EE 读取图片, 图片格式有限制
  //img = cv::imread("./tt.jpeg");
  //img = cv::imread("./tt.jpeg", CV_LOAD_IMAGE_GRAYSCALE); //灰度加载
  img = cv::imread("./tt.jpeg", CV_LOAD_IMAGE_COLOR); //three-channel color image
  printf("load\n");


  if (img.empty()) {
    printf("load failed\n");
    cout << "加载 xx.jepg failed" << std::endl;
    exit(1);
  }

  ///EE显示图片, 第一个参数是名字
  cv::namedWindow("win1", cv::WINDOW_AUTOSIZE);
  cv::imshow("win1", img);

  ///EE 翻转图片
  cv::flip(img, img, 1);

  cv::namedWindow("win2", cv::WINDOW_AUTOSIZE);

  ///EE 鼠标事件回调
  cv::setMouseCallback("win2", onMouse, reinterpret_cast<void*>(&img));


  ///EE 绘制文本
  cv::putText(img,
              "this is an apple",
              cv::Point(40,200),
              cv::FONT_HERSHEY_PLAIN,
              2.0,
              255,
              3,8 );
  cv::imshow("win2", img);

  ///EE 保存图片
  //cv::imwrite("out.png", img);


  cv::waitKey(0);
  return 0;
}
