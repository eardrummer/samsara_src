/*
*  This is part of the Samsara Project, by Manaswi, Siddharth and Kushagra - SMC, UPF 2016
*  No part of this code may be used(directly or modified) without contacting the authors
*
*  This is an experimental physics based virtual sound system project and we hope to develop it further and release it in the future
*  29/01/2016
*
*    Contact : manaswimishra17@gmail.com
*/


#ifndef _OF_ATOM
#define _OF_ATOM

#include "ofMain.h"

class ofAtom{
    
public:
    void setup();
    void update();
    void draw();
    
    //Constructor
    
    ofAtom(int type, int id, int posX, int posY, int radius);
    
    //Destructor TODO:
    
    //variables
    int m_type; // Type of atom. 0 - Constuctor, 1 - Preserver/Fx, 2 - Destroyer, 3 - Virtual, (4,5,6) - Frozen C, P and D
    int m_id;
    int m_velocityX;
    int m_velocityY;
    int m_posX;
    int m_posY;
    int m_radius;
    int m_mass;
    
    //Colliding function
    int collide(ofAtom* nearAtom);		//nearAtom is the Atom nearest to this.ofAtom
    void assignVelocity(int velocityX, int velocityY); 
    void assign(int type, int id, int posX, int posY, int velocityX, int velocityY, int radius);
    void assign(int type, int id, int posX, int posY, int radius);
    float atomFxDist(ofAtom* PAtom);		//Calculates distance from closest Fx Atom (Preserver Atom)

};

#endif
