#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxGif.h"

#include "ofAtom.h"

#define HOST "10.80.138.34"
#define PORT 9455

/*
 struct Ball {
 int x;
 int y;
 int vx;
 int vy;
 int radius;
 };
 */


class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void mousePressed(int x, int y, int button);
    void keyPressed(int key);
    
    double max1;
    int indx1;
    ofVec2f centroidmax1;
    ofPolyline minAreaRect1;
    int n1;
    
    double max2;
    int indx2;
    ofVec2f centroidmax2;
    ofPolyline minAreaRect2;
    int n2;
    
    double max3;
    int indx3;
    ofVec2f centroidmax3;
    ofPolyline minAreaRect3;
    int n3;
    
    //Atom Code
    // Initializing the Different atoms dynamically
    // TODO: Replace with ofAtom **CAtom
    ofAtom **CAtom;
    ofAtom **PAtom;
    ofAtom **DAtom;
    
    
    ofxGIF::fiGifLoader gifloader;
    ofVideoGrabber cam;
    ofxCv::ContourFinder contourFinder1;
    ofColor targetColor1;
    ofColor targetColor2;
    ofxCv::ContourFinder contourFinder2;
    ofColor targetColor3;
    ofxCv::ContourFinder contourFinder3;
    cv::Point2f center1;
    cv::Point2f center2;
    cv::Point2f center3;
    
    ofxPanel gui;
    ofParameter<float> threshold1;
    ofParameter<float> threshold2;
    ofParameter<float> threshold3;
    ofParameter<bool> trackHs;
    
    //OSC code
    ofTrueTypeFont font;
    ofxOscSender sender;
    ofBuffer imgAsBuffer;
    ofImage img;
};
