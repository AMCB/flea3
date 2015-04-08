//
//  Params.h
//  flea3
//
//  Created by Gal Sasson on 5/31/14.
//
//

#ifndef __flea3__Params__
#define __flea3__Params__

#include <iostream>
#include "ofMain.h"
#include "ofxGui.h"

class Params
{
public:
    
    static ofxIntSlider camExposure;
    static ofxIntSlider camShutter;
    static ofxIntSlider camSharpness;
    
    static ofxIntSlider imgThresh;
};
#endif /* defined(__flea3__Params__) */
