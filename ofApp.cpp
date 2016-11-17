
#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    cam.setup(640, 480);
    
    //targetColor2 is red, targetColor2 is blue - Similarly contourFinder1(or 2), threshold1(or 2)
    contourFinder1.setMinAreaRadius(10);
    contourFinder1.setMaxAreaRadius(150);
    
    contourFinder2.setMinAreaRadius(10);
    contourFinder2.setMaxAreaRadius(150);
    
    targetColor1.set(255,0,0);
    targetColor2.set(0,0,255);
    
    contourFinder1.setTargetColor(targetColor1, TRACK_COLOR_RGB);
    contourFinder2.setTargetColor(targetColor2, TRACK_COLOR_HS);
    
    gui.setup();
    gui.add(threshold1.set("Threshold", 90, 0, 255));
    gui.add(threshold2.set("Threshold", 30, 0, 255));
}

void ofApp::update() {
    cam.update();
    if(cam.isFrameNew()) {
        contourFinder1.setTargetColor(targetColor1,TRACK_COLOR_RGB);
        contourFinder1.setThreshold(threshold1);
        contourFinder1.findContours(cam);
        
        contourFinder2.setTargetColor(targetColor2,TRACK_COLOR_HS);
        contourFinder2.setThreshold(threshold2);
        contourFinder2.findContours(cam);
    }
    
}

void ofApp::draw() {
    ofSetColor(255);
    cam.draw(0, 0);
    
    ofSetLineWidth(2);
    contourFinder1.draw();
    contourFinder2.draw();
    
    gui.draw();
    
}

void ofApp::mousePressed(int x, int y, int button) {
    //targetColor = cam.getPixels().getColor(x, y);
}
