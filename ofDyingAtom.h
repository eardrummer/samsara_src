/*
*  This is part of the Samsara Project, by Manaswi, Siddharth and Kushagra - SMC, UPF 2016
*  No part of this code may be used(directly or modified) without contacting the authors
*
*  This is an experimental physics based virtual sound system project and we hope to develop it further and release it in the future
*  29/01/2016
*
*    Contact : manaswimishra17@gmail.com
*/


#ifndef _OF_DyingAtom
#define _OF_DyingAtom

#include "ofMain.h"

//Dying Creator : a structure to handle a dying creator atom
class ofDyingAtom{
public:
    
    int m_posX;
    int m_posY;
    int m_velocityX;
    int m_velocityY;
    int m_dyingTimeCounter;
    int m_type;
    
    // 1 = currently dying, 0 = not dying
    int m_status;
    
    //constructor
    ofDyingAtom(int posX, int posY, int velX, int velY, int status);
    
    void assign(int posX, int posY, int velX, int velY, int status, int counter, int type);
    void display();
    int statusCheck();
    void statusAssign(int status);
};

#endif
