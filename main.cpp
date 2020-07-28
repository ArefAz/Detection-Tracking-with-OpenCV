//
// Created by aref on 7/23/20.
//

#include "MyTrackers.h"
#include "MyDetector.h"
#include "cxxopts.hpp"

int main(int argc, char **argv) {

    cout << endl;
    // Adding argument parser
    cxxopts::Options options("./tracking",
                             "This program detects the objects in the video, then assigns them a unique "
                             "ID and will track them while maintaining the ID of the object until it is not observable."
                             "\nYou should set four paths correctly before running the executable "
                             "(model file, config file, video file, and classes file). "
                             "OTHERWISE AN EXCEPTION WILL BE THROWN!\n");
    options.add_options()
            ("v,video", "Video file path (Required)",
             cxxopts::value<string>()->default_value("../.videos/video.mp4"))

            ("m,model", "Detection model path (Only Dark-net is supported) (Required)",
             cxxopts::value<string>()->default_value("../models/yolov3.weights"))

            ("c,config", "Model's config file path (Required)",
             cxxopts::value<string>()->default_value("../models/yolov3.cfg"))

            ("classes", "Path to the file containing the class names for the detector (Required)",
             cxxopts::value<string>()->default_value("../models/coco.name"))

            ("t,tracker",
             R"(Tracking algorithm: {"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"})",
             cxxopts::value<string>()->default_value("KCF"))

            ("f,fullscreen", "Enables fullscreen presentation", cxxopts::value<bool>()->implicit_value("true"))

            ("draw-detects", "Shows the box labels on the frame", cxxopts::value<bool>()->implicit_value("true"))

            ("draw-ids", "Enables ID drawing on frames, to disable use: --draw-ids=false",
             cxxopts::value<bool>()->default_value("true"))

            ("d,detect-step", "Use the detector every X frames", cxxopts::value<int>()->default_value("30"))

            ("i,iou-thresh", "IOU Threshold for matching the boxes", cxxopts::value<float>()->default_value("0.6"))

            ("conf-thresh", "Confidence Threshold for the detector",
             cxxopts::value<float>()->default_value("0.65"))

            ("nms-thresh", "NMS Threshold for the detector", cxxopts::value<float>()->default_value("0.65"))

            ("h,help", "Prints this message");


    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    // Variable declaration and initialization
    bool drawIds = result["draw-ids"].as<bool>(), go = true, fullScreen = result["fullscreen"].as<bool>();
    int key, frameCounter = 0, detectionStep = result["detect-step"].as<int>();
    double timer, fps;
    float iouThreshold = result["iou-thresh"].as<float>();
    string videoFilename(result["video"].as<string>()), trackerAlgorithm = result["tracker"].as<string>();
    string model(result["model"].as<string>()), config(result["config"].as<string>());
    string classesFile(result["classes"].as<string>());
    string windowName("Detection & Tracking");

    cv::Mat frame;
    vector<Rect> boundingBoxes;
    // Create and init the detector object
    MyDetector detector(model, config, classesFile, 416, 416, result["conf-thresh"].as<float>(),
                        result["nms-thresh"].as<float>(), result["draw-detects"].as<bool>());

    // Initialize output window
    if (fullScreen) {
        cv::namedWindow(windowName, cv::WND_PROP_FULLSCREEN);
        cv::setWindowProperty(windowName, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    } else
        cv::namedWindow(windowName);

    // Open the video and check if it is good to go
    cv::VideoCapture cap(videoFilename);
    if (!cap.isOpened()) {
        cout << "Could not open the video; exiting..." << endl;
        exit(-1);
    }
    if (!cap.read(frame)) {
        cout << "Could not read the frame from the video; exiting..." << endl;
        exit(-1);
    }

    // Use detector on first frame of the video and init the trackers object
    boundingBoxes = detector.detect(frame);
    MyTrackers trackers(boundingBoxes, trackerAlgorithm, windowName, drawIds, iouThreshold);


    // For each frame, do:
    while (cap.isOpened()) {

        // If there are no more frames in the video break the loop
        if (!cap.read(frame))
            break;

        // If frameCount reaches detection-step use detector and reset the trackers, else just update the trackers
        // Also, calculate FPS
        timer = (double) cv::getTickCount();
        if (frameCounter == 0) {
            boundingBoxes = detector.detect(frame);
            trackers.setTrackers(frame, boundingBoxes);
        } else
            boundingBoxes = trackers.update(frame);
        fps = cv::getTickFrequency() / ((double) cv::getTickCount() - timer);

        frameCounter++;
        if (frameCounter > detectionStep)
            frameCounter = 0;

        // Draw bounding boxes & show tracker and FPS info
        for (const auto &bbox : boundingBoxes)
            cv::rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);

        cv::putText(frame, trackerAlgorithm + " Tracker", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.75,
                    Scalar(50, 170, 50), 2);
        cv::putText(frame, "FPS : " + to_string((int(fps))), Point(20, 50), FONT_HERSHEY_SIMPLEX, 0.75,
                    Scalar(50, 170, 50), 2);

        cv::imshow(windowName, frame);

        key = cv::waitKey(go);
        if (key == 115) // Add an ROI manually to be tracked by pressing 'S'
            trackers.addTracker(frame);
        else if (key == 102) // Moving forward one frame at a time by pressing 'f'
            go = false;
        else if (key == 32) // Pause or go by pressing Space
            go = !go;
        else if (key == 113 || key == 27) // Exit by pressing 'Q' or ESC
            break;

    }

    // Destroy the window and release the video capture object
    cv::destroyAllWindows();
    cap.release();
    cout << "The program has been finished successfully." << endl;
    return 0;
}
