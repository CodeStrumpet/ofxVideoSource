#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    videoSource.videoType = VideoTypeMovie; // change this to set the video source...
    //videoSource.videoType = VideoTypeCodeLabsPSEye;
    //videoSource.videoType = VideoTypePS3EyeGrabber;
    
    videoSource.movieFilePath = "yu_beckon.mov"; // only used if videoType is 'VideoTypeMovie'
    
    videoSource.setupCamera();
    
    incomingImage.allocate(videoSource.getCameraWidth(), videoSource.getCameraHeight(), OF_IMAGE_COLOR);

}

//--------------------------------------------------------------
void testApp::update(){
    bool bNewFrame = false;
    
    bNewFrame = videoSource.updateFrame();
    
	if (bNewFrame) {
        
        incomingImage.setFromPixels(videoSource.getCurrFramePixels(),videoSource.getCameraWidth(),
                                    videoSource.getCameraHeight(), OF_IMAGE_COLOR);
    }


}

//--------------------------------------------------------------
void testApp::draw(){
    incomingImage.draw(0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
