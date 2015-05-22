#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(false);
    ofSetFrameRate(100);
    
    initGui();
    
    mtracker.setup();
}

//--------------------------------------------------------------
void testApp::update()
{
    mtracker.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(255);
    
    mtracker.draw();
    
 //   gui.draw();

    ofSetColor(255);
    stringstream ss;
    ss << "fps: "<<ofGetFrameRate();
    ofDrawBitmapString(ss.str(), ofGetWindowWidth() - 200, 20);
}

void testApp::exit()
{
}

void testApp::initGui()
{
    Params::camExposure.setup("camExposure", 1, 1, 1023);
    Params::camShutter.setup("camShutter", 802, 1, 1023);
    Params::camSharpness.setup("camSharpness", 1024, 0, 4095);
    Params::imgThresh.setup("Image Thresh", 128, 0, 255);
    
    captureBackground.addListener(&mtracker, &MarkerTracker::captureBackground);
    calibrate.addListener(&mtracker, &MarkerTracker::calibrate);
    
    gui.setup();
    gui.add(&Params::camExposure);
    gui.add(&Params::camShutter);
    gui.add(&Params::camSharpness);
    gui.add(&Params::imgThresh);
    
    gui.add(captureBackground.setup("Capture Background"));
    gui.add(calibrate.setup("Enter Calibration"));
    
    gui.loadFromFile("settings.xml");
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    cout << key << endl;

	if(key == 's') {
		gui.saveToFile("settings.xml");
	}
	else if(key == 'l') {
		gui.loadFromFile("settings.xml");
	}
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
    mtracker.mousePressed(x, y);
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

