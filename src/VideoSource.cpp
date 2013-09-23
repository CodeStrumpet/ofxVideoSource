//
//  VideoSource.cpp
//  comfort-zone
//
//  Created by Paul Mans on 8/8/13.
//
//

#include "VideoSource.h"

VideoSource::VideoSource()
{
    videoType = VideoTypeOfVideoGrabber;
}


void VideoSource::setupCamera()
{

    if (videoType == VideoTypeMovie) {

        if (movieFilePath.size() <= 0) {
            cout << "error:  movieFilePath not defined" << endl;
            return;
        }

        vidPlayer.loadMovie(movieFilePath);
        vidPlayer.play();


    } else if (videoType == VideoTypeOfVideoGrabber || videoType == VideoTypeCodeLabsPSEye) {

        setupCamera(0);
        
    } else if (videoType == VideoTypeDcCameraGrabber) {

        #ifdef TARGET_OSX
        dcCameraGrabber.setup();
        dcCameraGrabber.camera.setSize(320, 240); // TODO should pass in desired width & height here
        #endif

    } else if (videoType == VideoTypePS3EyeGrabber) {
        #ifdef TARGET_OSX
        ps3EyeGrabber.setupCamera(320, 240);
        #endif
    }
}

void VideoSource::setupCamera(int deviceID) {
    if (videoType == VideoTypeOfVideoGrabber || videoType == VideoTypeCodeLabsPSEye) {
        
        cout << "ofVidGrabber is initialized: " << ofVidGrabber.isInitialized() << endl;
        ofVidGrabber.setDeviceID(deviceID);
        ofVidGrabber.initGrabber(320, 240);  // TODO should pass in desired width & height here
        ofVidGrabber.setDesiredFrameRate(60);
        
        if (videoType == VideoTypeCodeLabsPSEye) {
#ifndef TARGET_OSX
            ofVidGrabber.setAutoExposure(false);
            ofVidGrabber.setAutoGain(false);
#endif
        }
        
        // make sure there is sufficient space allocated for the current camera size
        pixels.allocate(getCameraHeight(), getCameraWidth(), OF_IMAGE_COLOR);
    }
    else {
        throw "Cannot specify a device ID with this type of video input.";
    }
}

bool VideoSource::updateFrame()
{
    bool bNewFrame = false;

    if (videoType == VideoTypeMovie) {

        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();

    } else if (videoType == VideoTypeOfVideoGrabber || videoType == VideoTypeCodeLabsPSEye) {
        ofVidGrabber.update();

        bNewFrame = ofVidGrabber.isFrameNew();

        if (bNewFrame) {
            pixels.setFromPixels(ofVidGrabber.getPixels(), ofVidGrabber.getWidth(), ofVidGrabber.getHeight(), OF_IMAGE_COLOR);
            
            //ofVidGrabberFrame.setFromPixels(ofVidGrabber.getPixels(), ofVidGrabber.getWidth(), ofVidGrabber.getHeight());

        }
    } else if (videoType == VideoTypeDcCameraGrabber) {

        #ifdef TARGET_OSX
        
        bNewFrame = dcCameraGrabber.camera.grabVideo(dcGrabberFrame);

        if (dcGrabberFrame.getWidth() != getCameraWidth() || dcGrabberFrame.getHeight() != getCameraHeight()) {

            cout << "size mismatch:  " << dcGrabberFrame.getWidth() << ", " << dcGrabberFrame.getHeight() << endl;

            dcGrabberFrame.allocate(getCameraWidth(), getCameraHeight(), OF_IMAGE_COLOR);
        }
         
        #endif
    } else if (videoType == VideoTypePS3EyeGrabber) {
        #ifdef TARGET_OSX
        bNewFrame = ps3EyeGrabber.updateFrame();
        #endif
    }
    return bNewFrame;
}

unsigned char* VideoSource::getCurrFramePixels() {
    if (videoType == VideoTypeMovie) {
        return vidPlayer.getPixels();
    } else if (videoType == VideoTypeOfVideoGrabber || videoType == VideoTypeCodeLabsPSEye) {
        return pixels.getPixels();
    } else if (videoType == VideoTypeDcCameraGrabber) {
        #ifdef TARGET_OSX
        return dcGrabberFrame.getPixels();
        #endif
    } else if (videoType == VideoTypePS3EyeGrabber) {
        #ifdef TARGET_OSX
        return ps3EyeGrabber.getCurrFramePixels();
        #endif
    }
    return NULL;
}

unsigned int VideoSource::getCameraWidth() {
    unsigned int width = 0;
    if (videoType == VideoTypeMovie) {
        return vidPlayer.getWidth();
    } else if (videoType == VideoTypeOfVideoGrabber || videoType == VideoTypeCodeLabsPSEye) {
        width = ofVidGrabber.getWidth();
    } else if (videoType == VideoTypeDcCameraGrabber) {
        #ifdef TARGET_OSX
        width = dcCameraGrabber.camera.getWidth();
        #endif
    } else if (videoType == VideoTypePS3EyeGrabber) {
        #ifdef TARGET_OSX
        return ps3EyeGrabber.getCameraWidth();
        #endif
    }
    return width;
}

unsigned int VideoSource::getCameraHeight() {
    unsigned int height = 0;
    if (videoType == VideoTypeMovie) {
        return vidPlayer.getHeight();
    } else if (videoType == VideoTypeOfVideoGrabber || videoType == VideoTypeCodeLabsPSEye) {
        height = ofVidGrabber.getHeight();
    } else if (videoType == VideoTypeDcCameraGrabber) {
        #ifdef TARGET_OSX
        height = dcCameraGrabber.camera.getHeight();
        #endif
    } else if (videoType == VideoTypePS3EyeGrabber) {
        #ifdef TARGET_OSX
        return ps3EyeGrabber.getCameraHeight();
        #endif
    }
    return height;
}


void VideoSource::draw(int x, int y) {
    if (videoType == VideoTypePS3EyeGrabber) {
#ifdef TARGET_OSX
        ps3EyeGrabber.draw(x, y);
#endif
    }
}

