#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void mousePressed(int x, int y, int button);
    
    ofVideoGrabber cam;
    ofxCv::ContourFinder contourFinder1;
    ofColor targetColor1;
    ofColor targetColor2;
    ofxCv::ContourFinder contourFinder2;
    //ofColor targetColor3;
    //ofxCv::ContourFinder contourFinder3;
    
    ofxPanel gui;
    ofParameter<float> threshold1;
    ofParameter<float> threshold2;
    ofParameter<bool> trackHs;
};
