#include "ReadFrame.hpp"

using namespace cv;

ReadFrame::ReadFrame(CameraState &cam)
: cam(cam)
{}

bool ReadFrame::operator()()
{
    // Capture frame-by-frame
    cam.video >> cam.frame;  
    // If the frame is empty, break immediately
    if (cam.frame.empty())
    {
        return false;
    }
    
    return true;
}