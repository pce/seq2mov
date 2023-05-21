#include <iostream>
#include <thread>
#include <vector>

#include <opencv2/core/core.hpp>       // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp> // Video write

using namespace std;
using namespace cv;

int main()
{
    vector<Mat> images;

    string imagesList[2] = {
        "pic_1.png",
        "pic_2.png"
        // , "..."
    };

    for (auto imageFilename : imagesList)
    {
        // string name = "data/pic_" + index + ".png";
        string name = imageFilename;
        Mat img = imread(name);
        if (img.empty())
        {
            cerr << "file " << name << " not loaded" << endl;
            continue;
        }
        images.push_back(img);
    }

    Size targetSize = Size(images[0].cols, images[0].rows);
    int targetFps = 30;
    string videoFilename = "out.avi";

    // VideoWriter outputVideo;
    // outputVideo.open(videoFilename, -1, targetFps, S, true);
    // VideoWriter outputVideo(videoFilename, cv::VideoWriter::fourcc('M','J','P','G'),2,S);
    VideoWriter outputVideo(videoFilename, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), targetFps, targetSize);

    if (!outputVideo.isOpened())
    {
        cout << "Could not open the output video for write: " << endl;
        return -1;
    }

    for (int i = 0; i < images.size(); i++)
    {
        outputVideo << images[i];
    }

    cout << "Finished writing" << endl;
    return 0;
}
