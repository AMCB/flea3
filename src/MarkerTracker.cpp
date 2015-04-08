//
//  MarkerTracker.cpp
//  flea3
//
//  Created by Gal Sasson on 5/31/14.
//
//

#include "MarkerTracker.h"

MarkerTracker::MarkerTracker() : mode(MODE_PRE_CAMERA), bCalibrated(false)
{
    
}

void MarkerTracker::setup()
{
    if (!camera.setup()) {
        return;
    }
    
    background.allocate(camera.frameSize.x, camera.frameSize.y);
    graySource.allocate(camera.frameSize.x, camera.frameSize.y);
    backgroundSub.allocate(camera.frameSize.x, camera.frameSize.y);
    threshResult.allocate(camera.frameSize.x, camera.frameSize.y);
    camToScreenTrans = ofVec2f(-(ofGetWindowWidth() - camera.frameSize.x)/2, 0);
    destCorners.push_back(ofVec2f(0, 0));
    destCorners.push_back(ofVec2f(0, 1));
    destCorners.push_back(ofVec2f(1, 0));
    destCorners.push_back(ofVec2f(1, 1));
    
    mode = MODE_PRE_CALIBRATION;
}

void MarkerTracker::update()
{
    camera.update();
    
 /*   if (!camera.bInitialized) {
        return;
    }
    
    cvCvtColor(camera.camImg.getCvImage(), graySource.getCvImage(), CV_RGB2GRAY);
    graySource.flagImageChanged();
    
    backgroundSub.absDiff(graySource, background);
    
    cvThreshold(backgroundSub.getCvImage(), threshResult.getCvImage(), Params::imgThresh, 255, CV_THRESH_BINARY);
    threshResult.flagImageChanged();
    
    contourFinder.findContours(threshResult,
                               10,   // min area
                               1000,  // max area
                               4,   // num considered
                               false, // bFindHoles,
                               true); // bUseApproximation=true
    
    currentBlobs = contourFinder.blobs;
  */
}

void MarkerTracker::draw()
{
    ofPushMatrix();
    
    ofTranslate(-camToScreenTrans);
    
    if (mode > MODE_PRE_CAMERA) {
        ofSetColor(255);
        camera.draw();
        ofSetLineWidth(3);
//        contourFinder.draw();
 /*
        for (int i=0, x=0; i<contourFinder.nBlobs; i++)
        {
            ofSetColor(255);
            ofFill();
            ofRect(10 + x, camera.frameSize.y-20, 10, 10);
            x += 15;
            
            ofSetColor(255, 0, 0);
            ofFill();
            ofEllipse(currentBlobs[i].centroid, 5, 5);
            ofLine(currentBlobs[i].centroid + ofVec2f(0, 5),
                   currentBlobs[i].centroid + ofVec2f(0, 15));
            ofLine(currentBlobs[i].centroid + ofVec2f(0, -5),
                   currentBlobs[i].centroid + ofVec2f(0, -15));
            ofLine(currentBlobs[i].centroid + ofVec2f(5, 0),
                   currentBlobs[i].centroid + ofVec2f(15, 0));
            ofLine(currentBlobs[i].centroid + ofVec2f(-5, 0),
                   currentBlobs[i].centroid + ofVec2f(-15, 0));
        }
  */
    }

 /*   // draw calibration markers
    ofSetColor(40, 40, 200);
    ofFill();
    for (int i=0; i<corners.size(); i++)
    {
        ofEllipse(corners[i], 10, 10);
    }

    
    ofTranslate(0, camera.frameSize.y+10);
    
    ofSetColor(255);
    ofFill();
    ofRect(0, 0, camera.frameSize.x, camera.frameSize.y);
    
    // we are now at the bottom of the camera frame
    ofPushMatrix();
    ofTranslate(5, 150);
    ofSetColor(0);
    if (mode == MODE_PRE_CAMERA) {
        ofDrawBitmapString("Could not connect to camera", 0, 0);
    }
    else if (mode == MODE_PRE_CALIBRATION) {
        ofDrawBitmapString("System not calibrated", 0, 0);
    }
    else if (mode == MODE_CALIBRATION) {
        ofDrawBitmapString("Calibration", 0, 0);
        if (corners.size() == 0) {
            ofDrawBitmapString("Click top-left corner of projection", 0, 20);
        }
        else if (corners.size() == 1) {
            ofDrawBitmapString("Click bottom-left corner of projection", 0, 20);
        }
        else if (corners.size() == 2) {
            ofDrawBitmapString("Click top-right corner of projection", 0, 20);
        }
        else if (corners.size() == 3) {
            ofDrawBitmapString("Click bottom-right corner of projection", 0, 20);
        }
    }
    else if (mode == MODE_POST_CALIBRATION) {
        ofDrawBitmapString("System Calibrated", 0, 0);
    }
    ofPopMatrix();
    
    // draw transformations
    ofPushMatrix();
    ofTranslate(0, 20);
    ofScale(0.20, 0.20);

    ofSetColor(0);
    ofDrawBitmapString("Grayscale", 0, -15);
    ofDrawBitmapString("Background Subtraction", camera.frameSize.x + 20, -15);
    ofDrawBitmapString("Threshold", camera.frameSize.x*2 + 40, -15);
    ofSetColor(255);
    graySource.draw(0, 0);
    backgroundSub.draw(camera.frameSize.x + 20, 0);
    threshResult.draw(camera.frameSize.x*2 + 40, 0);
    contourFinder.draw(camera.frameSize.x*2 + 40, 0);
    
    ofPopMatrix();

    // draw markers position
    ofSetColor(0);
    ofTranslate(camera.frameSize.x - camera.frameSize.x/3, 20);
    ofDrawBitmapString("Normalized Marker Position", 0, -3);
    ofScale(0.3, 0.3);
    ofSetColor(50);
    ofFill();
    ofRect(0, 0, 600, 600);
    ofSetColor(0);
    for (int i=0; i<currentBlobs.size(); i++)
    {
        ofVec2f pos = pTransform.getTransformedPoint(currentBlobs[i].centroid);
        ofEllipse(pos * 600, 50, 50);
    }
  */
    
    ofPopMatrix();
}

void MarkerTracker::mousePressed(float x, float y)
{
    if (mode == MODE_CALIBRATION) {
        corners.push_back(ofVec2f(x, y) + camToScreenTrans);
        if (corners.size() >= 4) {
            
            cv::Mat mat = PerspectiveTransform::getTransformationMatrix(corners, destCorners);
            pTransform.setup(mat);
            bCalibrated = true;
            mode = MODE_POST_CALIBRATION;
        }
    }
}

void MarkerTracker::captureBackground()
{
    // copy graySource into background
    background = graySource;    
}

void MarkerTracker::calibrate()
{
    bCalibrated = false;
    mode = MODE_CALIBRATION;
    corners.clear();
}
