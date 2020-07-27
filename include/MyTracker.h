//
// Created by aref on 7/25/20.
//

#ifndef TRACKING_MYTRACKER_H
#define TRACKING_MYTRACKER_H

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

using namespace cv;
using namespace std;

class MyTracker {
public:
    MyTracker(const cv::Mat &frame, const Rect2d &bbox, int trackerId, const std::string &trackerAlgorithm = "CSRT");

    bool update(const cv::Mat &frame);

    cv::Rect getROI();

private:

    cv::Ptr<cv::Tracker> tracker;
    cv::Rect2d ROI;
    int id;

};


#endif //TRACKING_MYTRACKER_H
