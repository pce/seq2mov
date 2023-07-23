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

int extractNumber(const fs::path &path)
{
    std::string filename = path.filename().string();
    size_t start = filename.find_first_of("0123456789");
    size_t end = filename.find_last_of("0123456789");

    if (start == std::string::npos || end == std::string::npos)
        return 0; // No numeric part found in the filename

    return std::stoi(filename.substr(start, end - start + 1));
}

void printVerbose(const string &message, bool verbose)
{
    if (verbose)
    {
        cout << message << endl;
    }
}

int main(int argc, char *argv[])
{

    string pathArg = "data";
    string videoFilename = "out.avi";
    bool verbose = false;
    int batchSize = 100;

    vector<Mat> images;
    vector<fs::path> files;

    // command-line arguments
    unordered_map<string_view, string_view> arguments;

    for (int i = 1; i < argc; ++i)
    {
        string_view arg = argv[i];
        if (arg.starts_with("-"))
        {
            if (++i < argc)
            {
                arguments[arg] = argv[i];
            }
        }
    }

    // Check for specific arguments
    if (arguments.count("-v"))
    {
        verbose = true;
    }

    if (arguments.count("-b"))
    {
        batchSize = stoi(arguments["-b"].data());
    }

    if (arguments.count("-o"))
    {
        videoFilename = arguments["-o"].data();
    }

    if (argc > 1)
    {
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

    std::sort(files.begin(), files.end(), [](const auto &lhs, const auto &rhs)
              {
        int lhsNumber = extractNumber(lhs);
        int rhsNumber = extractNumber(rhs);
        if (lhsNumber != rhsNumber) {
            return lhsNumber < rhsNumber;
        } else {
            return lhs.filename().string() < rhs.filename().string();
        } });

    for (auto imageFilename : files)
    {
        string name = imageFilename.string();
        Mat img = imread(name);
        if (img.empty())
        {
            cerr << "file " << name << " not loaded" << endl;
            continue;
        }
        images.push_back(img);
        printVerbose("Loaded image: " + name, verbose);
    }

    Size targetSize = Size(images[0].cols, images[0].rows);
    int targetFps = 30;

    // VideoWriter outputVideo;
    // outputVideo.open(videoFilename, -1, targetFps, S, true);
    VideoWriter outputVideo(videoFilename, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), targetFps, targetSize);

    if (!outputVideo.isOpened())
    {
        cerr << "Could not open the output video for write: " << endl;
        return -1;
    }

    // write frames in batches
    for (int i = 0; i < images.size(); i += batchSize)
    {
        int endIdx = min(i + batchSize, static_cast<int>(images.size()));
        for (int j = i; j < endIdx; j++)
        {
            outputVideo << images[j];
        }
    }

    printVerbose("Finished writing", verbose);
    return 0;
}
