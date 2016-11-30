
#include "ofApp.h"
#include <iostream>

using namespace ofxCv;
using namespace cv;
using namespace std;

int indx = 0;
void ofApp::setup() {
    cam.setup(640, 480);
    
    //targetColor1 is red, targetColor2 is blue, targetColor3 is yellow - Similarly contourFinder1(or 2 or 3), threshold1(or 2 or 3)
    contourFinder1.setMinAreaRadius(10);
    contourFinder1.setMaxAreaRadius(200);
    
    contourFinder2.setMinAreaRadius(10);
    contourFinder2.setMaxAreaRadius(200);
    
    contourFinder3.setMinAreaRadius(10);
    contourFinder3.setMaxAreaRadius(200);
    
    targetColor1.set(255,0,0);
    targetColor2.set(0,0,255);
    targetColor3.set(255,255,0);
    
    contourFinder1.setTargetColor(targetColor1, TRACK_COLOR_RGB);
    contourFinder2.setTargetColor(targetColor2, TRACK_COLOR_HS);
    contourFinder2.setTargetColor(targetColor3, TRACK_COLOR_RGB);
    
    //loading background
    gifloader.load("images/flashing.gif");
    
    //Balls code
    ofBackground(0, 0, 0);
    ofSetFrameRate(30);
    
    for (int i=0; i<10; i++) {
        balls[i].x = ofRandomWidth();
        balls[i].y = ofRandomHeight();
        balls[i].vx = ofRandom(-10,10);
        balls[i].vy = ofRandom(-10,10);
        balls[i].radius = ofRandom(10,40);
    }
    
    //OSC Code
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);
    
    gui.setup();
    gui.add(threshold1.set("Threshold", 90, 0, 255));
    gui.add(threshold2.set("Threshold", 50, 0, 255));
    gui.add(threshold3.set("Threshold", 90, 0, 255));
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
        
        contourFinder3.setTargetColor(targetColor3,TRACK_COLOR_RGB);
        contourFinder3.setThreshold(threshold3);
        contourFinder3.findContours(cam);
    }
    
    //For drawing background
    if (ofGetElapsedTimeMillis() % 3)
    {
        indx++;
        if (indx > gifloader.pages.size()-1) indx = 0;
    }
    
    //Balls code
    for (int i=0; i<10; i++) {
        
        balls[i].x = balls[i].x + balls[i].vx;
        balls[i].y = balls[i].y + balls[i].vy;
        
        if (balls[i].x<0 || balls[i].x > ofGetWidth()) {
            balls[i].vx = -balls[i].vx;
        }
        
        if (balls[i].y<0 || balls[i].y > ofGetHeight()) {
            balls[i].vy = -balls[i].vy;
        }
    }
    
    //OSC code
    ofxOscMessage m;
    m.setAddress("/sendxOfBall_1");
    m.addIntArg(balls[0].y);
    //m.addStringArg("down");
    sender.sendMessage(m, false);
    
}

void ofApp::draw() {
    ofSetColor(255);
    
    //Uncomment the next line for viewing the camera input
    //cam.draw(0, 0);
    //cam.draw(cam.getWidth(),0,-cam.getWidth(),cam.getHeight());  //For laterally inverting cam video
    
    //For drawing background: 71 is the starting x position as the image size is 498*480
    //Comment the next line for viewing the camera input
    gifloader.pages[indx].draw(71, 0);
    
    ofSetLineWidth(2);
    //contourFinder1.draw();
    //contourFinder2.draw();
    
    ofNoFill();
    
    //Finding and drawing the center of the contour for targetcolor1
    ofPolyline minAreaRect1;
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
            minAreaRect1 = toOf(contourFinder1.getMinAreaRect(i));
        }
    }
    ofSetColor(magentaPrint);
    minAreaRect1.draw();
    ofDrawCircle(centroidmax1, 10);
    
    
    //Finding and drawing the center of the contour for targetcolor2
    ofPolyline minAreaRect2;
    int n2 = contourFinder2.size();
    double max2 = 0.0;
    int indx2;
    ofVec2f centroidmax2;
    for(int i = 0; i < n2; i++) {
        ofVec2f centroid2 = toOf(contourFinder2.getCenter(i));
        double area2 = contourFinder2.getContourArea(i);
        if(area2 > max2 ){
            max2 = area2;
            indx2 = i;
            centroidmax2 = centroid2;
            minAreaRect2 = toOf(contourFinder2.getMinAreaRect(i));
        }
    }
    ofSetColor(cyanPrint);
    minAreaRect2.draw();
    ofDrawCircle(centroidmax2, 10);
    
    
    //Finding and drawing the center of the contour for targetcolor1
    ofPolyline minAreaRect3;
    int n3 = contourFinder3.size();
    double max3 = 0.0;
    int indx3;
    ofVec2f centroidmax3;
    for(int i = 0; i < n3; i++) {
        
        ofVec2f centroid3 = toOf(contourFinder3.getCenter(i));
        double area3 = contourFinder3.getContourArea(i);
        if(area3 > max3 ){
            max3 = area3;
            indx3 = i;
            centroidmax3 = centroid3;
            minAreaRect3 = toOf(contourFinder3.getMinAreaRect(i));
        }
    }
    ofSetColor(yellowPrint);
    minAreaRect3.draw();
    ofDrawCircle(centroidmax3, 10);
    
    //Balls code
    for (int i=0; i<10; i++) {
        ofSetColor(255, 255, 0);
        ofDrawCircle(balls[i].x, balls[i].y, balls[i].radius);
    }
    
    gui.draw();
    
}

void ofApp::mousePressed(int x, int y, int button) {
    //targetColor = cam.getPixels().getColor(x, y);
}
