//
//  MarkerTracker.h
//  flea3
//
//  Created by Gal Sasson on 5/31/14.
//
//

#ifndef __flea3__MarkerTracker__
#define __flea3__MarkerTracker__

#include <iostream>
#include "ofMain.h"
#include "ofxOpenCv.h"

#include "Camera.h"
#include "PerspectiveTransform.h"

typedef enum {
    MODE_PRE_CAMERA,
    MODE_PRE_CALIBRATION,
    MODE_CALIBRATION,
    MODE_POST_CALIBRATION
} MarkerTrackerMode;

class MarkerTracker
{
public:
    MarkerTracker();
    void setup();
    
    void update();
    void draw();
    
    void mousePressed(float x, float y);
    
    void captureBackground();
    void calibrate();
    
    bool bCalibrated;
    
private:
    MarkerTrackerMode mode;
    
    Camera camera;
    PerspectiveTransform pTransform;
    
    ofVec2f camToScreenTrans;
    vector<ofVec2f> corners;
    vector<ofVec2f> destCorners;
    
    ofxCvGrayscaleImage background;
    ofxCvGrayscaleImage graySource;
    ofxCvGrayscaleImage backgroundSub;
    ofxCvGrayscaleImage threshResult;
    
    ofxCvContourFinder contourFinder;
    vector<ofxCvBlob> currentBlobs;
    
};

#endif /* defined(__flea3__MarkerTracker__) */
