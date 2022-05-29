#pragma once

#include "CameraState.hpp"

#include <opencv2/core.hpp>

class ReadFrame
{
    CameraState &cam;
public:
    ReadFrame(CameraState &cam);

    bool operator()();
};