**Usage:**

````
wget https://pjreddie.com/media/files/yolov3.weights -P ./models/
wget https://github.com/pjreddie/darknet/blob/master/cfg/yolov3.cfg?raw=true -O ./models/yolov3.cfg
wget https://github.com/pjreddie/darknet/blob/master/data/coco.names?raw=true -O ./models/coco.names
mkdir build
cd build
cmake ..
make
./tracking [Options]
````

---

Note: You need to pass a few mandatory arguments to options (`--video, --model, --config, --classes`).
However, except for the video path, other default paths should be working fine.

Use `./tracking --help` to see options manual.


-------------

**TODO** 

````
1. Even if the detector fails to detect the tracked-object try to track it using previous ROI.
2. Make box-matching algorithm efficient.
3. When a tracked-object is lost (and not found after a while), make its ID available to newly added objects.
4. Make the trackers more efficient and let them run in parallel.
5. Add support for various object detectors.
6. Add more functions to GUI.
````
