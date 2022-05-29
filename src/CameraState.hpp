#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>


class CameraState
{
public:
    cv::VideoCapture video;
    std::string name;
    bool isStopped;

    int frame_width;
    int frame_height;
    cv::Size frame_size;

    cv::Point2i pos_aircraft;
    std::queue<cv::Point2i> last_locations;


    cv::Mat frame, frame_gray, blurred_frame, fg_mask, canny_output, dilation_dst;
    cv::Mat contourOutput;
    cv::Mat contourImage;

    cv::Ptr<cv::BackgroundSubtractorKNN> back_sub = cv::createBackgroundSubtractorKNN(60, 100.0, false);

    cv::Vec3d vectorToObj;

    void initVideoAndSize(cv::VideoCapture &video);
    void printVideoInfo();
};