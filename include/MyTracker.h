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
    MyTracker();

    MyTracker(const cv::Mat &frame, const Rect2d &bbox, int trackerId, const std::string &trackerAlgorithm = "CSRT",
              bool doDrawId = false);

    void setTracker(const cv::Mat &frame, const Rect2d &bbox, int trackerId,
                    const std::string &trackerAlgorithm = "CSRT", bool drawId = false);

    void drawId(const cv::Mat &frame) const;

    bool update(const cv::Mat &frame);

    cv::Rect getROI();

    int getId() const;

private:

    cv::Ptr<cv::Tracker> tracker;
    cv::Rect2d ROI;
    int id{};
    bool doDrawId{};

};


#endif //TRACKING_MYTRACKER_H
