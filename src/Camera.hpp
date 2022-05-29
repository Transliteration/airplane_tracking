#pragma once

#include "CameraState.hpp"
#include "ReadFrame.hpp"
#include "FindMotion.hpp"


class Camera
{
public:
    CameraState state;
    bool frameProcessed, isVideoEnded;
    ReadFrame readFrame;
    FindMotion findMotion;

    Camera();

    bool isOpened() const;

    void setVideo(cv::VideoCapture &video);

    ~Camera();

    void setName(std::string &&name);

};