#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxGif.h"
#include "ofxBlur.h"

#include "ofAtom.h"
#include "ofDyingAtom.h"

#define HOST "192.168.43.143"
#define PORT 9555
#define CPORT 9001
#define PPORT 9002
#define DPORT 9003

#define MAXCreator 7
#define MAXPreserver 5
#define MAXDestroyer 3
#define MAXAtoms MAXCreator + MAXPreserver + MAXDestroyer
#define RADIUS 30
#define FRAMERATE 30
#define BEATRATE 2
#define DYINGTIME 3
#define NEAR_THRESHOLD 60

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
    
    //Freeze Code
    ofAtom **CAtomFreeze;
    ofAtom **PAtomFreeze;
    ofAtom **DAtomFreeze;
    
    //Initializing Virtual Atoms for Image Processing Gesture
    ofAtom **virtualAtom;
    ofAtom **virtualAtomInvisible;
    
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
    int indexPreserver = 0;
    int indexDestroyer = 0;
    
    //Life of the Creators -> LifeCreator[i] = 1 (i exists in the system), = 0 (i not in the system)
    int LifeCreator[MAXCreator];
    int LifeCreatorFreeze[MAXCreator];
    
    //Life of the Preservers
    int LifePreserver[MAXPreserver];
    int LifePreserverFreeze[MAXPreserver];
    
    // Life of the Destroyers
    int LifeDestroyer[MAXDestroyer];
    int LifeDestroyerFreeze[MAXDestroyer];
    
    //Dying State of the Creators -> DyingCreator[i] = 1 (i is currently dying)
    int DyingCreator[MAXCreator];
    
    //Destruction Function for the Atom that gets hit
    void Destroy(ofAtom* Atom);
    void Granulate(ofDyingAtom * GranulateAtom);
    int dyingTimeCounter;
    
    //Finding nearest Freeze Atom of the same type
    int nearestAtom(int type, int posX, int posY);
    
    //OSC Message Receiver
    ofxOscMessage C_OSC;
    ofxOscReceiver C_receiver;
    int C_OSCPosX = 0, C_OSCPosY = 0, C_isFreeze = 2, C_isInstance = 0, C_isInteract = 0;
    int C_flagInstance = 0;
    
    ofxOscMessage P_OSC;
    ofxOscReceiver P_receiver;
    int P_OSCPosX = 0, P_OSCPosY = 0, P_isFreeze = 2, P_isInstance = 0, P_isInteract = 0;
    int P_flagInstance = 0;
    
    ofxOscMessage D_OSC;
    ofxOscReceiver D_receiver;
    int D_OSCPosX = 0, D_OSCPosY = 0, D_isFreeze = 2, D_isInstance = 0, D_isInteract = 0;
    int D_flagInstance = 0;
    
    // TODO: Add same for P and D
    
    //osc Message Variables
    ofxOscMessage M1[MAXCreator],M2[MAXPreserver],M3,M4;
    ofxOscMessage M5[MAXCreator][MAXPreserver], PowerOn;
    
    ofxGIF::fiGifLoader gifloader;
    ofxGIF::fiGifLoader gifloader2;
    
    ofTrueTypeFont font;
    ofxOscSender sender;
    ofBuffer imgAsBuffer;
    ofImage img;
};
