//
//  PS3EYEGrabber.cpp
//  comfort-zone
//
//  Created by Paul Mans on 8/12/13.
//
//

#include "PS3EYEGrabber.h"


static const int ITUR_BT_601_CY = 1220542;
static const int ITUR_BT_601_CUB = 2116026;
static const int ITUR_BT_601_CUG = -409993;
static const int ITUR_BT_601_CVG = -852492;
static const int ITUR_BT_601_CVR = 1673527;
static const int ITUR_BT_601_SHIFT = 20;

static void yuv422_to_rgba(const uint8_t *yuv_src, const int stride, uint8_t *dst, const int width, const int height)
{
    const int bIdx = 2;
    const int uIdx = 0;
    const int yIdx = 0;
    
    const int uidx = 1 - yIdx + uIdx * 2;
    const int vidx = (2 + uidx) % 4;
    int j, i;
    
#define _max(a, b) (((a) > (b)) ? (a) : (b))
#define _saturate(v) static_cast<uint8_t>(static_cast<uint32_t>(v) <= 0xff ? v : v > 0 ? 0xff : 0)
    
    for (j = 0; j < height; j++, yuv_src += stride)
    {
        uint8_t* row = dst + (width * 4) * j; // 4 channels
        
        for (i = 0; i < 2 * width; i += 4, row += 8)
        {
            int u = static_cast<int>(yuv_src[i + uidx]) - 128;
            int v = static_cast<int>(yuv_src[i + vidx]) - 128;
            
            int ruv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CVR * v;
            int guv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CVG * v + ITUR_BT_601_CUG * u;
            int buv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CUB * u;
            
            int y00 = _max(0, static_cast<int>(yuv_src[i + yIdx]) - 16) * ITUR_BT_601_CY;
            row[2-bIdx] = _saturate((y00 + ruv) >> ITUR_BT_601_SHIFT);
            row[1]      = _saturate((y00 + guv) >> ITUR_BT_601_SHIFT);
            row[bIdx]   = _saturate((y00 + buv) >> ITUR_BT_601_SHIFT);
            row[3]      = (0xff);
            
            int y01 = _max(0, static_cast<int>(yuv_src[i + yIdx + 2]) - 16) * ITUR_BT_601_CY;
            row[6-bIdx] = _saturate((y01 + ruv) >> ITUR_BT_601_SHIFT);
            row[5]      = _saturate((y01 + guv) >> ITUR_BT_601_SHIFT);
            row[4+bIdx] = _saturate((y01 + buv) >> ITUR_BT_601_SHIFT);
            row[7]      = (0xff);
        }
    }
}

PS3EYEGrabber::PS3EYEGrabber() {
    
}

PS3EYEGrabber::~PS3EYEGrabber() {
    threadUpdate.stop();
    // You should stop before exiting
    // otherwise the app will keep working
    if(eye) {
        eye->stop();
        delete[] videoFrame;
    }
}

void PS3EYEGrabber::setupCamera(int width, int height) {
    using namespace ps3eye;
    
    camFrameCount = 0;
    camFpsLastSampleFrame = 0;
    camFpsLastSampleTime = 0;
    camFps = 0;
    
    // list out the devices
    std::vector<PS3EYECam::PS3EYERef> devices( PS3EYECam::getDevices() );
    
    if(devices.size())
    {
        eye = devices.at(0);
        bool res = eye->init(width, height, 60);
        eye->start();
        
        videoFrame 	= new unsigned char[eye->getWidth()*eye->getHeight()*4];
        videoTexture.allocate(eye->getWidth(), eye->getHeight(), GL_RGBA);
        
        threadUpdate.start();
    }
}

bool PS3EYEGrabber::updateFrame() {
    bool isNewFrame = false;
    if(eye)
    {
        isNewFrame = eye->isNewFrame();
        if(isNewFrame)
        {
            yuv422_to_rgba(eye->getLastFramePointer(), eye->getRowBytes(), videoFrame, eye->getWidth(),eye->getHeight());
            videoTexture.loadData(videoFrame, eye->getWidth(),eye->getHeight(), GL_RGBA);
        }
        
        camFrameCount += isNewFrame ? 1: 0;
        float timeNow = ofGetElapsedTimeMillis();
        if( timeNow > camFpsLastSampleTime + 1000 ) {
            uint32_t framesPassed = camFrameCount - camFpsLastSampleFrame;
            camFps = (float)(framesPassed / ((timeNow - camFpsLastSampleTime)*0.001f));
            
            camFpsLastSampleTime = timeNow;
            camFpsLastSampleFrame = camFrameCount;
        }
    }
    return isNewFrame;
}

unsigned char* PS3EYEGrabber::getCurrFramePixels() {
    
    // have to do conversion from rgba to rgb (!!! Inneficient !!!)
    conversionImage.setFromPixels(videoFrame, eye->getWidth(), eye->getHeight(), OF_IMAGE_COLOR_ALPHA);
    conversionImage.setImageType(OF_IMAGE_COLOR);
    
    // return the rgb pixels
    return conversionImage.getPixels();
}

unsigned int PS3EYEGrabber::getCameraWidth() {
    return eye->getWidth();
}

unsigned int PS3EYEGrabber::getCameraHeight() {
    return eye->getHeight();
}

/* 
 *  Method used for testing current camera feed
 */
void PS3EYEGrabber::draw(int x, int y) {
    ofSetHexColor(0xffffff);
    videoTexture.draw(x,y,eye->getWidth(),eye->getHeight());
}

