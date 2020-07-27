//
// Created by aref on 7/24/20.
//

#include "MyTrackers.h"


MyTrackers::MyTrackers(const std::vector<Rect> &ROIs, const std::string &trackerAlgorithm, const string &windowName,
                       bool drawIds, float iouThreshold) {

    this->windowName = windowName;
    this->trackerAlgorithm = trackerAlgorithm;
    this->prevROIs = this->ROIs = ROIs;
    this->drawIds = drawIds;
    this->iouThreshold = iouThreshold;
    this->startTime = std::chrono::system_clock::now();

    // Trackers object will lose its memory of object ids after X seconds
    // This should be implemented dynamically (using hash tables etc.)
    // to make lost tracked-object's ids available to new objects
    // However this will do the job for now
    this->resetInterval = 120;

}


std::vector<cv::Rect> MyTrackers::update(const cv::Mat &frame) {

    ROIs.clear();
    for (auto &tracker : this->trackersVector) {
        tracker.update(frame);
        ROIs.push_back(tracker.getROI());
    }
    return this->ROIs;

}


void MyTrackers::addTracker(const Mat &frame) {

    Rect2d box = selectROI(this->windowName, frame);
    MyTracker newTracker(frame, box, MyTrackers::nextTrackerId);
    MyTrackers::nextTrackerId++;
    trackersVector.push_back(newTracker);
}


void MyTrackers::clear() {

    trackersVector.clear();
    ROIs.clear();

    // If we have reached the time limit, reset the IDS
    if (this->intervalReached()) {
        MyTrackers::nextTrackerId = 0;
    }

}

bool MyTrackers::intervalReached(){

    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = now - this->startTime;
    if (elapsed_seconds.count() >= this->resetInterval) {
        this->startTime = std::chrono::system_clock::now();
        cout << "Time interval reached, resetting the ids..." << endl;
        cout << elapsed_seconds.count() << " " << std::chrono::system_clock::to_time_t(this->startTime) << endl << endl;
        return true;
    }
    else
        return false;

}


// This function will be called after each detection to match the ROIs and assign new IDs to the new objects
void MyTrackers::setTrackers(const Mat &frame, vector<cv::Rect> &boundingBoxes) {

    this->prevROIs = ROIs;
    std::vector<MyTracker> tVector(this->trackersVector);
    this->prevTrackersVector = tVector;
    this->clear();

    // Sort this->PrevROIs (ROIs in previous frame) w.r.t. compareROI function
    std::sort(this->prevROIs.begin(), this->prevROIs.end(), &MyTrackers::compareROIs);

    // Matching new boxes with previous ones to maintain the IDs
    for (const auto &newBox : boundingBoxes) {

        bool keepTracker = false;
        float minDist = 5000; // Some large value
        double iou;
        Rect closestOldBox;
        size_t minDistIdx;

        // For each new ROI find the nearest ROI in previous ROIs
        for (size_t oldBoxId = 0; oldBoxId < this->prevROIs.size(); ++oldBoxId) {

            auto oldBox = this->prevROIs[oldBoxId];
            auto l2Dist = float(sqrt(pow(newBox.x - oldBox.x, 2) + pow(newBox.y - oldBox.y, 2)));
            if (l2Dist < minDist) {
                minDist = l2Dist;
                minDistIdx = oldBoxId;
            }
        }


        closestOldBox = this->prevROIs[minDistIdx];
        MyTracker closestTracker;
        // Find the corresponding tracker object whose ROI is the closest to the new ROI
        for (auto &tracker : this->prevTrackersVector) {
            if (closestOldBox == tracker.getROI()) {
                keepTracker = true;
                closestTracker = tracker;
                break;
            }
        }

        // Calculate Union over Intersection of two boxes to make sure that they are actually the same object
        iou = IOU(newBox, closestOldBox);

        // If a match found in previous frame then maintain its ID, else assign a new ID to the object
        // Then add the new tracker object to the trackers vector
        MyTracker newTracker;
        if (keepTracker && iou >= this->iouThreshold) {
            newTracker.setTracker(frame, newBox, closestTracker.getId(), this->trackerAlgorithm, this->drawIds);
        } else {
            newTracker.setTracker(frame, newBox, MyTrackers::nextTrackerId, this->trackerAlgorithm, this->drawIds);
        }
        MyTrackers::nextTrackerId++;
        trackersVector.emplace_back(newTracker);


    }

}

double MyTrackers::IOU(const cv::Rect &ROI_1, const cv::Rect &ROI_2) {

    int xA, xB, yA, yB;
    double iou, intersectArea, uniArea;

    xA = max(ROI_1.x, ROI_2.x);
    yA = max(ROI_1.y, ROI_2.y);
    xB = min(ROI_1.x + ROI_1.width, ROI_2.x + ROI_2.width);
    yB = min(ROI_1.y + ROI_1.height, ROI_2.y + ROI_2.height);

    intersectArea = max(0, xB - xA + 1) * max(0, yB - yA + 1);
    uniArea = ROI_1.area() + ROI_2.area() - intersectArea;

    iou = intersectArea / uniArea;

    return iou;

}

//  Compares two given ROI w.r.t. l2-distance to Point(0, 0)
bool MyTrackers::compareROIs(const cv::Rect &ROI_1, const cv::Rect &ROI_2) {

    double d1, d2;
    d1 = pow(ROI_1.x, 2) + pow(ROI_1.y, 2);
    d2 = pow(ROI_2.x, 2) + pow(ROI_2.y, 2);

    return d2 > d1;

}

