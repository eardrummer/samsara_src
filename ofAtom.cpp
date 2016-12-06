#include "ofAtom.h"


//-----------------------------------------------------------------------------
ofAtom::ofAtom(int type, int id, int posX , int posY, int radius){
    
    m_type = type;  // See ofAtom.h for type
    m_id = id;
    m_posX = posX;
    m_posY = posY;
    m_radius = radius;
    
    m_velocityX = ofRandom(-7, 7);
    m_velocityY = ofRandom(-7, 7);
   
    if(m_velocityX == 0)
	m_velocityX = 1;
    if(m_velocityY == 0)
	m_velocityY = 1; 
    
    // Give different Mass to each type of atom
    m_mass = 5;
    
    
    //Updating global counters for the number of Atoms in the Samsara
    // TODO: Update n_Creator, n_Preserver, n_Destroyer of global scope
    /*
     if (type == 0){
     n_Creator++
     }
     else if(type == 1){
     n_Preserver++
     }
     else if(type == 2){
     n_Destroyer++
     }
     */  
    
}


//-------------------------------------------------------------------------------
void ofAtom::update(){
    
    m_posX = m_posX + (m_velocityX);
    m_posY = m_posY + (m_velocityY);
    
    if (m_posX < m_radius || m_posX > ofGetWidth() - m_radius) {
        m_velocityX = -1 * m_velocityX;

    }
    
    if (m_posY < m_radius || m_posY > ofGetHeight() - m_radius) {
        m_velocityY = -1 * m_velocityY;
    }
    
    if (m_posX < m_radius)
        m_posX = m_radius;
    if (m_posY < m_radius)
        m_posY = m_radius;
    if (m_posX > ofGetWidth() - m_radius)
        m_posX = ofGetWidth() - m_radius;
    if (m_posY > ofGetHeight() - m_radius)
        m_posY = ofGetHeight() - m_radius;
    
}

//--------------------------------------------------------------------------------
void ofAtom::draw(){
    
    ofFill();
    
    if(m_type == 0)
        ofSetColor(255,255,0);
    else if(m_type == 1)
        ofSetColor(150,255,0);
    else
        ofSetColor(255,150,0);
    
    ofDrawCircle(m_posX, m_posY, m_radius);
    
}

//--------------------------------------------------------------------------------
int ofAtom::collide(ofAtom* nearAtom){
    
    
    if(ofDist(nearAtom->m_posX, nearAtom->m_posY, m_posX, m_posY) < nearAtom->m_radius + m_radius) {
         
         
         float newVelX1 = (m_velocityX * (m_mass - nearAtom->m_mass) + (2 * nearAtom->m_mass * nearAtom->m_velocityX)) / float(m_mass + nearAtom->m_mass);
         float newVelY1 = (m_velocityY * (m_mass - nearAtom->m_mass) + (2 * nearAtom->m_mass * nearAtom->m_velocityY)) / float(m_mass + nearAtom->m_mass);
         float newVelX2 = (nearAtom->m_velocityX * (nearAtom->m_mass - m_mass) + (2 * m_mass * m_velocityX)) / float(m_mass + nearAtom->m_mass);
         float newVelY2 = (nearAtom->m_velocityY * (nearAtom->m_mass - m_mass) + (2 * m_mass * m_velocityY)) / float(m_mass + nearAtom->m_mass);
         
         
         m_velocityX = newVelX1;
         m_velocityY = newVelY1;
         nearAtom->m_velocityX = newVelX2;
         nearAtom->m_velocityY = newVelY2;
         
        // cout<<"VELOCITY"<<" "<<m_velocityX<<" "<<m_velocityY<<endl;
         
         
         this->update();
         nearAtom->update();
         // To check if one atom is overlapping another atom
         while(ofDist(nearAtom->m_posX, nearAtom->m_posY, m_posX, m_posY) < nearAtom->m_radius + m_radius){

		m_posX += ofSign(m_velocityX);
		m_posY += ofSign(m_velocityY);

	}  
         
         
        
       
        //MESSAGE AND RETURN TO INDICATE TYPE OF COLLISION:
        if (m_type == 0 && nearAtom->m_type == 1) {
        //    	    cout<<" EFFECTS Collision:"<<m_type<<" "<<m_posX<<","<<m_posY<<" "<<nearAtom->m_type<<" "<<nearAtom->m_posX<<","<<nearAtom->m_posY<<endl;
            return 1;	    
        }
        else if (m_type == 0 && nearAtom->m_type == 2) {
        //    	    cout<<" DESTROY Collision:"<<m_type<<" "<<m_posX<<","<<m_posY<<" "<<nearAtom->m_type<<" "<<nearAtom->m_posX<<","<<nearAtom->m_posY<<endl;  
            return 2;
        }
        
    }
    
    
    return 0;   //No Collision occured
}

//-----------------------------------------------------------------------------------------
void ofAtom::assignVelocity(int velocityX, int velocityY){

	//m_velocityX = ofClamp(velocityX,-7,7);
        //m_velocityY = ofClamp(velocityY,-7,7);
        
        cout<<"VELOCITY"<<velocityX<<" "<<velocityY;
	
	m_velocityX = velocityX;
	m_velocityY = velocityY;

	if(velocityX > 7)
		m_velocityX = 7;
	else if(velocityX < -7)
		m_velocityX = -7;

	if(velocityY > 7)
		m_velocityY = 7;
	else if(velocityY < -7)
		m_velocityY = -7;

	if(velocityX == 0)
		m_velocityX = 1;

	if(velocityY == 0)
		m_velocityY = 1;


	//update();

}


//---------------------------------------------------------------------------------------

void assign(ofAtom* Atom, int type, int id, int posX, int posY, int velocityX, int velocityY){

	Atom->m_type = type;
	Atom->m_id = id;
	Atom->m_posX = posX;
	Atom->m_posY = posY;
	Atom->m_velocityX = velocityX;
	Atom->m_velocityY = velocityY;

	//Atom->update();
}

//----------------------------------------------------------------------------------------
float ofAtom::atomFxDist(ofAtom* PAtom){

	float Distance;
	Distance = ofDist(PAtom->m_posX, PAtom->m_posY, m_posX, m_posY);
	
	return Distance;
}

//---------------------------------------------------------------------------------------------

int death(ofAtom* Atom){

	

}

//----------------------------------------------------------------------------------------------
//void ofAtom::~ofAtom(){


//	TODO:delete this 


//}




