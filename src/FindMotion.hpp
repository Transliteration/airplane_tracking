#pragma once

#include "CameraState.hpp"

class FindMotion
{
    CameraState &cam;
    bool objectFound;
    
public:
    cv::Rect2i aircraft_window_to_check;
    FindMotion(CameraState &cam);

    bool operator()();
};
