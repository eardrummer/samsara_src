
#include "ofApp.h"
#include <iostream>

using namespace ofxCv;
using namespace cv;
using namespace std;

#define MAXCreator 2
#define MAXPreserver 2
#define MAXDestroyer 2
#define MAXAtoms 6
#define RADIUS 30

int indx = 0;
int flag = 0;
int n_Creator = 0;
int n_Preserver = 0;
int n_Destroyer = 0;
int n_Atoms = 0;

//Distances of the two creators from the effect ball. -> For MILESTONE.
float FxDistance[MAXCreator][MAXPreserver];


int isCollidedPreserver[MAXCreator][MAXPreserver];
int isCollidedDestroyer[MAXCreator][MAXDestroyer];

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
    contourFinder2.setTargetColor(targetColor3, TRACK_COLOR_HSV);
    
    //loading background
    gifloader.load("images/flashing.gif");
    
    //Atoms code
    //Initializing one Atom of each type
    CAtom = new ofAtom*[MAXCreator];
    PAtom = new ofAtom*[MAXPreserver];
    DAtom = new ofAtom*[MAXDestroyer];
    
    // REMOVE FOR STATIC INITIALIZATION
    /*
    n_Creator = 2;
    n_Preserver = 1;
    n_Destroyer = 1;
    n_Atoms = n_Creator + n_Preserver + n_Destroyer;
    */
    
    //Distances of the two creators from the effect ball. -> For MILESTONE.
    float FxDistance[MAXCreator][MAXPreserver];
    
    //Collision status of Creator atoms with Preserver and Dstroyer
    int isCollidedPreserver[MAXCreator][MAXPreserver];
    int isCollidedDestroyer[MAXCreator][MAXDestroyer];
    
    
    //    IPAtom = new ofAtom*[1];
    //    IPAtom[0] = new ofAtom();
    
    ofSetFrameRate(60);
    
    
    // REMOVE FOR STATIC INITIALIZATION
    
    /*
    
    // Atom Creation TODO: Replace by timed creation in update
    CAtom[0] = new ofAtom(0,0, 50, 50, RADIUS);
    PAtom[0] = new ofAtom(1,0, 200, 130, RADIUS);
    DAtom[0] = new ofAtom(2,0, 300, 60, RADIUS);
    
    CAtom[1] = new ofAtom(0,1, 50, 150, RADIUS);
    //CAtom[2] = new ofAtom(0,2,150,50,40);
    
    //PAtom[1] = new ofAtom(1,1, 200, 30, 40);
    //DAtom[1] = new ofAtom(2,1, 300, 160, 40);
    
    */
    
    /*
     for (int i=0; i<10; i++) {
     balls[i].x = ofRandomWidth();
     balls[i].y = ofRandomHeight();
     balls[i].vx = ofRandom(-10,10);
     balls[i].vy = ofRandom(-10,10);
     balls[i].radius = ofRandom(10,40);
     }
     */
    
    //OSC Code
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);
    
    gui.setup();
    gui.add(threshold1.set("Threshold", 90, 0, 255));
    gui.add(threshold2.set("Threshold", 50, 0, 255));
    gui.add(threshold3.set("Threshold", 100, 0, 255));
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
        
        contourFinder3.setTargetColor(targetColor3,TRACK_COLOR_HSV);
        contourFinder3.setThreshold(threshold3);
        contourFinder3.findContours(cam);
    }
    
    //Finding and drawing the center of the contour for targetcolor1
    n1 = contourFinder1.size();
    max1 = 0.0;
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
    
    //Finding and drawing the center of the contour for targetcolor2
    n2 = contourFinder2.size();
    max2 = 0.0;
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
    
    //Finding and drawing the center of the contour for targetcolor3
    n3 = contourFinder3.size();
    max3 = 0.0;
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
    
    if(n_Creator < MAXCreator && (ofGetFrameNum()%120==0))
    {
        CAtom[n_Creator] = new ofAtom(0,n_Creator,centroidmax1.x,centroidmax1.y,RADIUS);
        n_Creator++;
    }
    if(n_Preserver < MAXPreserver && (ofGetFrameNum() > (120*MAXCreator) && ofGetFrameNum()%120 == 0))
    {
        PAtom[n_Preserver] = new ofAtom(1,n_Preserver,centroidmax2.x,centroidmax2.y,RADIUS);
        n_Preserver++;
    }
    if(n_Destroyer < MAXDestroyer && (ofGetFrameNum() > (120*(MAXCreator + MAXPreserver)) && ofGetFrameNum()%120 == 0))
    {
        DAtom[n_Destroyer] = new ofAtom(2,n_Destroyer,centroidmax3.x,centroidmax3.y,RADIUS);
        n_Destroyer++;
    }
    
    
    for(int j = 0; j < n_Creator; j++)
        for(int i = 0; i < n_Preserver; i++){
            isCollidedPreserver[j][i] = 0;
        }
    
    for(int j = 0; j < n_Creator; j++)
        for(int i = 0; i < n_Destroyer; i++){
            isCollidedDestroyer[j][i] = 0;
        }
    
    //For drawing background
    if (ofGetFrameNum() % 2)
    {
        indx++;
        if (indx > gifloader.pages.size()-1) indx = 0;
    }
    
    //Atoms Code
    for (int i = 0; i < n_Creator; i++)
        CAtom[i]->update();
    
    for (int i = 0; i < n_Preserver; i++)
        PAtom[i]->update();
    
    for (int i = 0; i < n_Destroyer; i++)
        DAtom[i]->update();
    
    //Checking for Atoms closest to each.
    //Creator
    for(int j = 0; j < n_Creator; j++) {
        for(int i = 0; i < n_Preserver; i++){
            isCollidedPreserver[j][i] = CAtom[j]->collide(PAtom[i]);
        }
        for(int i = 0; i < n_Destroyer; i++){
            isCollidedDestroyer[j][i] = CAtom[j]->collide(DAtom[i]);
        }
        for(int i = 0; i < n_Creator; i++){
            //TODO: Check for distance from other creator atoms
            if(i!=j){
                //	CAtom[j]->collide(CAtom[i]);
            }
        }
    }
    
    //Preserver
    for(int j = 0; j < n_Preserver; j++){
        for(int i = 0; i < n_Destroyer; i++){
            PAtom[j]->collide(DAtom[i]);
        }
        for(int i = 0; i < n_Preserver; i++){
            //TODO: Make multiple Preserver atoms collide
            if(i!=j){
                PAtom[j]->collide(PAtom[i]);
            }
        }
        
    }
    //Destroyer
    for(int j = 0; j < n_Destroyer; j++)
        for(int i = 0; i < n_Destroyer; i++){
            //TODO: Check for distance from other creator atoms
            
            if(i!=j){
                DAtom[j]->collide(DAtom[i]);
            }
            
        }
    
    //OSC code
    ofxOscMessage m1,m2,m3,m4;
    m1.setAddress("/collisionType");
    m2.setAddress("/collisionType");
    m3.setAddress("/FxDistance1");
    m4.setAddress("/FxDistance2");  //TODO: Fix for n_Creator number of Sends
    
    for(int j = 0; j < n_Creator; j++){
        for(int i = 0; i < n_Preserver; i++){
            if(isCollidedPreserver[j][i] != 0){
                
                m1.addIntArg(1);	// Sending a 1 for collision with preserver
                sender.sendMessage(m1,false);
            }
        }
        
        for(int i = 0; i < n_Destroyer; i++){
            
            if(isCollidedDestroyer[j][i] != 0){
                m2.addIntArg(0);	// Sending a 0 for collision with destruction
                sender.sendMessage(m2,false);
            }
        }
    }
    
    for(int j = 0; j < n_Creator; j++){
        for(int i = 0; i < n_Preserver; i++){
            FxDistance[j][i] = ofDist(CAtom[j]->m_posX, CAtom[j]->m_posY, PAtom[i]->m_posX, PAtom[i]->m_posY);
            //TODO:Send this as OSC message
        }
    }
    m3.addIntArg(FxDistance[0][0]);
    sender.sendMessage(m3,false);
    
    m4.addIntArg(FxDistance[1][0]);
    sender.sendMessage(m4,false);
    
}

void ofApp::draw() {
    ofSetColor(255);
    ofBackground(0);
    
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
    
    //Code to flip everything laterally, write all the drawing code between this and ofPopMatrix()
    ofPushMatrix(); // save the old coordinate system
    ofTranslate( ofGetWidth(), 0.0f); // move the origin to the bottom-left hand corner of the window
    ofScale(-1.0f, 1.0f); // flip the x axis horizontally
    

    ofSetColor(magentaPrint);
    //minAreaRect1.draw();
    ofDrawCircle(centroidmax1, 15);
    
    ofSetColor(cyanPrint);
    //minAreaRect2.draw();
    ofDrawCircle(centroidmax2, 15);
    
    ofSetColor(yellowPrint);
    //minAreaRect3.draw();
    ofDrawCircle(centroidmax3, 15);
    
    //Atoms Code
    //Loop through all existing atoms.
    for(int i = 0; i < n_Creator; i++){
        CAtom[i]->draw();
    }
    for(int i = 0; i < n_Preserver; i++){
        PAtom[i]->draw();
    }
    for(int i = 0; i < n_Destroyer; i++){
        DAtom[i]->draw();
    }
    
    ofPopMatrix(); // restore the previous coordinate system
    gui.draw();
    
}

void ofApp::mousePressed(int x, int y, int button) {
    //targetColor = cam.getPixels().getColor(x, y);
}
