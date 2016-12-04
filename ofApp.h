#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxGif.h"

#include "ofAtom.h"

#define HOST "10.80.139.239"
#define PORT 8000

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
