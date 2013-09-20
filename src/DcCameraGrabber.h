//
//  DcCameraGrabber.h
//  watercolorwalls
//
//  Created by Paul Mans on 3/23/13.
//
//

#ifndef __comfort_zone__DcCameraGrabber__
#define __comfort_zone__DcCameraGrabber__

#include <iostream>
#include "ofxLibdc.h"

class DcCameraGrabber {

public:

    // ofxLibdc is a namespace, and Camera is the object
    ofxLibdc::Camera camera;

    // include these just to make sure they compile
    ofxLibdc::Grabber grabber;
    ofxLibdc::PointGrey pointGrey;

    DcCameraGrabber();
    
    void setup();

private:


};

#endif /* defined(__comfort_zone__DcCameraGrabber__) */
