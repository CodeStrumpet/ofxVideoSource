//
//  PS3EYEGrabber.h
//  comfort-zone
//
//  Created by Paul Mans on 8/12/13.
//
//

#ifndef __comfort_zone__PS3EYEGrabber__
#define __comfort_zone__PS3EYEGrabber__

#include <iostream>

#include "ofMain.h"
#include "ps3eye.h"

class ps3eyeUpdate : public ofThread{
    
public:
    ps3eyeUpdate(){
    }
    
    void start(){
        startThread(false, false);   // blocking, verbose
    }
    
    void stop(){
        stopThread();
    }
    
    //--------------------------
    void threadedFunction()
    {
        while( isThreadRunning() != 0 )
        {
            bool res = ps3eye::PS3EYECam::updateDevices();
            if(!res)
            {
                break;
            }
        }
    }
};

class PS3EYEGrabber {
public:
    
    PS3EYEGrabber();
    ~PS3EYEGrabber();
    
    void setupCamera(int width, int height);
    
    bool updateFrame();
    
    unsigned char *getCurrFramePixels();
    
    unsigned int getCameraWidth();
    
    unsigned int getCameraHeight();
    
    void draw(int x, int y);

    
    
    ps3eye::PS3EYECam::PS3EYERef eye;
    
    int camFrameCount;
    int camFpsLastSampleFrame;
    float camFpsLastSampleTime;
    float camFps;
    
    ps3eyeUpdate threadUpdate;
    
    unsigned char * videoFrame;
    ofTexture videoTexture;
    ofImage conversionImage;
    
    
private:
    
    
};

#endif /* defined(__comfort_zone__PS3EYEGrabber__) */
