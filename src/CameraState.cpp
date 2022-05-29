#include "CameraState.hpp"

using namespace cv;

void CameraState::initVideoAndSize(VideoCapture &video)
{
    this->video = std::move(video);
    frame_width = int(video.get(3));
    frame_height = int(video.get(4));
    frame_size = Size(frame_width, frame_height);
}

void CameraState::printVideoInfo()
{
    auto frame_width = int(video.get(3));
    auto frame_height = int(video.get(4));
    auto fps = video.get(cv::CAP_PROP_FPS);

    std::cout << "Total frames " << video.get(cv::CAP_PROP_FRAME_COUNT) << std::endl;
    std::cout << "Total time " << video.get(cv::CAP_PROP_FRAME_COUNT)/fps << std::endl;
    std::cout << "Frame width = "<< frame_width << std::endl;
    std::cout << "Frame height = "<< frame_height << std::endl;
    std::cout << "FPS = " << fps << std::endl;
}