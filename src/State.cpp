#include "State.hpp"

using namespace cv;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t framesProcessed = PTHREAD_COND_INITIALIZER;
pthread_cond_t all_frames_done = PTHREAD_COND_INITIALIZER;

// # number of frames to skip
#define SKIPNUM 2
#define SHOW_SKIPNUM 3
// # skip frames for BGSub algoritm and imshow
#define BGSUB_SKIP_ENABLE false
// # skip frames for imshow only
#define SHOW_SKIP_ENABLE false

int cameraCount;

template <
    class result_t   = std::chrono::milliseconds,
    class clock_t    = std::chrono::steady_clock,
    class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

void *processFrame(void *arg)
{
    Camera *cam = static_cast<Camera*>(arg);

    auto skip = 0;

    const auto total_time = std::chrono::steady_clock::now();
    

    while (cam->isOpened()) // and frame_count < 300
    {
        if (!cam->readFrame()) break;

        // chech for skip frames
        if (BGSUB_SKIP_ENABLE)
        {
            if (skip < SKIPNUM)
            {
                skip += 1;
                continue;
            }
            else
                skip = 0;
        }
        

        if (!cam->findMotion()) break;

        pthread_mutex_lock(&mutex);
        cam->frameProcessed = true;

        while (cam->frameProcessed)
            pthread_cond_wait(&all_frames_done, &mutex);
        pthread_mutex_unlock(&mutex);
        
    }

    cam->isVideoEnded = true;
    cam->frameProcessed = true;

    std::cout << "Total time = " << since(total_time).count() << std::endl;

    return nullptr;
}


State::State()
{
    
}

bool State::initCameras(std::string num)
{
    std::string videoName(std::string("case") + num);
    auto video_left = VideoCapture("materials/" + videoName + "/left.avi");
    if  (!video_left.isOpened())
    {
        std::cout << "Error File Not Found" << std::endl;
        return false;
    }
    auto video_right = VideoCapture("materials/" + videoName + "/right.avi");
    if  (!video_right.isOpened())
    {
        std::cout << "Error File Not Found" << std::endl;
        return false;
    }
    cameras.resize(2);
    cameras[0] = new Camera();
    cameras[1] = new Camera();

    cameras[0]->setVideo(video_left);
    cameras[1]->setVideo(video_right);

    cameras[0]->setName("left");
    cameras[1]->setName("right");

    // case 1
    // cameras[0]->findMotion.aircraft_window_to_check = cv::Rect2i(40, 138, 100, 60);
    // cameras[1]->findMotion.aircraft_window_to_check = cv::Rect2i(356, 138, 100, 60);
    // case 2
    // cameras[0]->findMotion.aircraft_window_to_check = cv::Rect2i(0, 20, 100, 100);
    // cameras[1]->findMotion.aircraft_window_to_check = cv::Rect2i(880, 20, 100, 100);

    // case 3
    cameras[0]->findMotion.aircraft_window_to_check = cv::Rect2i(380, 120, 100, 100);
    cameras[1]->findMotion.aircraft_window_to_check = cv::Rect2i(356, 118, 100, 100);

    return true;
}

void State::process()
{
    cameraCount = cameras.size();
    std::vector<pthread_t> threads(cameraCount);

    std::cout << "Hi" << std::endl;

    for (int i = 0; i < 2; i++)
    {
        printf("Creating thread #%d\n", i);
        fflush(stdout);

        pthread_create(&threads[i], nullptr, processFrame, (void*)(cameras[i]));
    }

    pthread_detach(threads[0]);
    // pthread_detach(threads[1]);
    Mat E = (Mat_<float>(3,3) << 0, 40, 0, 39.5, 0, 6.96, 0, 0, 0);
    Mat rot = (Mat_<float>(3,3) << 0.9396926,  0.0000000, -0.3420202, 
                                   0.0000000,  1.0000000,  0.0000000, 
                                   0.3420202,  0.0000000,  0.9396926);
    std::vector<cv::Point3f> lines;
    std::vector<Point2f> points1;
    points1.push_back({0, 0});


    do
    {
        if (cameras[0]->isVideoEnded && cameras[1]->isVideoEnded) break;
        if (!(cameras[0]->frameProcessed || cameras[1]->frameProcessed)) continue;
        
        pthread_mutex_lock(&mutex);

        // put there logic to find object coordinates in real world 


        for (auto &&cam : cameras)
        {
            cam->frameProcessed = false;
        }
        
        pthread_cond_broadcast(&all_frames_done);
        pthread_mutex_unlock(&mutex);
        
    } while (true);
    
    std::cout << "Main thread DONE!" << std::endl;

    std::cout << cameras[0]->state.last_locations.size() << std::endl;
}

State::~State()
{
    for (auto &camera : cameras)
    {
        delete camera;
    }
    
    destroyAllWindows();
}