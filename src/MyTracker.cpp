//
// Created by aref on 7/25/20.
//

#include "MyTracker.h"

MyTracker::MyTracker() = default;

MyTracker::MyTracker(const cv::Mat &frame, const Rect2d &bbox, int trackerId, const std::string &trackerAlgorithm,
                     const bool doDrawId) {

    this->setTracker(frame, bbox, trackerId, trackerAlgorithm, doDrawId);

}

// Initialize the tracker
void MyTracker::setTracker(const Mat &frame, const Rect2d &bbox, int trackerId, const string &trackerAlgorithm,
                           const bool drawId) {
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

    this->doDrawId = drawId;
    this->id = trackerId;
    this->ROI = bbox;
    this->tracker->init(frame, this->ROI);
}

bool MyTracker::update(const cv::Mat &frame) {
    if (this->doDrawId)
        this->drawId(frame);
    return this->tracker->update(frame, this->ROI);
}

// Returns the tracker ROI
cv::Rect MyTracker::getROI() {
    return Rect(this->ROI);
}

// Returns the tracker ID
int MyTracker::getId() const {
    return this->id;
}

// Shows the ID of the object above it in the given frame
void MyTracker::drawId(const Mat &frame) const {
    putText(frame, to_string(this->id), Point(ROI.x - 10, ROI.y - 10), FONT_HERSHEY_SIMPLEX, 0.65, Scalar(50, 255, 50), 3);
}



