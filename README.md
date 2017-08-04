These are simple tools for using jpeg files set as video.

jpegs2mp4 convert jpeg files set to a mp4 file.

jpegs2video show the set as video without an output file.

The files are supposed to be the equal resolution. 
You can select a region of interest in the frame of the images to show in the video.  
You can set fps for the output. But in the case of jpegs2video, this isn't a real fps.          
You can add a text marker (part of the current jpeg file name) on the video.
We use the marker to trace current time in the video when the time encoded in the filenames of the jpegs           
in the form ....YYYYMMDDHHMMSS.jpg or ...YYYYMMDDHHMM.jpg .

The programs use OpenCV libs and stdc++.

To build:

 g++ --std=c++11 jpegs2mp4.cpp -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio  -o jpegs2mp4

 g++ --std=c++11 jpegs2video.cpp -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio -lopencv_highgui  -o jpegs2video

or 

 gcc ... -lstdc++ ...

or just: 

 make

Enjoy!
