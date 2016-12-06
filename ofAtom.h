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
    int m_type; // Type of atom. 0 - Constuctor, 1 - Preserver/Fx, 2 - Destroyer
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
    void assign(ofAtom* Atom, int type, int id, int posX, int posY, int velocityX, int velocityY);
    float atomFxDist(ofAtom** PAtom, int n_Preserver);		//Calculates distance from closest Fx Atom (Preserver Atom)
    int atomFxId(ofAtom** PAtom, int n_Preserver);
    int death(ofAtom* Atom);

};

#endif
