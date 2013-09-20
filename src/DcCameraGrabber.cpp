//
//  DcCameraGrabber.cpp
//  watercolorwalls
//
//  Created by Paul Mans on 3/23/13.
//
//

#include "DcCameraGrabber.h"

DcCameraGrabber::DcCameraGrabber() {
 
    
}

void DcCameraGrabber::setup() {
    
    // setup() will pick the first camera. Or if there are multiple cameras you
	// can use the number, or the GUID of the camera.
    
    //camera.getCameraCount();
    //cout << "exposure: " << camera.getCameraCount() << endl;
    
	camera.setup();
    
    cout << "camera: " << camera.getLibdcCamera() << endl;
    cout << "camera: " << camera.getImageType() << endl;

}
