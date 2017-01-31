/*
*  This is part of the Samsara Project, by Manaswi, Siddharth and Kushagra - SMC, UPF 2016
*  No part of this code may be used without contacting the authors
*
*  This is an experimental physics based virtual sound system project and we hope to develop it further and release it in the future
*  29/01/2016
*
*    Contact : manaswimishra17@gmail.com
*/

#include "ofApp.h"
#include <iostream>

using namespace std;

// 1 to Enable velocity control in Image processing
int variableVelocityFlag = 0;
int indx = 0;
int indx2 = 0;
int flag = 0;
int n_Creator = 0;
int n_Preserver = 0;
int n_Destroyer = 0;
int n_Atoms = 0;
int startOfEnd = 0;


void ofApp::setup() {
    blur.setup(1280,720,5,.2,10);
    
    //loading background
    gifloader.load("images/Star_bw.gif");
    gifloader2.load("images/giphy4.gif");
    
    //Atoms code
    //Initializing one Atom of each type
    CAtom = new ofAtom*[MAXCreator];
    PAtom = new ofAtom*[MAXPreserver];
    DAtom = new ofAtom*[MAXDestroyer];
    
    //Freeze Code
    CAtomFreeze = new ofAtom*[MAXCreator];
    PAtomFreeze = new ofAtom*[MAXPreserver];
    DAtomFreeze = new ofAtom*[MAXDestroyer];
    
    //Initializing FxMatrix (Distances of each Creator from Effects = 0 before the pair exists)
    for(int i = 0; i < MAXCreator; i++)
        for(int j = 0; j < MAXPreserver; j++)
            FxMatrix[i][j] = 0;
    
    //Initializing LifeCreator - status of each Creator
    for(int i = 0; i < MAXCreator; i++){
        LifeCreator[i] = 0;
        LifeCreatorFreeze[i] = 0;

	// dummy variables
	CAtom[i] = new ofAtom(0,i,0,0,RADIUS);
	CAtomFreeze[i] = new ofAtom(4,i,0,0,RADIUS);
    }
    //Initializing LifePreserver - status of each Preserver
    for(int i = 0; i < MAXPreserver; i++){
        LifePreserver[i] = 0;
        LifePreserverFreeze[i] = 0;

        // dummy variables
        PAtom[i] = new ofAtom(1,i,0,0,RADIUS);
        PAtomFreeze[i] = new ofAtom(5,i,0,0,RADIUS);

    }
    //Initializing LifeDestroyer - status
    for(int i = 0; i < MAXDestroyer; i++){
        LifeDestroyer[i] = 0;
        LifeDestroyerFreeze[i] = 0;

        // dummy variables
        DAtom[i] = new ofAtom(2,i,0,0,RADIUS);
        DAtomFreeze[i] = new ofAtom(6,i,0,0,RADIUS);

    }
    //Initializing dying Environment - Status to check if that Creator is currently dying and its pos,vel
    dyingEnvironment = new ofDyingAtom*[MAXCreator];
    dyingEnd = new ofDyingAtom*[MAXAtoms];
    
    for(int i = 0; i < MAXCreator; i++){
        dyingEnvironment[i] = new ofDyingAtom(0,0,0,0,0);
    }
    
    for(int j = 0; j < MAXAtoms; j++){
        dyingEnd[j] = new ofDyingAtom(0,0,0,0,0);
    }
    TheEnd = 0;
    //Initializing 3 virtual atoms for image processing
    virtualAtom = new ofAtom*[3];
    virtualAtom[0] = new ofAtom(3, 0, 0, 0 , RADIUS/2);
    virtualAtom[1] = new ofAtom(3, 1, 0, 0 , RADIUS/2);
    virtualAtom[2] = new ofAtom(3, 2, 0, 0 , RADIUS/2);
    
    virtualAtomInvisible = new ofAtom*[3];
    virtualAtomInvisible[0] = new ofAtom(7, 0, 0, 0 , RADIUS/2);
    virtualAtomInvisible[1] = new ofAtom(7, 1, 0, 0 , RADIUS/2);
    virtualAtomInvisible[2] = new ofAtom(7, 2, 0, 0 , RADIUS/2);
    
    
    // Setting Frame Rate for processing
    ofSetFrameRate(FRAMERATE);
    //timeBeginPeriod(1);   // https://forum.openframeworks.cc/t/emptyexample-running-at-50fps-in-release-mode/13731/7
    
    
    //OSC Code
    
    // OSC Receiving -------------------------
    C_receiver.setup(CPORT);
    P_receiver.setup(PPORT);
    D_receiver.setup(DPORT);
    
     // PD -----------------------------------------------------------------------------------------
     
     
     // open an outgoing connection to HOST:PORT
     sender.setup(HOST, PORT);
     
     // Setting OSC Addresses ---------------------------------------------------
     PowerOn.setAddress("/power");
     M3.setAddress("/collisionPreserver");
     M4.setAddress("/collisionDestroyer");
     M6.setAddress("/drum");
     
     // FxMatrix
     std::string msg;
     for(int i = 0; i < MAXCreator; i++){
     for(int j = 0; j < MAXPreserver; j++){
     msg.append("/FxMatrix/");
     msg.append(ofToString(i+1));
     msg.append("/");
     msg.append(ofToString(j+1));
     M5[i][j].setAddress(msg);
     msg = "";
     }
     }
     
     // LifeCreator
     msg = "";
     for(int i = 0; i < MAXCreator; i++){
     msg.append("/lifeCreator/");
     msg.append(ofToString(i+1));
     M1[i].setAddress(msg);
     msg = "";
     }
     
     // LifePreserver
     msg = "";
     for(int i = 0; i < MAXPreserver; i++){
     msg.append("/lifePreserver/");
     msg.append(ofToString(i+1));
     M2[i].setAddress(msg);
     msg = "";
     }
     
    // -------------------------------------------------------------------------------------
}

void ofApp::update() {
   
    
    // Blurring for graphics
    blur.setScale(0.2);
    
    // NEW OSC CONTROLLED CODE   ------------------------------------------------------------------------
    
    // Assign these values from OSC
    //  C_OSCPosX, C_OSCPosY;   C_isFreeze, C_isInstance, C_isInteract
    //  P_OSCPosX, P_OSCPosY;   P_isFreeze, P_isInstance, P_isInteract
    //  D_OSCPosX, D_OSCPosY;   D_isFreeze, D_isInstance, D_isInteract
    
    
    while(C_receiver.hasWaitingMessages()){
        
        C_receiver.getNextMessage(C_OSC);
        
        if(C_OSC.getAddress() == "/freeze"){
            C_isFreeze = C_OSC.getArgAsInt32(0);
        }
        if(C_OSC.getAddress() == "/push1"){
            C_isInstance = C_OSC.getArgAsInt32(0);
        }
        if(C_OSC.getAddress() == "/push2"){
            C_isInteract = C_OSC.getArgAsInt32(0);
        }
        if(C_OSC.getAddress() == "/xyPad"){
            C_OSCPosX = 1280 * C_OSC.getArgAsFloat(0);
            C_OSCPosY = 720 * C_OSC.getArgAsFloat(1);
        }
        
        
        // Check for Creator Freeze OSC Msg

        // Not in freeze mode
        if(C_isFreeze == 0 || C_isFreeze == 2) {
            
            //Displaying Cursor Position as virtualAtomInvisible
            virtualAtomInvisible[0]->assign(7, 0 , C_OSCPosX , C_OSCPosY , 0, 0, RADIUS/2);
            
            for(int i = 0; i < MAXCreator; i++){
                
                //Copying all Frozen Atoms to UnFreeze State
                if(LifeCreatorFreeze[i] == 1){
                    
                    CAtom[i]->assign(0,i, CAtomFreeze[i]->m_posX, CAtomFreeze[i]->m_posY , RADIUS);
                    LifeCreator[i] = 1;
                    LifeCreatorFreeze[i] = 0;
                    //delete [] CAtomFreeze[i];
                    
                }
            }
            
            //Creating Atom
            if(C_isInstance == 1 && C_flagInstance == 0){
                C_flagInstance = 1;
                indexCreator = MAXCreator;
                for(int i = 0; i < MAXCreator; i++){
                    if(LifeCreator[i] == 0){
                        indexCreator = i;
                        break;
                    }
                }
                
                if(indexCreator < MAXCreator){
                    CAtom[indexCreator]->assign(0,indexCreator, C_OSCPosX, C_OSCPosY , RADIUS);
                    LifeCreator[indexCreator] = 1;
                    if(n_Creator < MAXCreator-1)
		    n_Creator++;
                }
            }
            else if(C_isInstance == 0 && C_flagInstance == 1)
                C_flagInstance = 0;
            
            
            //Allowing Interaction (Bhasad)
            //Bhasad on
            if(C_isInteract == 1){
                virtualAtom[0]->assign(3, 0 , C_OSCPosX , C_OSCPosY , 0, 0, RADIUS/2);
                virtualAtomInvisible[0]->assign(7, 0, 0, 0, 0, 0, RADIUS/2);
                
            }
            
            //Bhasad Off
            else if(C_isInteract == 0){
                virtualAtom[0]->assign(3, 0 , 0 , 0 , 0, 0, RADIUS/2);
                virtualAtomInvisible[0]->assign(7, 0 , C_OSCPosX , C_OSCPosY , 0, 0, RADIUS/2);
                
            }
        }
        
        
        // Freeze Mode
        else if(C_isFreeze == 1){
            
            // Freezing all Atoms and copying them into freeze state
            for(int i = 0; i < MAXCreator; i++){
                
                if(LifeCreator[i] == 1){
                    
                    LifeCreator[i] = 0;
                    LifeCreatorFreeze[i] = 1;
                    
                    CAtomFreeze[i]->assign(4, i, CAtom[i]->m_posX, CAtom[i]->m_posY, RADIUS);
                    //delete [] CAtom[i];
                }
            }
            
            // Deleting an atom
            if(C_isInstance == 1 && C_flagInstance == 0){
                
                C_flagInstance = 1;
                
                // Returns index of nearest atom of same type, and -1 if none are near.
                indexCreator = nearestAtom( 0, C_OSCPosX, C_OSCPosY);
                if(indexCreator != -1){
                    
                    LifeCreatorFreeze[indexCreator] = 0;
                    //delete [] CAtomFreeze[indexCreator];
                }
            }
            else if(C_isInstance == 0 && C_flagInstance == 1){
                C_flagInstance = 0;
            }
            
            
            // Allow Moving the Atom
            if(C_isInteract == 1){
                
                virtualAtom[0]->assign(3, 0 , C_OSCPosX , C_OSCPosY , 0, 0, RADIUS/2);
                virtualAtomInvisible[0]->assign(7, 0, 0, 0, 0, 0, RADIUS/2);
                
                indexCreator = nearestAtom( 0, virtualAtom[0]->m_posX, virtualAtom[0]->m_posY);
                if(indexCreator != -1){
                    
                    CAtomFreeze[indexCreator]->assign(4, indexCreator, C_OSCPosX, C_OSCPosY, 0, 0, RADIUS);
                    
                }
                
            }
            else if(C_isInteract == 0){
                
                virtualAtom[0]->assign(3, 0 , 0 , 0 , 0, 0, RADIUS/2);
                virtualAtomInvisible[0]->assign(7, 0 , C_OSCPosX , C_OSCPosY , 0, 0, RADIUS/2);
                
            }
            
        }
        
        
    } // END OF WAITING FOR OSC --------------------------------------------------------------
    
    while(P_receiver.hasWaitingMessages()){
        
        P_receiver.getNextMessage(P_OSC);
        
        if(P_OSC.getAddress() == "/freeze"){
            P_isFreeze = P_OSC.getArgAsInt32(0);
        }
        if(P_OSC.getAddress() == "/push1"){
            P_isInstance = P_OSC.getArgAsInt32(0);
        }
        if(P_OSC.getAddress() == "/push2"){
            P_isInteract = P_OSC.getArgAsInt32(0);
        }
        if(P_OSC.getAddress() == "/xyPad"){
            P_OSCPosX = 1280 * P_OSC.getArgAsFloat(0);
            P_OSCPosY = 720 * P_OSC.getArgAsFloat(1);
        }
        
        
        // Check for Creator Freeze OSC Msg
        // Not in freeze mode
        if(P_isFreeze == 0 || P_isFreeze == 2) {
            
            //Displaying Cursor Position as virtualAtomInvisible
            virtualAtomInvisible[1]->assign(7, 1 , P_OSCPosX , P_OSCPosY , 0, 0, RADIUS/2);
            
            for(int i = 0; i < MAXPreserver; i++){
                
                //Copying all Frozen Atoms to UnFreeze State
                if(LifePreserverFreeze[i] == 1){
                    
                    PAtom[i]->assign(1,i, PAtomFreeze[i]->m_posX, PAtomFreeze[i]->m_posY , RADIUS);
                    LifePreserver[i] = 1;
                    LifePreserverFreeze[i] = 0;
                    //delete [] PAtomFreeze[i];
                    
                }
            }
            
            //Creating Atom
            if(P_isInstance == 1 && P_flagInstance == 0){
                P_flagInstance = 1;
                indexPreserver = MAXPreserver;
                for(int i = 0; i < MAXPreserver; i++){
                    if(LifePreserver[i] == 0){
                        indexPreserver = i;
                        break;
                    }
                }
                
                if(indexPreserver < MAXPreserver){
                    PAtom[indexPreserver]->assign(1,indexPreserver, P_OSCPosX, P_OSCPosY , RADIUS);
                    LifePreserver[indexPreserver] = 1;
                    if(n_Preserver < MAXPreserver-1)
                    n_Preserver++;
                }
            }
            else if(P_isInstance == 0 && P_flagInstance == 1)
                P_flagInstance = 0;
            
            
            //Allowing Interaction (Bhasad)
            //Bhasad on
            if(P_isInteract == 1){
                virtualAtom[1]->assign(3, 1 , P_OSCPosX , P_OSCPosY , 0, 0, RADIUS/2);
                virtualAtomInvisible[1]->assign(7, 1, 0, 0, 0, 0, RADIUS/2);
                
            }
            
            //Bhasad Off
            else if(P_isInteract == 0){
                virtualAtom[1]->assign(3, 1 , 0 , 0 , 0, 0, RADIUS/2);
                virtualAtomInvisible[1]->assign(7, 1 , P_OSCPosX , P_OSCPosY , 0, 0, RADIUS/2);
                
            }
        }
        
        
        // Freeze Mode
        else if(P_isFreeze == 1){
            
            // Freezing all Atoms and copying them into freeze state
            for(int i = 0; i < MAXPreserver; i++){
                
                if(LifePreserver[i] == 1){
                    
                    LifePreserver[i] = 0;
                    LifePreserverFreeze[i] = 1;
                    
                    PAtomFreeze[i]->assign(5, i, PAtom[i]->m_posX, PAtom[i]->m_posY, RADIUS);
                    //delete [] PAtom[i];
                }
            }
            
            // Deleting an atom
            if(P_isInstance == 1 && P_flagInstance == 0){
                
                P_flagInstance = 1;
                
                // Returns index of nearest atom of same type, and -1 if none are near.
                indexPreserver = nearestAtom( 1, P_OSCPosX, P_OSCPosY);
                if(indexPreserver != -1){
                    
                    LifePreserverFreeze[indexPreserver] = 0;
                    //delete [] PAtomFreeze[indexPreserver];
                }
            }
            else if(P_isInstance == 0 && P_flagInstance == 1){
                P_flagInstance = 0;
            }
            
            
            // Allow Moving the Atom
            if(P_isInteract == 1){
                
                virtualAtom[1]->assign(3, 1 , P_OSCPosX , P_OSCPosY , 0, 0, RADIUS/2);
                virtualAtomInvisible[1]->assign(7, 1, 0, 0, 0, 0, RADIUS/2);
                
                indexPreserver = nearestAtom( 1, virtualAtom[1]->m_posX, virtualAtom[1]->m_posY);
                if(indexPreserver != -1){
                    
                    PAtomFreeze[indexPreserver]->assign(5, indexPreserver, P_OSCPosX, P_OSCPosY, 0, 0, RADIUS);
                    
                }
                
            }
            else if(P_isInteract == 0){
                
                virtualAtom[1]->assign(3, 1 , 0 , 0 , 0, 0, RADIUS/2);
                virtualAtomInvisible[1]->assign(7, 1 , P_OSCPosX , P_OSCPosY , 0, 0, RADIUS/2);
                
            }
            
        }
        
        
    }
    // --------------------------------------------------------------------------------
    
    while(D_receiver.hasWaitingMessages()){
        
        D_receiver.getNextMessage(D_OSC);
        
        if(D_OSC.getAddress() == "/freeze"){
            D_isFreeze = D_OSC.getArgAsInt32(0);
        }
        if(D_OSC.getAddress() == "/push1"){
            D_isInstance = D_OSC.getArgAsInt32(0);
        }
        if(D_OSC.getAddress() == "/push2"){
            D_isInteract = D_OSC.getArgAsInt32(0);
        }
        if(D_OSC.getAddress() == "/xyPad"){
            D_OSCPosX = 1280 * D_OSC.getArgAsFloat(0);
            D_OSCPosY = 720 * D_OSC.getArgAsFloat(1);
        }
        
        
        // Check for Creator Freeze OSC Msg
        // Not in freeze mode
        if(D_isFreeze == 0 || D_isFreeze == 2) {
            
            //Displaying Cursor Position as virtualAtomInvisible
            virtualAtomInvisible[2]->assign(7, 2 , D_OSCPosX , D_OSCPosY , 0, 0, RADIUS/2);
            
            for(int i = 0; i < MAXDestroyer; i++){
                
                //Copying all Frozen Atoms to UnFreeze State
                if(LifeDestroyerFreeze[i] == 1){
                    
                    DAtom[i]->assign(2,i, DAtomFreeze[i]->m_posX, DAtomFreeze[i]->m_posY , RADIUS);
                    LifeDestroyer[i] = 1;
                    LifeDestroyerFreeze[i] = 0;
                    //delete [] DAtomFreeze[i];
                    
                }
            }
            
            //Creating Atom
            if(D_isInstance == 1 && D_flagInstance == 0){
                D_flagInstance = 1;
                indexDestroyer = MAXDestroyer;
                for(int i = 0; i < MAXDestroyer; i++){
                    if(LifeDestroyer[i] == 0){
                        indexDestroyer = i;
                        break;
                    }
                }
                
                if(indexDestroyer < MAXDestroyer){
                    DAtom[indexDestroyer]->assign(2,indexDestroyer, D_OSCPosX, D_OSCPosY , RADIUS);
                    LifeDestroyer[indexDestroyer] = 1;
                    n_Destroyer++;
                }
            }
            else if(D_isInstance == 0 && D_flagInstance == 1)
                D_flagInstance = 0;
            
            
            //Allowing Interaction (Bhasad)
            //Bhasad on
            if(D_isInteract == 1){
                virtualAtom[2]->assign(3, 2 , D_OSCPosX , D_OSCPosY , 0, 0, RADIUS/2);
                virtualAtomInvisible[2]->assign(7, 2, 0, 0, 0, 0, RADIUS/2);
                
            }
            
            //Bhasad Off
            else if(D_isInteract == 0){
                virtualAtom[2]->assign(3, 2 , 0 , 0 , 0, 0, RADIUS/2);
                virtualAtomInvisible[2]->assign(7, 2 , D_OSCPosX , D_OSCPosY , 0, 0, RADIUS/2);
                
            }
        }
        
        
        // Freeze Mode
        else if(D_isFreeze == 1){
            
            // Freezing all Atoms and copying them into freeze state
            for(int i = 0; i < MAXDestroyer; i++){
                
                if(LifeDestroyer[i] == 1){
                    
                    LifeDestroyer[i] = 0;
                    LifeDestroyerFreeze[i] = 1;
                    
                    DAtomFreeze[i]->assign(6, i, DAtom[i]->m_posX, DAtom[i]->m_posY, RADIUS);
                    //delete [] DAtom[i];
                }
            }
            
            // Deleting an atom
            if(D_isInstance == 1 && D_flagInstance == 0){
                
                D_flagInstance = 1;
                
                // Returns index of nearest atom of same type, and -1 if none are near.
                indexDestroyer = nearestAtom( 2, D_OSCPosX, D_OSCPosY);
                if(indexDestroyer != -1){
                    
                    LifeDestroyerFreeze[indexDestroyer] = 0;
                    //delete [] DAtomFreeze[indexDestroyer];
                }
            }
            else if(D_isInstance == 0 && D_flagInstance == 1){
                D_flagInstance = 0;
            }
            
            
            // Allow Moving the Atom
            if(D_isInteract == 1){
                
                virtualAtom[2]->assign(3, 2 , D_OSCPosX , D_OSCPosY , 0, 0, RADIUS/2);
                virtualAtomInvisible[2]->assign(7, 2, 0, 0, 0, 0, RADIUS/2);
                
                indexDestroyer = nearestAtom( 2, virtualAtom[2]->m_posX, virtualAtom[2]->m_posY);
                if(indexDestroyer != -1){
                    
                    DAtomFreeze[indexDestroyer]->assign(6, indexDestroyer, D_OSCPosX, D_OSCPosY, 0, 0, RADIUS);
                    
                }
                
            }
            else if(D_isInteract == 0){
                
                virtualAtom[2]->assign(3, 2 , 0 , 0 , 0, 0, RADIUS/2);
                virtualAtomInvisible[2]->assign(7, 2 , D_OSCPosX , D_OSCPosY , 0, 0, RADIUS/2);
                
            }
            
        }
        
        
    }
    
    
    // NEW CODE ENDS HERE ------------------------------------------------------------------------------------
    
    
    // -------/-----------------------------------------------------------------------------------------------
    
    /*
     
     // Creating Atoms at every BEATRATE seconds --------------------------------------------------------------
     if((ofGetFrameNum()%(BEATRATE*FRAMERATE)==0) && TheEnd == 0 )
     {
     indexCreator = MAXCreator;
     for(int i = 0; i < MAXCreator; i++){
     if(LifeCreator[i] == 0){
     indexCreator = i;    // First Available Creator ID
     break;
     }
     }
     
     //Checking if there is space in the environment for creating a Creator Atom
     if( indexCreator < MAXCreator){
     
     CAtom[indexCreator] = new ofAtom(0,indexCreator,centroidmax1.x,centroidmax1.y,RADIUS);
     
     if(variableVelocityFlag)
     CAtom[indexCreator]->assignVelocity(velocity1.x,velocity1.y);
     
     LifeCreator[indexCreator] = 1;
     
     // Update n_Creator if it hasnt reached MAX even once.
     if(n_Creator < MAXCreator)
     n_Creator++;
     }
     }
     
     // Creating Preserver Atoms at every 2 seconds
     if(TheEnd == 0 && n_Preserver < MAXPreserver && (ofGetFrameNum() > (BEATRATE*FRAMERATE*MAXCreator) && ofGetFrameNum()%(BEATRATE*FRAMERATE) == 0))
     {
     PAtom[n_Preserver] = new ofAtom(1,n_Preserver,centroidmax2.x,centroidmax2.y,RADIUS);
     
     LifePreserver[n_Preserver] = 1;
     
     if(variableVelocityFlag)
     PAtom[n_Preserver]->assignVelocity(velocity2.x,velocity2.y);
     n_Preserver++;
     }
     
     // Creating Destroyer Atoms at every 2 seconds
     if(TheEnd == 0 && n_Destroyer < MAXDestroyer && (ofGetFrameNum() > ((BEATRATE*FRAMERATE)*(MAXCreator + MAXPreserver)) && ofGetFrameNum()%(BEATRATE*FRAMERATE) == 0))
     {
     DAtom[n_Destroyer] = new ofAtom(2,n_Destroyer,centroidmax3.x,centroidmax3.y,RADIUS);
     
     LifeDestroyer[n_Destroyer] = 1;
     
     if(variableVelocityFlag)
     DAtom[n_Destroyer]->assignVelocity(velocity3.x,velocity3.y);
     n_Destroyer++;
     }
     
     */
    
    //Initializing Collision status matrix --------------------------------------------------------------
    for(int j = 0; j < MAXCreator; j++)
        for(int i = 0; i < MAXPreserver; i++){
            isCollidedPreserver[j][i] = 0;
        }
    
    for(int j = 0; j < MAXCreator; j++)
        for(int i = 0; i < MAXDestroyer; i++){
            isCollidedDestroyer[j][i] = 0;
        }
    
    for(int i = 0; i < MAXCreator; i++)
        isCollidedVirtual[i] = 0;
    
    //Initializing FxMatrix every update frame
    for(int i = 0; i < MAXCreator; i++)
        for(int j = 0; j < MAXPreserver; j++)
            FxMatrix[i][j] = 0;
    
    
    if(startOfEnd == 0){
        //For drawing background
        if (ofGetFrameNum() % 2)
        {
            indx++;
            if (indx > gifloader.pages.size()-1) indx = 0;
        }}
    else {
        //For drawing background
        if (ofGetFrameNum() % 2)
        {
            indx2++;
            if (indx2 > gifloader2.pages.size()-1) indx2 = 0;
        }
    }
    //Atoms Code ------------------------------------------------------------------------------
    
    
    for (int i = 0; i < MAXCreator; i++){
        if(LifeCreator[i])
            CAtom[i]->update();
    }
    
    for (int i = 0; i < MAXPreserver; i++){
        if(LifePreserver[i])
            PAtom[i]->update();
    }
    for (int i = 0; i < MAXDestroyer; i++){
        if(LifeDestroyer[i])
            DAtom[i]->update();
    }
    
    //COLLISION CHECK --------------------------------------------------------------------------
    
    flag = 0;
    //Checking for Atoms Collisions
    //Creator
    for(int j = 0; j < MAXCreator; j++) {
        for(int i = 0; i < MAXPreserver; i++){
            
            if(LifeCreator[j] && LifePreserver[i]){
                isCollidedPreserver[j][i] = CAtom[j]->collide(PAtom[i]);
                
                if(isCollidedPreserver[j][i] == 1 && TheEnd == 1){
                    Destroy(CAtom[j]);
		    LifeCreator[j] = 0;
                    Destroy(PAtom[i]);
		    LifePreserver[i] = 0;
                }
            }
        }
        
        // Destruction Collisions
        for(int i = 0; i < MAXDestroyer; i++){
            
            if(LifeCreator[j] && LifeDestroyer[i]){
                isCollidedDestroyer[j][i] = CAtom[j]->collide(DAtom[i]);
                if(isCollidedDestroyer[j][i] == 2){
                    Destroy(CAtom[j]);
		    LifeCreator[j] = 0;
                    
                    if(TheEnd == 1){
                        Destroy(DAtom[i]);
			LifeDestroyer[i] = 0;
		    }
                }
            }
        }
        // Virtual Atom Collisions
        for(int i = 0; i < 3; i ++){
            if(LifeCreator[j]){
                isCollidedVirtual[j] = CAtom[j]->collide(virtualAtom[i]);
                if(isCollidedVirtual[j] == 3 && i == 2){
                    Destroy(CAtom[j]);
		    LifeCreator[j] = 0;
		}
            }
        }
        
        for(int i = 0; i < MAXCreator; i++){
            //TODO: Check for distance from other creator atoms
            
            if(LifeCreator[j] && LifeCreator[i])
                if(i!=j){
                    flag = CAtom[j]->collide(CAtom[i]);
                    if(TheEnd == 1 && flag == 1){
                        Destroy(CAtom[j]);
			LifeCreator[j] = 0;
                        Destroy(CAtom[i]);
			LifeCreator[i] = 0;
                    }
                }
        }
    }
    
    //Preserver
    for(int j = 0; j < MAXPreserver; j++){
        for(int i = 0; i < MAXDestroyer; i++){
            if(LifePreserver[j] && LifeDestroyer[i]){
                flag = PAtom[j]->collide(DAtom[i]);
                if(TheEnd == 1 && flag == 1){
                    Destroy(PAtom[j]);
		    LifePreserver[j] = 0;
                    Destroy(DAtom[i]);
		    LifeDestroyer[i] = 0;
                }
            }
        }
        for(int i = 0; i < MAXPreserver; i++){
            if(LifePreserver[j] && LifePreserver[i])
                if(i!=j){
                    flag = PAtom[j]->collide(PAtom[i]);
                    if(TheEnd == 1 && flag == 1){
                        Destroy(PAtom[j]);
			LifePreserver[j] = 0;
                        Destroy(PAtom[i]);
			LifePreserver[i] = 0;
                    }
                }
        }
        // Virtual Atom Collisions
        for(int i = 0; i < 3; i ++){
            if(LifePreserver[j])
                PAtom[j]->collide(virtualAtom[i]);
        }
        
    }
    //Destroyer
    for(int j = 0; j < MAXDestroyer; j++){
        for(int i = 0; i < MAXDestroyer; i++){
            if(LifeDestroyer[j] && LifeDestroyer[i])
                if(i!=j){
                    flag = DAtom[j]->collide(DAtom[i]);
                    if(TheEnd == 1 && flag == 1){
                        Destroy(DAtom[j]);
			LifeDestroyer[j] = 0;
                        Destroy(DAtom[i]);
			LifeDestroyer[i] = 0;
                    }
                }
            
        }
        // Virtual Atom Collisions
        for(int i = 0; i < 3; i ++){
            if(LifeDestroyer[j])
                DAtom[j]->collide(virtualAtom[i]);
        }
        
    }
    //Kill the Last atom
    flag = 0;
    if(TheEnd == 1){
        for(int i = 0; i < MAXCreator; i++)
            if(LifeCreator[i])
                flag++;
        
        for(int i = 0; i < MAXPreserver; i++)
            if(LifePreserver[i])
                flag++;
        
        for(int i = 0; i < MAXDestroyer; i++)
            if(LifeDestroyer[i])
                flag++;
        
        if(flag == 1){
            for(int i = 0; i < MAXCreator; i++)
                if(LifeCreator[i]){
                    Destroy(CAtom[i]);
		    LifeCreator[i] = 0;
		}
            for(int i = 0; i < MAXPreserver; i++)
                if(LifePreserver[i]){
                    Destroy(PAtom[i]);
		    LifePreserver[i] = 0;
		}
            for(int i = 0; i < MAXDestroyer; i++)
                if(LifeDestroyer[i]){
                    Destroy(DAtom[i]);
		    LifeDestroyer[i] = 0;
		}
            
        }
    }
    
    
    // EFFECTS MATRIX
    
    for(int j = 0; j < MAXCreator; j++){
        for(int i = 0; i < MAXPreserver; i++){
            
            if(LifeCreator[j] && LifePreserver[i]  && TheEnd == 0)
                FxMatrix[j][i] = CAtom[j]->atomFxDist(PAtom[i]);
        }
    }
    
    
    // Debugging Display
    
    /*
     for(int i = 0; i < MAXCreator; i++)
     cout<<LifeCreator[i]<<" ";
     cout<<"C"<<endl;
     
     for(int i = 0; i < MAXPreserver; i++)
     cout<<LifePreserver[i]<<" ";
     cout<<"P"<<endl;
     
     for(int i = 0;i < MAXDestroyer; i++)
     cout<<LifeDestroyer[i]<<" ";
     cout<<"D"<<endl;
     */
    
     /*
     for(int j = 0; j < n_Creator; j++){
     for(int i = 0; i < n_Preserver; i++){
     cout<<FxMatrix[j][i]<<" ";
     }
     cout<< endl;
     }
     
     cout<<" Frame Khatm "<<endl;
     */
    
    //OSC codes ----------------------------------------------------------------------------------
    
     // First Msg on first frame of code
     if(ofGetFrameNum() == 1){
     PowerOn.addIntArg(1);
     sender.sendMessage(PowerOn,false);
     }
     
     // OSC for FxMatrix
     for(int i = 0; i < n_Creator; i++){
     for(int j = 0; j < n_Preserver; j++){
     M5[i][j].addIntArg(FxMatrix[i][j]);
     sender.sendMessage(M5[i][j],false);
     M5[i][j].clear();
     }
     }
     // OSC for LifeCreator
     for(int i = 0; i < MAXCreator; i++){
     M1[i].addIntArg(LifeCreator[i] + LifeCreatorFreeze[i]);
     sender.sendMessage(M1[i],false);
     M1[i].clear();
     }

     // OSC for LifePreserver
     for(int i = 0; i < MAXPreserver; i++){
     M2[i].addIntArg(LifePreserver[i] + LifePreserverFreeze[i]);
     sender.sendMessage(M2[i],false);
     M2[i].clear();
     }
     
     //OSC for drum - number of Atoms in the System (normalized to 7)
     int n_Atoms = 0;
     for(int i = 0; i < MAXCreator; i++){
     
	if(LifeCreator[i] == 1 || LifeCreatorFreeze[i] == 1)
		n_Atoms++;
     }
     for(int i = 0; i < MAXPreserver; i++){
	
	if(LifePreserver[i] == 1 || LifePreserverFreeze[i] == 1)
		n_Atoms++;	
     }
     for(int i = 0; i < MAXDestroyer; i++){
	
	if(LifeDestroyer[i] == 1 || LifeDestroyerFreeze[i] == 1)
		n_Atoms++;

     }
     n_Atoms = int((n_Atoms / float(MAXAtoms)) * 7.0);
     
     M6.addIntArg(n_Atoms);
     sender.sendMessage(M6,false);
     M6.clear();

     // OSC for Collision Granular
     for(int j = 0; j < MAXCreator; j++){
     for(int i = 0; i < MAXPreserver; i++){
     if(isCollidedPreserver[j][i] != 0){
     
     M3.addIntArg(1);	// Sending a 1 for collision with preserver
     sender.sendMessage(M3,false);
     }
     }
     for(int i = 0; i < n_Destroyer; i++){
     if(isCollidedDestroyer[j][i] != 0){
     M4.addIntArg(1);	// Sending a 1 for collision with destruction
     sender.sendMessage(M4,false);
     }
     }
     if(isCollidedVirtual[j] != 0){
     M4.addIntArg(1);    // Sending a 1 for collision with destroying virtual atom
     sender.sendMessage(M4,false);
     }
     
     }
     
}

void ofApp::draw() {
    
    blur.begin();
    
    ofSetColor(255);
    ofBackground(0);
    
    //For drawing background: the image size is 498*480
    //Comment the next line for viewing the camera input
    if(startOfEnd == 1){
        gifloader2.pages[indx2].draw(0,0,1280,720);}
    else gifloader.pages[indx].draw(391, 120);
    
    ofSetLineWidth(2);
    
    if(startOfEnd==1)
        ofNoFill();
    
    //Atoms Code
    //Loop through all existing atoms.
    
    
    // TODO:Check for Freeze mode
    
    for(int i = 0; i < MAXCreator; i++){
        
        if(LifeCreator[i])
            CAtom[i]->draw();
    }
    for(int i = 0; i < MAXPreserver; i++){
        if(LifePreserver[i])
            PAtom[i]->draw();
    }
    for(int i = 0; i < MAXDestroyer; i++){
        if(LifeDestroyer[i])
            DAtom[i]->draw();
    }
    
    
    
    // Draw Virtual Atoms
    for(int i = 0; i < 3; i++){
        virtualAtom[i]->draw();
    }
    
    // TODO: Draw Virtual Atoms in Un Freeze Mode
    for(int i = 0; i < 3; i++){
        virtualAtomInvisible[i]->draw();
    }
    
    // Draw Frozen Atoms
    for(int i = 0; i < MAXCreator; i++){
        
        if(LifeCreatorFreeze[i])
            CAtomFreeze[i]->draw();
    }
    for(int i = 0; i < MAXPreserver; i++){
        if(LifePreserverFreeze[i])
            PAtomFreeze[i]->draw();
    }
    for(int i = 0; i < MAXDestroyer; i++){
        if(LifeDestroyerFreeze[i])
            DAtomFreeze[i]->draw();
    }
    
    
    
    // DYING CREATOR ATOMS GRAPHICS
    if( TheEnd == 0){
        for(int i = 0; i < MAXCreator; i++){
            if(dyingEnvironment[i]->statusCheck() == 1){
                if(ofGetElapsedTimef() < DYINGTIME){
                    Granulate(dyingEnvironment[i]);
                    dyingEnvironment[i]->m_dyingTimeCounter++;
                }
                else{
                    dyingEnvironment[i]->statusAssign(0);
                    dyingEnvironment[i]->m_dyingTimeCounter = 0;
                }
            }
        }
    }
    // DYING GRAPHICS AT THE END
    
    if(TheEnd == 1){
        
        for(int i = 0; i < MAXAtoms; i++){
            if(dyingEnd[i]->statusCheck() == 1){
                if(ofGetElapsedTimef() < DYINGTIME){
                    
                    Granulate(dyingEnd[i]);
                    dyingEnd[i]->m_dyingTimeCounter++;
                }
                else{
                    dyingEnd[i]->statusAssign(0);
                    dyingEnd[i]->m_dyingTimeCounter = 0;
                }
            }
        }
    }
    
    blur.end();
    blur.draw();
}

void ofApp::mousePressed(int x, int y, int button) {
    //targetColor = cam.getPixels().getColor(x, y);
}


void ofApp::keyPressed(int key){
    
    switch(key){
            
        case 'q':
        case 'Q':
            for(int i = 0; i < MAXCreator; i++){
                if(LifeCreator[i])
                    delete [] CAtom[i];
                if(LifeCreatorFreeze[i])
                    delete [] CAtomFreeze[i];
            }
            for(int i = 0; i < MAXCreator; i++){
                delete [] dyingEnvironment[i];
            }
            for(int i = 0; i < MAXPreserver; i++){
                if(LifePreserver[i])
                    delete [] PAtom[i];
                if(LifePreserverFreeze[i])
                    delete [] PAtomFreeze[i];
            }
            for(int i = 0; i < MAXDestroyer; i++){
                if(LifeDestroyer[i])
                    delete [] DAtom[i];
                if(LifeDestroyerFreeze[i])
                    delete [] DAtomFreeze[i];
            }
            for(int i = 0; i < MAXAtoms; i++){
                delete [] dyingEnd[i];
            }
            for(int i = 0; i < 3; i++){
                delete [] virtualAtom[i];
                delete [] virtualAtomInvisible[i];
            }
            //timeEndPeriod(1);
            ofExit();
            break;
        case ' ':
            TheEnd = 1;
            startOfEnd = 1;
            break;
            
    }
    
}



//------------------------------------------------------------------------
// DESTRUCTION CODE

void ofApp::Destroy(ofAtom* Atom){
    
    if(Atom->m_type == 0){
        LifeCreator[Atom->m_id] = 0;
        if(TheEnd == 0)
            dyingEnvironment[Atom->m_id]->assign(Atom->m_posX, Atom->m_posY, Atom->m_velocityX, Atom->m_velocityY, 1,0,0);
        
        if(TheEnd == 1)
            dyingEnd[Atom->m_id]->assign(Atom->m_posX,Atom->m_posY,Atom->m_velocityX,Atom->m_velocityY,1,0,0);
    }
    else if(Atom->m_type == 1){
        LifePreserver[Atom->m_id] = 0;
        dyingEnd[MAXCreator + Atom->m_id]->assign(Atom->m_posX, Atom->m_posY, Atom->m_velocityX, Atom->m_velocityY, 1, 0,1);
    }
    else if(Atom->m_type == 2){
        LifeDestroyer[Atom->m_id] = 0;
        dyingEnd[MAXCreator + MAXPreserver + Atom->m_id]->assign(Atom->m_posX, Atom->m_posY, Atom->m_velocityX, Atom->m_velocityY, 1,0,2);
    }
    // This sets a reset on the counter upto DYINGTIME	
    ofResetElapsedTimeCounter();
    
    // Free the memory assigned to Atom
    //delete [] Atom;
    
}

void ofApp::Granulate(ofDyingAtom * GranulateAtom){
    
    
    int scale = GranulateAtom->m_dyingTimeCounter;
    int directionX = ofSign(GranulateAtom->m_velocityX);
    int directionY = ofSign(GranulateAtom->m_velocityY);
    int alpha = 5; // Rate at which the granules move away
    
    int opacity = 500 - (10 + (scale * 10));
    
    ofFill();
    if(GranulateAtom->m_type == 0)
        ofSetColor(225,255,255,opacity);
    else if(GranulateAtom->m_type == 1)
        ofSetColor(60,250,115,opacity);
    else
        ofSetColor(255,60,0,opacity);
    
    
    ofDrawCircle(GranulateAtom->m_posX, GranulateAtom->m_posY, RADIUS/3 );
    ofDrawCircle(GranulateAtom->m_posX + (alpha*scale*directionX), GranulateAtom->m_posY, RADIUS/3);
    ofDrawCircle(GranulateAtom->m_posX + (alpha*scale*directionX/2), GranulateAtom->m_posY, RADIUS/3);
    ofDrawCircle(GranulateAtom->m_posX, GranulateAtom->m_posY + (alpha*scale * directionY), RADIUS/3);
    ofDrawCircle(GranulateAtom->m_posX, GranulateAtom->m_posY + (alpha*scale * directionY/2), RADIUS/3);
    ofDrawCircle(GranulateAtom->m_posX + (alpha*scale*directionX), GranulateAtom->m_posY + (alpha*scale*directionY), RADIUS/3);
    ofDrawCircle(GranulateAtom->m_posX + (alpha*scale*directionX/2), GranulateAtom->m_posY + (alpha*scale*directionY/2), RADIUS/3);
    ofDrawCircle(GranulateAtom->m_posX + (alpha*scale*directionX*3/2), GranulateAtom->m_posY + (alpha*scale*directionY/2), RADIUS/3);	
    ofDrawCircle(GranulateAtom->m_posX + (alpha*scale*directionX/2), GranulateAtom->m_posY + (alpha*scale*directionY*3/2), RADIUS/3);
}


int ofApp::nearestAtom(int type, int posX, int posY){
    
    int dist;
    int min = 10000;
    int index;
    
    if(type == 0){
        
        for(int i = 0; i <MAXCreator; i++){
            if(LifeCreatorFreeze[i] == 1){
                
                dist = ofDist(CAtomFreeze[i]->m_posX, CAtomFreeze[i]->m_posY, posX, posY);				
                
                if(dist < min){
                    min = dist;
                    index = i;
                }
            }
        }
        
        
    }
    else if(type == 1){
        
        for(int i = 0; i <MAXPreserver; i++){
            if(LifePreserverFreeze[i] == 1){
                
                dist = ofDist(PAtomFreeze[i]->m_posX, PAtomFreeze[i]->m_posY, posX, posY);
                if(dist < min){
                    min = dist;
                    index = i;
                }
            }
        }
    }
    else if(type == 2){
        
        for(int i = 0; i <MAXDestroyer; i++){
            if(LifeDestroyerFreeze[i] == 1){
                
                dist = ofDist(DAtomFreeze[i]->m_posX, DAtomFreeze[i]->m_posY, posX, posY);
                if(dist < min){
                    min = dist;
                    index = i;
                }
            }
        }
    }
    
    // If the nearest Atom is further than NEAR_THRESHOLD return -1
    if(min > NEAR_THRESHOLD)
        return -1;
    
    return index;
    
}
