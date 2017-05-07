// 直方图
// 
#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

class Histogram1D {
 private:
  int histSize[1];
  float hranges[2];
  const float* ranges[1];
  int channels[1];
  cv::Mat hist;

 public:
  Histogram1D() {
    // Prepare default arguments for 1D histogram
    histSize[0]= 256; // 256 bins
    hranges[0]= 0.0; // from 0 (inclusive)
    hranges[1]= 256.0; // to 256 (exclusive)
    ranges[0]= hranges;
    channels[0]= 0; // we look at channel 0
    cv::Mat hist;
  }

  cv::Mat getHistogram(const cv::Mat &img) {
    cv::calcHist(&img,
                 1,
                 channels,
                 cv::Mat(),
                 hist,
                 1,
                 histSize,
                 ranges);
    return hist;
  }

  cv::Mat getHistogramImg(int zoom=1) const {
    double maxVal = 0;
    double minVal = 0;
    cv::minMaxLoc(hist, &minVal, &maxVal, 0,0);

    int histSize = hist.rows;

    cv::Mat img(histSize * zoom,
                histSize*zoom,
                CV_8U,
                cv::Scalar(255));
    int hpt = static_cast<int>(0.9*histSize);

    for (int h=0; h < histSize; ++h) {
      float binVal = hist.at<float>(h);
      if (binVal > 0) {
        int intensity = static_cast<int>(binVal*hpt/maxVal);
        cv::line(img, cv::Point(h*zoom, histSize*zoom),
                 cv::Point(h*zoom, (histSize-intensity)*zoom),
                 cv::Scalar(0),
                 zoom);
      }
    }
    return img;
  }


  static cv::Mat applyLookUp(const cv::Mat &img, const cv::Mat &lookup) {
    cv::Mat result;
    cv::LUT(img, lookup, result);
    return result;
  }

  cv::Mat stretch(const cv::Mat &image, int minValue=0) {
    // Compute histogram first
    cv::Mat hist= getHistogram(image); // find left extremity of the histogram
    int imin= 0; //最左边右值的第一个下标
    for( ; imin < histSize[0]; imin++ ) {
      // ignore bins with less than minValue entries
      if (hist.at<float>(imin) > minValue)
        break;
    }
    // find right extremity of the histogram
    int imax= histSize[0]-1;
    for( ; imax >= 0; imax-- ) {
      // ignore bins with less than minValue entries
      if (hist.at<float>(imax) > minValue)
        break;
    }
    // Create lookup table
    int dim(256);
    cv::Mat lookup(1, // 1 dimension
                   &dim,
                   // 256 entries
                   CV_8U);
    // uchar
    // Build lookup table
    for (int i=0; i<256; i++) {
      // stretch between imin and imax
      if (i < imin)
        lookup.at<uchar>(i)= 0;
      else if (i > imax) 
        lookup.at<uchar>(i)= 255;
      else 
        lookup.at<uchar>(i)= cvRound(255.0*(i-imin)/(imax-imin));
    }
    // Apply lookup table
    cv::Mat result;
    result= applyLookUp(image,lookup);
    printf("ileft=%d iright=%d\n", imin, imax);
    return result;
  }

};


using std::cout;
int main(int argc, char *argv[])
{
  //CV_8U 灰度图片
  //CV_BUC3 三通道
  //cv::Mat img(240,320, CV_8UC3, cv::Scalar(0,0,255)); //3 color channel
  //cv::Mat img(cv::Size(240,320), CV_8UC3, cv::Scalar(0,0,255)); //3 color channel
  //cv::imshow("img1", img);

  cv::Mat img = cv::imread("gray.png",0);

  Histogram1D h;
  cv::Mat histo = h.getHistogram(img);
  for (int i=0; i<256; ++i)
    printf("Value %d = %f\n", i, histo.at<float>(i));

  cv::namedWindow("histogram");
  cv::imshow("histogram", h.getHistogramImg());

  ///EE阀值函数,显示二元颜色
  //cv::Mat thresholded;
  //cv::threshold(img, thresholded,150,255,cv::THRESH_BINARY);
  //cv::namedWindow("threshold");
  //cv::imshow("threshold", thresholded);

  cv::Mat streteched = h.stretch(img, 100.01f);
  cv::imshow("stretech", streteched);

  cv::namedWindow("histogram2");
  h.getHistogram(streteched);
  cv::imshow("histogram2", h.getHistogramImg());

  ///直方图均衡化
  cv::Mat equ;
  cv::equalizeHist(img, equ);
  cv::imshow("equalize", equ);
  h.getHistogram(equ);
  cv::imshow("equalize2", h.getHistogramImg());
  
  cv::waitKey(0);
  return 0;
}

