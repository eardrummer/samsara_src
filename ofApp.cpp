
#include "ofApp.h"
#include <iostream>

using namespace ofxCv;
using namespace cv;
using namespace std;

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
    gui.add(threshold2.set("Threshold", 50, 0, 255));
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
    //contourFinder1.draw();
    //contourFinder2.draw();
    
    ofNoFill();
    
    //Finding and drawing the center of the contour for color1
    int n1 = contourFinder1.size();
    double max1 = 0.0;
    int indx1;
    ofVec2f centroidmax1;
    for(int i = 0; i < n1; i++) {
        ofVec2f centroid1 = toOf(contourFinder1.getCenter(i));
        double area1 = contourFinder1.getContourArea(i);
        if(area1 > max1 ){
            max1 = area1;
            indx1 = i;
            centroidmax1 = centroid1;
        }
    }
    ofSetColor(cyanPrint);
    ofDrawCircle(centroidmax1, 10);
    
    gui.draw();
    
}

void ofApp::mousePressed(int x, int y, int button) {
    //targetColor = cam.getPixels().getColor(x, y);
}
