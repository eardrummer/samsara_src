/*
*  This is part of the Samsara Project, by Manaswi, Siddharth and Kushagra - SMC, UPF 2016
*  No part of this code may be used(directly or modified) without contacting the authors
*
*  This is an experimental physics based virtual sound system project and we hope to develop it further and release it in the future
*  29/01/2016
*
*    Contact : manaswimishra17@gmail.com
*/


#include "ofDyingAtom.h"

ofDyingAtom::ofDyingAtom(int posX, int posY, int velX, int velY, int status){
    
    if(status == 0){
        m_posX = 0;
        m_posY = 0;
        
        m_velocityX = 0;
        m_velocityY = 0;
        
        m_status = 0;
        
    }
    else if(status == 1){
        m_posX = posX;
        m_posY = posY;
        m_velocityX = velX;
        m_velocityY = velY;
        
        m_status = status;
    }
    
    m_dyingTimeCounter = 0; // during constructor creation initialize with 0
}


void ofDyingAtom::assign(int posX, int posY, int velX, int velY, int status, int counter, int type){
    
    m_posX = posX;
    m_posY = posY;
    m_velocityX = velX;
    m_velocityY = velY;
    
    m_type = type;
    m_status = status;
    m_dyingTimeCounter = counter;
    
}
void ofDyingAtom::display(){
    
    cout<<"DIE  "<<m_posX<<" "<<m_posY<<" status "<<m_status<<endl;
}

int ofDyingAtom::statusCheck(){
    
    return m_status;
}

void ofDyingAtom::statusAssign(int status){
    
    m_status = status;
}
