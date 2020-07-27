//
// Created by aref on 7/24/20.
//

#include "MyTrackers.h"


MyTrackers::MyTrackers(const std::vector<Rect> &ROIs, const std::string &trackerAlgorithm, const string &windowName) {

    this->windowName = windowName;
    this->trackerAlgorithm = trackerAlgorithm;
    this->ROIs = ROIs;

}


std::vector<cv::Rect> MyTrackers::update(const cv::Mat &frame) {

    ROIs.clear();
    for (auto &tracker : this->trackersVector) {
        tracker.update(frame);
        ROIs.push_back(tracker.getROI());
    }
    return this->ROIs;

}


cv::Ptr<cv::Tracker> MyTrackers::createTrackerByName() {

    string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};
    cv::Ptr<Tracker> tracker;

    // Check the tracker algorithm validity
    string *trackerType = std::find(std::begin(trackerTypes), std::end(trackerTypes), trackerAlgorithm);
    if (trackerType == std::end(trackerTypes)) {
        cout << "Please use a valid tracker algorithm: "
             << R"({"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"})" << endl;
        cout << "Using default tracker algorithm (CSRT)..." << endl;
        trackerAlgorithm = trackerTypes[7];
        tracker = TrackerCSRT::create();
    } else {
        if (trackerAlgorithm == "BOOSTING")
            tracker = TrackerBoosting::create();
        if (trackerAlgorithm == "MIL")
            tracker = TrackerMIL::create();
        if (trackerAlgorithm == "KCF")
            tracker = TrackerKCF::create();
        if (trackerAlgorithm == "TLD")
            tracker = TrackerTLD::create();
        if (trackerAlgorithm == "MEDIANFLOW")
            tracker = TrackerMedianFlow::create();
        if (trackerAlgorithm == "GOTURN")
            tracker = TrackerGOTURN::create();
        if (trackerAlgorithm == "MOSSE")
            tracker = TrackerMOSSE::create();
        if (trackerAlgorithm == "CSRT")
            tracker = TrackerCSRT::create();
    }

    return tracker;

}


void MyTrackers::addTracker(const Mat &frame) {
    cv::Ptr<cv::Tracker> tracker = createTrackerByName();
    Rect2d box = selectROI(this->windowName, frame);
    MyTracker newTracker(frame, box, MyTrackers::nextTrackerId);
    MyTrackers::nextTrackerId++;
    trackersVector.push_back(newTracker);
}


void MyTrackers::clear() {
    trackersVector.clear();
    ROIs.clear();
    MyTrackers::nextTrackerId = 0;

}


void MyTrackers::setTrackers(const Mat &frame, vector<cv::Rect> &boundingBoxes) {

    this->clear();
    for (auto &box : boundingBoxes) {
        MyTracker newTracker(frame, box, MyTrackers::nextTrackerId, trackerAlgorithm);
        MyTrackers::nextTrackerId++;
        trackersVector.emplace_back(newTracker);
    }

}

