#pragma once

#include "Camera.hpp"

class State
{
public:
    std::vector<Camera*> cameras;
    State();

    bool initCameras(std::string num = "1");

    void process();
    ~State();
};