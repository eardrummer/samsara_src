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
	
	// 1 = currently dying, 0 = not dying
        int m_status;

	//constructor
        ofDyingAtom(int posX, int posY, int velX, int velY, int status);
	
	void assign(int posX, int posY, int velX, int velY, int status, int counter);
 	void display();   
	int statusCheck();
	void statusAssign(int status);
};

#endif
