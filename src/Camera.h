//
//  Camera.h
//  flea3
//
//  Created by Gal Sasson on 5/30/14.
//
//

#ifndef __flea3__Camera__
#define __flea3__Camera__

#include <iostream>
#include "ofMain.h"
#include <dc1394/dc1394.h>
#include "ofxOpenCv.h"

#include "Params.h"

#define PRIx64 "llx"

class Camera
{
public:
    Camera();
    ~Camera();
    bool setup();
    void update();
    void draw();
    
    ofVec2f frameSize;
    
    vector<uint64_t> getDeviceList();
    
    ofxCvColorImage camImg;
    ofImage simpleImage;

    bool bInitialized;

private:
    unsigned int imgWidth, imgHeight;
    ofxCvGrayscaleImage cvRaw8Img;
    
    dc1394_t* dc1394lib;
    dc1394camera_t *camera;
    dc1394video_mode_t video_mode = (dc1394video_mode_t)0;
    
    bool flea3_start();
    void flea3_set_params();
    void flea3_stop();
    void cleanup_and_exit(dc1394camera_t *camera);
    
    
//    ofxOpen
};
#endif /* defined(__flea3__Camera__) */
