#include <iostream>
#include <thread>
#include <vector>
#include <iostream>
#include <filesystem>

#include <opencv2/core/core.hpp>       // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp> // Video write

using namespace std;
using namespace cv;

namespace fs = std::filesystem;

int main()
{
    vector<Mat> images;

    vector<fs::path> files;

    for (const auto &entry : fs::directory_iterator("data"))
    {
        // std::cout << entry.path() << std::endl;
        if (entry.path().filename().string()[0] == '.')
        {
            continue;
        }
        files.push_back(entry.path());
    }

    std::sort(
        files.begin(), files.end(), [](const auto &lhs, const auto &rhs)
        { return (lhs.filename().string() <
                  rhs.filename().string()); });

    for (auto imageFilename : files)
    {
        // std::cout << imageFilename << std::endl;
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
