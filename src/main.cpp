#include <iostream>

#include "State.hpp"

#include <X11/Xlib.h>

using namespace cv;


int main()
{
    std::cout << "Start main" << std::endl;
    if (XInitThreads() == 0) 
    {
        std::cerr << "imshow in multiple threads wont work" << std::endl;
        return -1;
    }
    std::cout << "XInitThreads done" << std::endl;
    State state;
    std::cout << "State created" << std::endl;
    state.initCameras("3");
    std::cout << "Cameras are initialized" << std::endl;
    state.process();

    return 0;
}