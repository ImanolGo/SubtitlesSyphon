/*
 *  MouseManager.cpp
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */

#include "MouseManager.h"
#include "AppManager.h"


MouseManager::MouseManager(): Manager()
{
    //Intentionally left empty
}

MouseManager::~MouseManager()
{
   ofLogNotice() << "MouseManager::destructor";
}


//--------------------------------------------------------------

void MouseManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"MouseManager::initialized" ;
    this->addListeners();
}

void MouseManager::addListeners()
{
    ofRegisterMouseEvents(this); // this will enable the MouseManager class to listen to the mouse events.
}



void MouseManager::mousePressed(ofMouseEventArgs &e)
{
    int button = e.button;
    float x = e.x;
    float y = e.y;
    
    ofLogVerbose() <<"MouseManager::mousePressed-> button: " + ofToString(button) << ", x = " << x << ", y = " << y ;
    
}

void MouseManager::mouseReleased(ofMouseEventArgs &e)
{
    int button = e.button;
    float x = e.x;
    float y = e.y;

    
    ofLogVerbose() <<"MouseManager::mousePressed-> button: " + ofToString(button) << ", x = " << x << ", y = " << y ;
    
}

void MouseManager::mouseMoved(ofMouseEventArgs &e)
{
    // Intentionally left empty
    
}


void MouseManager::mouseDragged(ofMouseEventArgs &e)
{
   // Intentionally left empty
    
}


void MouseManager::mouseScrolled(ofMouseEventArgs &e)
{
    // Intentionally left empty
    
}

void MouseManager::mouseEntered(ofMouseEventArgs &e)
{
    // Intentionally left empty
    
}

void MouseManager::mouseExited(ofMouseEventArgs &e)
{
    // Intentionally left empty
    
}








