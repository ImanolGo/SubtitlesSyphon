/*
 *  LayoutManager.cpp
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */

#include "ofMain.h"

#include "AppManager.h"
#include "SettingsManager.h"
#include "ViewManager.h"


#include "LayoutManager.h"


const int LayoutManager::MARGIN = 20;

LayoutManager::LayoutManager(): Manager()
{
	//Intentionally left empty
}


LayoutManager::~LayoutManager()
{
    ofLogNotice() <<"LayoutManager::Destructor";
}


void LayoutManager::setup()
{
	if(m_initialized)
		return;

    ofLogNotice() <<"LayoutManager::initialized";

	Manager::setup();

    this->createTextVisuals();
    this->createSvgVisuals();
    this->createImageVisuals();

    this->addVisuals();

}


void LayoutManager::createTextVisuals()
{
    ///To implement in case we have text visuals
}

void LayoutManager::createSvgVisuals()
{
    ///To implement in case we have text visuals
}


void LayoutManager::createImageVisuals()
{
    //this->createBackground();
}

void LayoutManager::createBackground()
{
}

void LayoutManager::addVisuals()
{
    int depthLevel = -1;
    for(SvgMap::iterator it = m_svgVisuals.begin(); it!= m_svgVisuals.end(); ++it){
        AppManager::getInstance().getViewManager().addOverlay(it->second,depthLevel);
    }
    
    for(TextMap::iterator it = m_textVisuals.begin(); it!= m_textVisuals.end(); ++it){
        AppManager::getInstance().getViewManager().addOverlay(it->second,depthLevel);
    }
    
    for(ImageMap::iterator it = m_imageVisuals.begin(); it!= m_imageVisuals.end(); ++it){
        AppManager::getInstance().getViewManager().addOverlay(it->second,depthLevel);
    }
}


void LayoutManager::onFullScreenChange(bool value)
{
    if(value){
        ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
    }
    else{

        float width = 4*MARGIN + 2*AppManager::getInstance().getGuiManager().getWidth();
        float height = AppManager::getInstance().getGuiManager().getHeight() + 2*MARGIN;
        ofSetWindowShape(width,height);
    }
}

void LayoutManager::windowResized(int w, int h)
{
    //AppManager::getInstance().getStageManager().windowResized(w,h);

}



