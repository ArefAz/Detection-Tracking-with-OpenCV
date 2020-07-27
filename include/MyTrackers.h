//
// Created by aref on 7/24/20.
//

#ifndef TRACKING_MYTRACKERS_H
#define TRACKING_MYTRACKERS_H

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include "MyTracker.h"

using namespace std;
using namespace cv;

class MyTrackers {
public:
    MyTrackers(const std::vector<Rect> &ROIs, const std::string &trackerAlgorithm, const std::string &windowName);

    void addTracker(const cv::Mat &frame);

    std::vector<cv::Rect> update(const cv::Mat &frame);

    void setTrackers(const cv::Mat &frame, std::vector<cv::Rect> &boundingBoxes);

    inline static int nextTrackerId = 0;

private:
    cv::Ptr<cv::Tracker> createTrackerByName();

    std::vector<MyTracker> trackersVector;
    std::vector<cv::Rect> ROIs;
    std::string trackerAlgorithm;
    std::string windowName;

    void clear();
};


#endif //TRACKING_MYTRACKERS_H
