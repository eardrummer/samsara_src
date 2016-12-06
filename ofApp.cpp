
#include "ofApp.h"
#include <iostream>

using namespace ofxCv;
using namespace cv;
using namespace std;

// 1 to Enable velocity control in Image processing
int variableVelocityFlag = 0;
int indx = 0;
int flag = 0;
int n_Creator = 0;
int n_Preserver = 0;
int n_Destroyer = 0;
int n_Atoms = 0;


void ofApp::setup() {
    cam.setup(1280,720);
    
    //targetColor1 is red, targetColor2 is blue, targetColor3 is yellow - Similarly contourFinder1(or 2 or 3), threshold1(or 2 or 3)
    contourFinder1.setMinAreaRadius(10);
    contourFinder1.setMaxAreaRadius(200);
    
    contourFinder2.setMinAreaRadius(10);
    contourFinder2.setMaxAreaRadius(200);
    
    contourFinder3.setMinAreaRadius(10);
    contourFinder3.setMaxAreaRadius(200);
    
    targetColor1.set(255,0,0);
    targetColor2.set(0,0,255);
    targetColor3.set(0,255,0);
    
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

    //Initializing FxMatrix (Distances of each Creator from Effects = -1 before the pair exists)
    for(int i = 0; i < MAXCreator; i++)
	for(int j = 0; j < MAXPreserver; j++)
		FxMatrix[i][j] = -1;

    //Initializing LifeCreator - status of each Creator
    for(int i = 0; i < MAXCreator; i++)
	LifeCreator[i] = 0;


    //Initializing DyingCreator - Status to check if that Creator is currently dying
    for(int i = 0; i < MAXCreator; i++)
        DyingCreator[i] = 0;


    //    IPAtom = new ofAtom*[1];
    //    IPAtom[0] = new ofAtom();
    
    ofSetFrameRate(60);
    
     
    //OSC Code
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);
    
    gui.setup();
    gui.add(threshold1.set("Threshold1", 90, 0, 255));
    gui.add(threshold2.set("Threshold2", 50, 0, 255));
    gui.add(threshold3.set("Threshold3", 100, 0, 255));
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
        double area1 = contourFinder1.getContourArea(i);
        if(area1 > max1 ){
            max1 = area1;
            indx1 = i;
            centroidmax1 = toOf(contourFinder1.getCenter(i));
            velocity1 = toOf(contourFinder1.getVelocity(i));
        }
    }
    
    //Finding and drawing the center of the contour for targetcolor2
    n2 = contourFinder2.size();
    max2 = 0.0;
    for(int i = 0; i < n2; i++) {
        double area2 = contourFinder2.getContourArea(i);
        if(area2 > max2 ){
            max2 = area2;
            indx2 = i;
            centroidmax2 = toOf(contourFinder2.getCenter(i));
            velocity2 = toOf(contourFinder2.getVelocity(i));
        }
    }
    
    //Finding and drawing the center of the contour for targetcolor3
    n3 = contourFinder3.size();
    max3 = 0.0;
    for(int i = 0; i < n3; i++) {
        double area3 = contourFinder3.getContourArea(i);
        if(area3 > max3 ){
            max3 = area3;
            indx3 = i;
            centroidmax3 = toOf(contourFinder3.getCenter(i));
            velocity3 = toOf(contourFinder3.getVelocity(i));
        }
    }
   



    // Creating Atoms at every 2 seconds = 120 frames 
    if((ofGetFrameNum()%120==0))
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
    if(n_Preserver < MAXPreserver && (ofGetFrameNum() > (120*MAXCreator) && ofGetFrameNum()%120 == 0))
    {
        PAtom[n_Preserver] = new ofAtom(1,n_Preserver,centroidmax2.x,centroidmax2.y,RADIUS);
        
	if(variableVelocityFlag)
	PAtom[n_Preserver]->assignVelocity(velocity2.x,velocity2.y);
	n_Preserver++;
    }

    // Creating Destroyer Atoms at every 2 seconds
    if(n_Destroyer < MAXDestroyer && (ofGetFrameNum() > (120*(MAXCreator + MAXPreserver)) && ofGetFrameNum()%120 == 0))
    {
        DAtom[n_Destroyer] = new ofAtom(2,n_Destroyer,centroidmax3.x,centroidmax3.y,RADIUS);
        
	if(variableVelocityFlag)
	DAtom[n_Destroyer]->assignVelocity(velocity3.x,velocity3.y);
	n_Destroyer++;
    }
    
    

    //Initializing Collision status matrix
    for(int j = 0; j < MAXCreator; j++)
        for(int i = 0; i < MAXPreserver; i++){
            isCollidedPreserver[j][i] = 0;
        }
    
    for(int j = 0; j < MAXCreator; j++)
        for(int i = 0; i < MAXDestroyer; i++){
            isCollidedDestroyer[j][i] = 0;
        }

    //Initializing FxMatrix every update frame
    for(int i = 0; i < MAXCreator; i++)
        for(int j = 0; j < MAXPreserver; j++)
                FxMatrix[i][j] = -1;


    
    //For drawing background
    if (ofGetFrameNum() % 2)
    {
        indx++;
        if (indx > gifloader.pages.size()-1) indx = 0;
    }
    
    //Atoms Code
    for (int i = 0; i < n_Creator; i++){
	if(LifeCreator[i])
	CAtom[i]->update();
    }
    
    for (int i = 0; i < n_Preserver; i++)
        PAtom[i]->update();
    
    for (int i = 0; i < n_Destroyer; i++)
        DAtom[i]->update();
    
    //COLLISION CHECK

    //Checking for Atoms Collisions
    //Creator
    for(int j = 0; j < n_Creator; j++) {
        for(int i = 0; i < n_Preserver; i++){
	    
	    if(LifeCreator[j])
            isCollidedPreserver[j][i] = CAtom[j]->collide(PAtom[i]);
        }

	// Destruction Collisions
        for(int i = 0; i < n_Destroyer; i++){

	    if(LifeCreator[j])
            isCollidedDestroyer[j][i] = CAtom[j]->collide(DAtom[i]);
	    
	    if(isCollidedDestroyer[j][i] == 2)
		Destroy(CAtom[j]);

        }
        for(int i = 0; i < n_Creator; i++){
            //TODO: Check for distance from other creator atoms
            
	    if(LifeCreator[j] && LifeCreator[i])
	    if(i!=j){
              	CAtom[j]->collide(CAtom[i]);
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


    // EFFECTS MATRIX 

    for(int j = 0; j < n_Creator; j++){
	for(int i = 0; i < n_Preserver; i++){

	    if(LifeCreator[j])
            FxMatrix[j][i] = CAtom[j]->atomFxDist(PAtom[i]);
	}
    }

    // Debugging Display
/*
    for(int j = 0; j < MAXCreator; j++){
	for(int i = 0; i < MAXPreserver; i++){
	   cout<<FxMatrix[j][i]<<" ";
	}
    cout<< endl;
    }
   cout<<" Frame Khatm "<<endl;
*/
    for(int i = 0; i < MAXCreator; i++)
    cout<<" "<<LifeCreator[i]<<" ";
    cout<<endl;

    //OSC code
    
    // Collisions and Fx
    ofxOscMessage m1,m2,m3,m4;
    m1.setAddress("/collisionType");
    m2.setAddress("/collisionType");
    m3.setAddress("/FxMatrix"); // TODO: Bundle entire Matrix as OSC msg
    
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
    
    m3.addIntArg(FxMatrix[0][0]);
    sender.sendMessage(m3,false);
    
 //   m4.addIntArg(FxMatrix[1][0]);
 //   sender.sendMessage(m4,false);
    
}

void ofApp::draw() {
    ofSetColor(255);
    ofBackground(0);
    
    //Uncomment the next line for viewing the camera input
    //cam.draw(0, 0);
    //cam.draw(cam.getWidth(),0,-cam.getWidth(),cam.getHeight());  //For laterally inverting cam video
    
    //For drawing background: the image size is 498*480
    //Comment the next line for viewing the camera input
    gifloader.pages[indx].draw(391, 120);
    
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

        if(LifeCreator[i])
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


void ofApp::keyPressed(int key){

    switch(key){

	case 'q':
        case 'Q': 
		for(int i = 0; i < n_Creator; i++){
			if(LifeCreator[i])
			delete [] CAtom[i];
		} 
                for(int i = 0; i < n_Preserver; i++){
                        delete [] PAtom[i];
                }
                for(int i = 0; i < n_Destroyer; i++){
                        delete [] DAtom[i];
                }
		ofExit();

    }

}



//------------------------------------------------------------------------
// DESTRUCTION CODE

void ofApp::Destroy(ofAtom* Atom){

// TODO: What Destroy Graphics to use a) Shrinks b) Breaks into smaller balls
	
	LifeCreator[Atom->m_id] = 0;
        DyingCreator[Atom->m_id] = 1;
        
        // Free the memory assigned to Atom
	delete [] Atom;

}


