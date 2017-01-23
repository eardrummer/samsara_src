#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxGif.h"
#include "ofxBlur.h"
#include "ofxNetwork.h"

#include "ofAtom.h"
#include "ofDyingAtom.h"

#define HOST "192.168.1.38"
#define PORT 9555

#define USER_CREATOR 0
#define USER_PRESERVER  1
#define USER_DESTROYER 0

#define MAXCreator 15
#define MAXPreserver 5
#define MAXDestroyer 3
#define MAXAtoms MAXCreator + MAXPreserver + MAXDestroyer
#define RADIUS 30
#define FRAMERATE 30
#define BEATRATE 2
#define DYINGTIME 3


class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void mousePressed(int x, int y, int button);
    void keyPressed(int key);
    
    ofxBlur blur;
    
    //Atom Code
    // Initializing the Different atoms dynamically
    ofAtom **CAtom;
    ofAtom **PAtom;
    ofAtom **DAtom;
    
    //Initializing Virtual Atoms for Image Processing Gesture
    ofAtom **virtualAtom;
    
    //Status of each dying creator atom
    ofDyingAtom **dyingEnvironment;
    
    //dying status of all atoms at the end
    ofDyingAtom **dyingEnd;
    int TheEnd = 0;
    
    //Distances of the two creators from the effect ball. -> For MILESTONE.
    float FxMatrix[MAXCreator][MAXPreserver];
    
    //Collision Status of each Creator with all other Atoms
    int isCollidedPreserver[MAXCreator][MAXPreserver];
    int isCollidedDestroyer[MAXCreator][MAXDestroyer];
    int isCollidedVirtual[MAXCreator];
    
    //Index to identify a Creator
    int indexCreator = 0;
    
    //Life of the Creators -> LifeCreator[i] = 1 (i exists in the system), = 0 (i not in the system)
    int LifeCreator[MAXCreator];
    
    //Life of the Preservers
    int LifePreserver[MAXPreserver];
    
    // Life of the Destroyers
    int LifeDestroyer[MAXDestroyer];
    
    //Dying State of the Creators -> DyingCreator[i] = 1 (i is currently dying)
    int DyingCreator[MAXCreator];
    
    //Destruction Function for the Atom that gets hit
    void Destroy(ofAtom* Atom);
    void Granulate(ofDyingAtom * GranulateAtom);
    int dyingTimeCounter;
    
    
    //osc Message Variables
    ofxOscMessage M1[MAXCreator],M2[MAXPreserver],M3,M4;
    ofxOscMessage M5[MAXCreator][MAXPreserver], PowerOn;
    
    ofxGIF::fiGifLoader gifloader;
    ofxGIF::fiGifLoader gifloader2;
    
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
    
    //TCP Handling
    ofxTCPServer TCP;
    vector <string> storeText;
    uint64_t lastSent;
    
    ofxTCPClient tcpClient;
    string msgTx,msgRx;
    float counter;
    int connectTime;
    int deltaTime;
    
};
