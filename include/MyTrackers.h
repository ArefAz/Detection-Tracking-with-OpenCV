//
// Created by aref on 7/24/20.
//

#ifndef TRACKING_MYTRACKERS_H
#define TRACKING_MYTRACKERS_H

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <chrono>
#include "MyTracker.h"

using namespace std;
using namespace cv;

class MyTrackers {
public:
    MyTrackers(const std::vector<Rect> &ROIs, const std::string &trackerAlgorithm, const std::string &windowName,
            bool drawIds, float iouThreshold);

    void addTracker(const cv::Mat &frame);

    std::vector<cv::Rect> update(const cv::Mat &frame);

    void setTrackers(const cv::Mat &frame, std::vector<cv::Rect> &boundingBoxes);

    inline static int nextTrackerId = 0;

private:
    bool drawIds;
    int resetInterval;
    float iouThreshold;
    std::vector<MyTracker> trackersVector;
    std::vector<MyTracker> prevTrackersVector;
    std::vector<cv::Rect> ROIs;
    std::vector<cv::Rect> prevROIs;
    std::string trackerAlgorithm;
    std::string windowName;
    std::chrono::_V2::system_clock::time_point startTime;

    void clear();

    bool intervalReached();

    static bool compareROIs(const Rect &ROI_1, const Rect &ROI_2);

    static double IOU(const Rect &ROI_1, const Rect &ROI_2);
};


#endif //TRACKING_MYTRACKERS_H
