#include "Camera.hpp"

Camera::Camera()
: frameProcessed(false)
, isVideoEnded(false)
, readFrame(state)
, findMotion(state)
{
    
}

bool Camera::isOpened() const
{
    return state.video.isOpened();
}

void Camera::setVideo(cv::VideoCapture &video)
{
    state.initVideoAndSize(video);
    state.printVideoInfo();
}

Camera::~Camera()
{
    state.video.release();
}

void Camera::setName(std::string &&name)
{
    state.name = std::move(name);
}