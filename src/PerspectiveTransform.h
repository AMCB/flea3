//
//  PerspectiveTransform.h
//  flea3
//
//  Created by Gal Sasson on 5/31/14.
//
//

#ifndef __flea3__PerspectiveTransform__
#define __flea3__PerspectiveTransform__

#include <iostream>
#include "ofMain.h"
#include "ofxOpenCv.h"

class PerspectiveTransform
{
public:
    static cv::Mat getTransformationMatrix(vector<ofVec2f> src, vector<ofVec2f> dest);
    
    PerspectiveTransform();
    void setup(const cv::Mat& matrix);
    ofVec2f getTransformedPoint(const ofVec2f& p);
    
    bool bInitialized;
    
private:
    cv::Mat transformationMatrix;
    
};
#endif /* defined(__flea3__PerspectiveTransform__) */
