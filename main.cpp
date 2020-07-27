#include "MyTrackers.h"
#include "MyDetector.h"

int main(int argc, char **argv) {

    int key, frameCounter = 0, detectionStep = 60;
    double timer, fps;
    string videoFilename, trackerAlgorithm, windowName("Tracking");
    string model("/home/aref/CLionProjects/tracking/.models/yolov3.weights");
    string config("/home/aref/CLionProjects/tracking/.models/yolov3.cfg");

    cv::Mat frame;
    vector<Rect> boundingBoxes;
    MyDetector detector(model, config, 416, 416, 0.5, 1, false);

    if (argc < 2) {
        cout << "Using default args...\n" << endl;
        videoFilename = "/home/aref/CLionProjects/tracking/.videos/los_angeles.mp4";
        cout << "Tracker type is not provided; using default KCF tracker" << endl;
        trackerAlgorithm = "KCF";;
    } else {
        videoFilename = argv[1];
        if (argv[2] != nullptr)
            trackerAlgorithm = argv[2];
    }

    cv::VideoCapture cap(videoFilename);
    if (!cap.isOpened()) {
        cout << "Could not open the video; exiting..." << endl;
        return -1;
    }
    if (!cap.read(frame)) {
        cout << "Could not read the frame from the video; exiting..." << endl;
        return -1;
    }

    boundingBoxes = detector.detect(frame);
    MyTrackers trackers(boundingBoxes, trackerAlgorithm, windowName);


    while (cap.isOpened()) {

        if (!cap.read(frame))
            break;

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

        for (const auto &bbox : boundingBoxes)
            cv::rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);

        cv::putText(frame, trackerAlgorithm + " Tracker", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.75,
                    Scalar(50, 170, 50), 2);
        cv::putText(frame, "FPS : " + to_string((int(fps))), Point(20, 50), FONT_HERSHEY_SIMPLEX, 0.75,
                    Scalar(50, 170, 50), 2);

        cv::imshow(windowName, frame);

        key = cv::waitKey(1);
        if (key == 115) // Add an ROI manually to be tracked by pressing 'S'
            trackers.addTracker(frame);
        else if (key == 113 || key == 27) // Exit by pressing 'Q' or ESC
            break;

    }

    cv::destroyAllWindows();
    cap.release();
    cout << "The program has been finished successfully." << endl;

    return 0;
}
