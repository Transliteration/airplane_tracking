#include "FindMotion.hpp"

#include <opencv2/core.hpp>

using namespace cv;

Mat _dilate(InputArray input)
{
    Mat dilation_dst;
    // case 1
    // auto dilation_size = 3;
    
    // case 2
    auto dilation_size = 11;
    Mat element = getStructuringElement( MORPH_ELLIPSE,
                // case 2 3
                // Size( 2*dilation_size + 5, 2*dilation_size+1 ),
                // case 1
                Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                Point( -1, -1 ) );
    dilate( input, dilation_dst, element );
    return dilation_dst;
}

FindMotion::FindMotion(CameraState &cam)
: cam(cam)
, objectFound(false)
{}

bool FindMotion::operator()()
{
    resize(cam.frame, cam.frame, cam.frame_size/2, 0, 0, cv::INTER_AREA);
    cv::cvtColor(cam.frame, cam.frame_gray, COLOR_BGR2GRAY);
    medianBlur(cam.frame_gray, cam.blurred_frame, 3);

    cam.back_sub->apply(cam.frame_gray, cam.fg_mask,0.2);

    cam.dilation_dst = _dilate(cam.fg_mask);

    // find contours
    std::vector<std::vector<cv::Point> > contours;
    cam.contourOutput = cam.fg_mask.clone();
    findContours(cam.dilation_dst, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    //Draw the contours
    cam.contourImage = Mat(cam.fg_mask.size(), CV_8UC3, cv::Scalar(0,0,0));

    for (size_t idx = 0; idx < contours.size(); idx++) {
        auto boundingRect = minAreaRect(contours[idx]).boundingRect();

        rectangle(cam.fg_mask, boundingRect, Scalar(0,0,255), 2);

        // case 1 40 10000
        // case 2 400 400'000
        // case 3 400 100'000
        if (double area = boundingRect.area(); area < 400 || area > 100'000)
        {
            std::cout << area << std::endl;
            std::cout << "skip" << std::endl;
            continue;
        }

        // draw rectangle
        cam.pos_aircraft.x = boundingRect.x + boundingRect.width/2;
        cam.pos_aircraft.y = boundingRect.y + boundingRect.height/2;

        drawContours(cam.frame, contours, idx, {127, 127, 127});

        if (aircraft_window_to_check.contains(cam.pos_aircraft))
        {
            if (objectFound)
            {
                auto point = cam.last_locations.back();
                double coef = 2;
                
                auto new_window = Rect(point.x-boundingRect.width, point.y-boundingRect.height, boundingRect.width*coef, boundingRect.height*coef);
                auto size_of_object_inside_search_window = (double)aircraft_window_to_check.area()/new_window.area();
                if (size_of_object_inside_search_window > 0.5 && size_of_object_inside_search_window < coef)
                {
                    auto dist_between_last_and_new_loaction = cam.pos_aircraft;
                    dist_between_last_and_new_loaction.x -= (new_window.x +  new_window.width/2);
                    dist_between_last_and_new_loaction.y -= (new_window.y +  new_window.height/2);
                    auto delta = std::sqrt(dist_between_last_and_new_loaction.x*dist_between_last_and_new_loaction.x) + 
                                 std::sqrt(dist_between_last_and_new_loaction.y*dist_between_last_and_new_loaction.y);
                    if (delta/new_window.area()  < 15)
                    {
                        aircraft_window_to_check = new_window;
                        rectangle(cam.frame, boundingRect, Scalar(255,255,0), 1);
                        cam.last_locations.push(cam.pos_aircraft);
                        std::cout << cam.pos_aircraft << std::endl;
                    }
                }
                else
                {
                    rectangle(cam.frame, boundingRect, Scalar(0,0,255), 1);
                }
            }
            else
            {
                objectFound = true;
                cam.last_locations.push(cam.pos_aircraft);

            }
        }
        else
        {
            rectangle(cam.frame, boundingRect, Scalar(0,0,255), 1);
            
        }
        
    }

    rectangle(cam.frame, aircraft_window_to_check, Scalar(0,255,0), 2);

    if (objectFound)
    {
        // put there logic for getting vector pointing to found object from camera perspective (in real world coordinates)

    }


    imshow( cam.name, cam.frame );
    char c=(char)waitKey(1);
    if(c==27)
        return false;
    
    return true;
}