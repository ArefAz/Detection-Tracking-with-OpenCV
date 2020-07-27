//
// Created by aref on 7/25/20.
//

#include "MyTracker.h"

MyTracker::MyTracker(const cv::Mat &frame, const Rect2d &bbox, int trackerId, const std::string &trackerAlgorithm) {


    string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};

    // Check the tracker algorithm validity
    string *trackerType = std::find(std::begin(trackerTypes), std::end(trackerTypes), trackerAlgorithm);
    if (trackerType == std::end(trackerTypes)) {
        cout << "Please use a valid tracker algorithm: "
             << R"({"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"})" << endl;
        cout << "Using default tracker algorithm (CSRT)..." << endl;
        this->tracker = TrackerCSRT::create();

    } else {
        if (trackerAlgorithm == "BOOSTING")
            this->tracker = TrackerBoosting::create();
        if (trackerAlgorithm == "MIL")
            this->tracker = TrackerMIL::create();
        if (trackerAlgorithm == "KCF")
            this->tracker = TrackerKCF::create();
        if (trackerAlgorithm == "TLD")
            this->tracker = TrackerTLD::create();
        if (trackerAlgorithm == "MEDIANFLOW")
            this->tracker = TrackerMedianFlow::create();
        if (trackerAlgorithm == "GOTURN")
            this->tracker = TrackerGOTURN::create();
        if (trackerAlgorithm == "MOSSE")
            this->tracker = TrackerMOSSE::create();
        if (trackerAlgorithm == "CSRT")
            this->tracker = TrackerCSRT::create();
    }

    this->id = trackerId;
    this->ROI = bbox;
    this->tracker->init(frame, this->ROI);

}

bool MyTracker::update(const cv::Mat &frame) {
    return this->tracker->update(frame, this->ROI);
}

cv::Rect MyTracker::getROI() {
    return this->ROI;
}



