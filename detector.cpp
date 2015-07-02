#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include "opencv2/imgproc/imgproc.hpp"

// Global variables used in canny edge detection
int lowThreshold = 10;
int kernelSize = 3;
int ratio = 3;

// Robots mask
///////////////////// Size and values pof the mask /////////////////////
int maskSize = 3;
cv::Mat robotMask = cv::Mat::ones(maskSize, maskSize, CV_8UC1);

/*
  Apply the canny edge detector in the gray scale imnage
 */
void applyCanny(const cv::Mat& inImage, cv::Mat* outImage)
{
  // Reduse the noise in the input image
  cv::blur(inImage, *outImage, cv::Size(3, 3));
  //cv::imshow("Blured image", *outImage);

  // Apply Canny detector
  cv::Canny(*outImage, *outImage,
    lowThreshold, lowThreshold * ratio, kernelSize);

  //cv::imshow("Canny image", *outImage);
}

int main(int argc, char** argv)
{
  // The input image
  cv::Mat inputImage;
  inputImage = cv::imread("obstacle.png");

  if (! inputImage.data)
  {
    std::cout << ("No image data");
    return -1;
  }

  // Show the input image
  cv::imshow("InputImage", inputImage);

  // Image to gray scale
  cv::Mat grayImage;
  cvtColor(inputImage, grayImage, CV_BGR2GRAY);

  // Find the gradient differences aka Dangerous areas
  cv::Mat edgesImage;

  applyCanny(grayImage, &edgesImage);

  // All edges will be represented by value of 255
  cv::threshold(edgesImage, edgesImage, 0, 255, cv::THRESH_BINARY);
  cv::imshow("Edges after thresholding", edgesImage);

  ///////// Fill the edges map with the unkown values from inputImage //////////

  // Pass the robot through all the map and make the new map
  cv::Mat newMap;
  cv::filter2D(edgesImage, newMap, -1, robotMask,
    cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
  cv::imshow("The new map", newMap);

  //////// If values in map < 0 set as -1
  //////// Else if > 0 threshold to 100
  //////// If 0 as it is

  cv::waitKey();
  return 0;
}
