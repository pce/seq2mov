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

int extractNumber(const fs::path& path) {
    std::string filename = path.filename().string();
    size_t start = filename.find_first_of("0123456789");
    size_t end = filename.find_last_of("0123456789");

    if (start == std::string::npos || end == std::string::npos)
        return 0; // No numeric part found in the filename

    return std::stoi(filename.substr(start, end - start + 1));
}

int main(int argc, char* argv[])
{

    string pathArg = "data";

    vector<Mat> images;
    vector<fs::path> files;

   if (argc > 1) {
        pathArg = argv[1]; 
    }

    for (const auto &entry : fs::directory_iterator(pathArg))
    {
        // std::cout << entry.path() << std::endl;
        if (entry.path().filename().string()[0] == '.')
        {
            continue;
        }
        files.push_back(entry.path());
        // if (files.size() >= 3600) {
        //     break;
        // }
    }


    std::sort(files.begin(), files.end(), [](const auto& lhs, const auto& rhs) {
        int lhsNumber = extractNumber(lhs);
        int rhsNumber = extractNumber(rhs);
        if (lhsNumber != rhsNumber) {
            return lhsNumber < rhsNumber;
        } else {
            return lhs.filename().string() < rhs.filename().string();
        }
    });

    for (auto imageFilename : files)
    {
        std::cout << imageFilename << std::endl;
        string name = imageFilename.string();
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
