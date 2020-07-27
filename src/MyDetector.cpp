//
// Created by aref on 7/25/20.
//

#include "MyDetector.h"

MyDetector::MyDetector(const std::string &model, const std::string &config, const std::string &classesFile,
                       const int inputWidth, int inputHeight, float confThreshold, float nmsThreshold,
                       bool drawOutputs) {

    this->net = cv::dnn::readNetFromDarknet(config, model);
    this->net.setPreferableBackend(DNN_BACKEND_OPENCV);
    this->net.setPreferableTarget(DNN_TARGET_CPU);
    this->inputWidth = inputWidth;
    this->inputHeight = inputHeight;
    this->confThreshold = confThreshold;
    this->nmsThreshold = nmsThreshold;
    this->drawOutputs = drawOutputs;

    // Reads class names and add them in the object
    string line;
    ifstream ifs(classesFile.c_str());
    while (getline(ifs, line))
        this->classes.push_back(line);

}

// Performs the detection process
std::vector<cv::Rect> MyDetector::detect(const cv::Mat &frame) {

    cv::Mat blob;
    cv::Size inputSize(this->inputWidth, this->inputHeight);
    vector<String> outNames = this->net.getUnconnectedOutLayersNames();
    vector<Mat> outputs;

    // Create a blob from the frame
    blobFromImage(frame, blob, 1 / 255.0, inputSize, Scalar(0, 0, 0), true, false);
    // Set the blob to the network
    this->net.setInput(blob);
    // Run the forward pass
    this->net.forward(outputs, getOutputsNames());
    // Process the outputs
    this->postProcess(frame, outputs);

    // Return detected boxes' coordinates
    return this->boxes;

}

// Removes the bounding boxes with low confidence using non-maximum suppression
void MyDetector::postProcess(const Mat &frame, const std::vector<Mat> &outs) {

    float *data;
    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boundingBoxes;

    for (const auto &out : outs) {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        data = (float *) out.data;
        for (int j = 0; j < out.rows; ++j, data += out.cols) {
            Mat scores = out.row(j).colRange(5, out.cols);
            Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            minMaxLoc(scores, nullptr, &confidence, nullptr, &classIdPoint);
            if (confidence > confThreshold) {
                int centerX = (int) (data[0] * frame.cols);
                int centerY = (int) (data[1] * frame.rows);
                int width = (int) (data[2] * frame.cols);
                int height = (int) (data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float) confidence);
                Rect newBox(left, top, width, height);
                boundingBoxes.emplace_back(newBox);
            }
        }
    }

    // Perform non maximum suppression to eliminate redundant overlapping boxes with lower confidences
    vector<int> indices;
    NMSBoxes(boundingBoxes, confidences, confThreshold, nmsThreshold, indices);
    for (int idx : indices) {
        Rect box = boundingBoxes[idx];
        if (this->drawOutputs)
            drawPredictions(classIds[idx], confidences[idx], box.x, box.y, box.x + box.width, box.y + box.height,
                            frame);
    }

    this->boxes = boundingBoxes;
}


// Draw the predicted bounding box
void MyDetector::drawPredictions(int classId, float conf, int left, int top, int right, int bottom, const Mat &frame) {

    //Draw a rectangle displaying the bounding box
    rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 178, 50), 3);

    //Get the label for the class name and its confidence
    string label = format("%.2f", conf);
    if (!this->classes.empty()) {
        CV_Assert(classId < (int) this->classes.size());
        label = this->classes[classId] + ":" + label;
    }

    //Display the label at the top of the bounding box
    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = max(top, labelSize.height);
    rectangle(frame, Point(left, top - round(1.5 * labelSize.height)),
              Point(left + round(1.5 * labelSize.width), top + baseLine), Scalar(255, 255, 255), FILLED);
    putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 1);
}

// Get the names of the network's output layers
vector<String> MyDetector::getOutputsNames() {

    vector<String> names;
    if (names.empty()) {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        vector<int> outLayers = this->net.getUnconnectedOutLayers();

        //get the names of all the layers in the network
        vector<String> layersNames = this->net.getLayerNames();

        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}