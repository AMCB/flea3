//
//  PerspectiveTransform.cpp
//  flea3
//
//  Created by Gal Sasson on 5/31/14.
//
//

#include "PerspectiveTransform.h"


cv::Mat PerspectiveTransform::getTransformationMatrix(vector<ofVec2f> src, vector<ofVec2f> dst)
{
    cv::Mat mat;
    if (src.size() < 4 ||
        dst.size() < 4 ||
        src.size() != dst.size())
    {
        return mat;
    }

    cv::Point2f cvSrc[4];
    for (int i=0; i<4; i++) {
        cvSrc[i].x = src[i].x;
        cvSrc[i].y = src[i].y;
    }
    
    cv::Point2f cvDst[4];
    for (int i=0; i<4; i++) {
        cvDst[i].x = dst[i].x;
        cvDst[i].y = dst[i].y;
    }
    
    return getPerspectiveTransform(cvSrc, cvDst);
}


PerspectiveTransform::PerspectiveTransform() : bInitialized(false)
{
}

void PerspectiveTransform::setup(const cv::Mat& matrix)
{
    transformationMatrix = matrix;
    bInitialized = true;
}

ofVec2f PerspectiveTransform::getTransformedPoint(const ofVec2f& p)
{
    ofVec2f res;
    
    if (!bInitialized) {
        return res;
    }
    
    cv::Mat sample = (cv::Mat_<double>(3, 1) << p.x, p.y, 1);
    cv::Mat result = transformationMatrix * sample;
    
    double s = result.at<double>(2, 0);
    res.x = result.at<double>(0, 0)/s;
    res.y = result.at<double>(1, 0)/s;

    return res;
}
