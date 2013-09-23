//
//  VideoSource.h
//  comfort-zone
//
//  Created by Paul Mans on 8/8/13.
//
//

#ifndef __comfort_zone__VideoSource__
#define __comfort_zone__VideoSource__

#include <iostream>
#include "ofMain.h"

#ifdef TARGET_OSX
#include "DcCameraGrabber.h"
#include "PS3EYEGrabber.h"
#else
#include "ofxCLeye.h"
#endif

typedef enum VideoType {
    VideoTypeMovie,
    VideoTypeOfVideoGrabber,
    VideoTypeDcCameraGrabber,
    VideoTypePS3EyeGrabber,
    VideoTypeCodeLabsPSEye
} VideoType;


class VideoSource {
public:

    VideoType videoType;

    string movieFilePath;

    VideoSource();

    void setupCamera();
    void setupCamera(int deviceID);

    bool updateFrame();

    unsigned char *getCurrFramePixels();

    unsigned int getCameraWidth();

    unsigned int getCameraHeight();

    void draw(int x, int y);

private:

#ifdef TARGET_OSX
    ofVideoGrabber ofVidGrabber;
#else
    ofxCLeye ofVidGrabber;
#endif
    
    ofVideoPlayer vidPlayer;
    
    ofPixels pixels;

#ifdef TARGET_OSX

    DcCameraGrabber dcCameraGrabber;

    ofImage dcGrabberFrame;

    PS3EYEGrabber ps3EyeGrabber;

#endif

};


#endif /* defined(__comfort_zone__VideoSource__) */
