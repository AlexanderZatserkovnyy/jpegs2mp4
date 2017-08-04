CC=g++
CXXFLAGS += -g -Wall -O3 --std=c++11
LDLIBS= -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio  
LDLIBS2=$(LDLIBS) -lopencv_highgui
TARGET1_SOURCES = jpegs2mp4.cpp
TARGET2_SOURCES = jpegs2video.cpp
TARGET1 = jpegs2mp4
TARGET2 = jpegs2video

.PHONY: all mp4 viewer

all: mp4 viewer

mp4: $(TARGET1)

viewer: $(TARGET2)

$(TARGET1): $(TARGET1_SOURCES) 
	$(CC) $(CXXFLAGS) $^ $(LDLIBS) -o $@

$(TARGET2): $(TARGET2_SOURCES)
	$(CC) $(CXXFLAGS) $^ $(LDLIBS2) -o $@

