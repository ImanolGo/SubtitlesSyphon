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

	Manager::setup();
    
    this->setupFbo();
    this->setupSyphon();
    
    this->createTextVisuals();
    this->createSvgVisuals();
    this->createImageVisuals();
    
    this->addVisuals();
    
    ofLogNotice() <<"LayoutManager::initialized";

}


void LayoutManager::setupFbo()
{
    int margin = MARGIN;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    float ratio = width/ height;
    
    m_windowRect.width = ofGetWidth() - AppManager::getInstance().getGuiManager().getWidth() - 4*margin;
    m_windowRect.height =  m_windowRect.width / ratio;
    
    m_windowRect.x = 3*margin + AppManager::getInstance().getGuiManager().getWidth() ;
    m_windowRect.y = ofGetHeight()*0.5 - m_windowRect.height*0.5;
    
    m_fbo.allocate(width, height, GL_RGBA);
    m_fbo.begin(); ofClear(0,0,0,0); m_fbo.end();
    
    //m_fbo.getTexture().getTextureData().bFlipTexture = true;
 
}

void LayoutManager::setupSyphon()
{
    string name = AppManager::getInstance().getSettingsManager().getSyphonName();
    m_syphonServer.setName(name);
    
    ofLogNotice() <<"SyphonManager::setupSyphon << Setting up Syphon server: " << name;
}

void LayoutManager::update()
{
    // m_scrollView.update();
    
    this->updateSyphonTexture();
}


void LayoutManager::updateSyphonTexture()
{
    m_syphonServer.publishFBO(&m_fbo);
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

void LayoutManager::draw()
{
    
    ofEnableAlphaBlending();
    m_fbo.begin();
    ofPushStyle();
        ofClear(255, 0, 0);
    
        AppManager::getInstance().getTextManager().draw();
    
    ofPopStyle();
    m_fbo.end();
    ofDisableAlphaBlending();
    
    m_fbo.draw(m_windowRect.x,m_windowRect.y,m_windowRect.width,m_windowRect.height);
    
}

void LayoutManager::windowResized(int w, int h)
{
    int margin =  LayoutManager::MARGIN;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    float ratio = width/ height;
    
    m_windowRect.width = w - AppManager::getInstance().getGuiManager().getWidth() - 4*margin;
    m_windowRect.height =  m_windowRect.width / ratio;
    
    m_windowRect.x = 3*margin + AppManager::getInstance().getGuiManager().getWidth() ;
    m_windowRect.y = h*0.5 - m_windowRect.height*0.5;

}



