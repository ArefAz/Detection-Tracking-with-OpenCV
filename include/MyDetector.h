//
// Created by aref on 7/25/20.
//

#ifndef TRACKING_MYDETECTOR_H
#define TRACKING_MYDETECTOR_H

#include "opencv2/opencv.hpp"
#include "opencv2/dnn.hpp"

using namespace std;
using namespace cv;
using namespace cv::dnn;

class MyDetector {
public:

    MyDetector(const string &model, const string &config, int inputWidth = 320, int inputHeight = 320,
               float confThreshold = 0.5, float nmsThreshold = 0.4, bool drawOutputs = false);

    std::vector<cv::Rect> detect(const cv::Mat &frame);

private:
    bool drawOutputs;
    int inputWidth, inputHeight;
    float confThreshold, nmsThreshold;
    vector<string> classes;
    vector<Rect> boxes;
    cv::dnn::Net net;

    void drawPredictions(int classId, float conf, int left, int top, int right, int bottom, const Mat &frame);

    void postProcess(const Mat &frame, const vector<Mat> &outs);

    vector<String> getOutputsNames();

};


#endif //TRACKING_MYDETECTOR_H
