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

Note: You need to pass a few mandatory arguments to options. 

Use `./tracking --help` to see options manual

