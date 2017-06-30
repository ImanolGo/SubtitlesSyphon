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
const int LayoutManager::FRAME_MARGIN = 2;

LayoutManager::LayoutManager(): Manager(), m_syphonToggle(true), m_syphonEnable(true)
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
    this->setupRectangle();
    
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
    
    float frame_width = ofGetWidth() - AppManager::getInstance().getGuiManager().getWidth() - 2*margin;
    
    m_currentWindowRect.width = 3*frame_width/5 - 2*margin;
    m_currentWindowRect.height =  m_currentWindowRect.width / ratio;
    
    m_previewWindowRect.width = 2*frame_width/5 - 2*margin;
    m_previewWindowRect.height = m_previewWindowRect.width / ratio;
    
    m_currentWindowRect.x = AppManager::getInstance().getGuiManager().getWidth()  + 3*margin;
    m_currentWindowRect.y = ofGetHeight()/2 - m_currentWindowRect.height/2;
    
    m_previewWindowRect.x = m_currentWindowRect.x + 2*margin + m_currentWindowRect.width;
    m_previewWindowRect.y = ofGetHeight()/2 - m_previewWindowRect.height/2;
    
    m_currentFbo.allocate(width, height, GL_RGBA);
    m_currentFbo.begin(); ofClear(0,0,0,0); m_currentFbo.end();
    
    m_previewFbo.allocate(width, height, GL_RGBA);
    m_previewFbo.begin(); ofClear(0,0,0,0); m_previewFbo.end();
    
    m_syphonFbo.allocate(width, height, GL_RGBA);
    m_syphonFbo.begin(); ofClear(0,0,0,0); m_syphonFbo.end();
    
    m_currentFbo.getTexture().getTextureData().bFlipTexture = true;
    m_syphonFbo.getTexture().getTextureData().bFlipTexture = true;
 
}

void LayoutManager::setupRectangle()
{
    float width = ofGetScreenWidth();
    float height = ofGetScreenHeight()/80;
    
    m_liveRectangle.setWidth(width); m_liveRectangle.setHeight(height);
    
    auto color = AppManager::getInstance().getSettingsManager().getColor("LiveRectangle");
    m_liveRectangle.setColor(color);
    
    m_currentFboRectangle.setPosition(ofPoint( m_currentWindowRect.x - FRAME_MARGIN, m_currentWindowRect.y - FRAME_MARGIN, 0));
    m_currentFboRectangle.setWidth(m_currentWindowRect.width + 2*FRAME_MARGIN); m_currentFboRectangle.setHeight(m_currentWindowRect.height + 2*FRAME_MARGIN);
    color = AppManager::getInstance().getSettingsManager().getColor("FrameRectangle");
    m_currentFboRectangle.setColor(color);
    
    
    m_previewFboRectangle.setPosition(ofPoint( m_previewWindowRect.x - FRAME_MARGIN, m_previewWindowRect.y - FRAME_MARGIN, 0));
    m_previewFboRectangle.setWidth(m_previewWindowRect.width + 2*FRAME_MARGIN); m_previewFboRectangle.setHeight(m_previewWindowRect.height + 2*FRAME_MARGIN);
    m_previewFboRectangle.setColor(color);
}

void LayoutManager::setupSyphon()
{
    string name = AppManager::getInstance().getSettingsManager().getSyphonName();
    m_syphonServer.setName(name);
    
    ofLogNotice() <<"SyphonManager::setupSyphon << Setting up Syphon server: " << name;
}

void LayoutManager::update()
{
    if(!m_initialized)
        return;
    
    
    this->updateSyphonTexture();
}


void LayoutManager::updateSyphonTexture()
{
    if(m_syphonEnable){
        m_syphonServer.publishFBO(&m_syphonFbo);
    }
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
    if(!m_initialized)
        return;
    
    //AppManager::getInstance().getTextManager().drawCurrentVisuals();
    this->drawFbos();
    this->drawRectangles();
}

void LayoutManager::drawRectangles()
{
    if(m_syphonEnable){
        m_liveRectangle.draw();
    }
}


void LayoutManager::drawFbos()
{
    this->drawCurrentFbo();
    this->drawPreviewFbo();

}

void LayoutManager::drawCurrentFbo()
{
    
    ofEnableAlphaBlending();
    m_currentFbo.begin();
        ofPushStyle();
        ofClear(0, 0, 0);
    
            AppManager::getInstance().getTextManager().draw();
    
    ofPopStyle();
    m_currentFbo.end();
    ofDisableAlphaBlending();
    
    m_syphonFbo.begin();
        ofClear(0);
        if(m_syphonToggle){
            m_currentFbo.draw(0,0);
        }
    m_syphonFbo.end();
    
    m_currentFboRectangle.draw();
    m_syphonFbo.draw(m_currentWindowRect.x,m_currentWindowRect.y,m_currentWindowRect.width,m_currentWindowRect.height);
   
}

void LayoutManager::drawPreviewFbo()
{
    
    ofEnableAlphaBlending();
    m_previewFbo.begin();
    ofPushStyle();
    ofClear(0, 0, 0);
    
    AppManager::getInstance().getTextManager().drawPreviousVisuals();
    
    ofPopStyle();
    m_previewFbo.end();
    ofDisableAlphaBlending();
    
    m_previewFboRectangle.draw();
    m_previewFbo.draw(m_previewWindowRect.x,m_previewWindowRect.y,m_previewWindowRect.width,m_previewWindowRect.height);
    
}

void LayoutManager::windowResized(int w, int h)
{
    int margin =  LayoutManager::MARGIN;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    float ratio = width/ height;
    float frame_width = w - AppManager::getInstance().getGuiManager().getWidth() - 2*margin;
    
    if(frame_width >= h)
    {
        m_currentWindowRect.width = 3*frame_width/5 - 2*margin;
        m_currentWindowRect.height =  m_currentWindowRect.width / ratio;
        
        m_previewWindowRect.width = 2*frame_width/5 - 2*margin;
        m_previewWindowRect.height = m_previewWindowRect.width / ratio;
        
        m_currentWindowRect.x = AppManager::getInstance().getGuiManager().getWidth()  + 3*margin;
        m_currentWindowRect.y = h/2 - m_currentWindowRect.height/2;
        
        m_previewWindowRect.x = m_currentWindowRect.x + 2*margin + m_currentWindowRect.width;
        m_previewWindowRect.y = h/2 - m_previewWindowRect.height/2;
    }
    else
    {
        m_currentWindowRect.width = frame_width - 2*margin;
        m_currentWindowRect.height =  m_currentWindowRect.width / ratio;
        
        m_previewWindowRect.width = 3*m_currentWindowRect.width/4;
        m_previewWindowRect.height = m_previewWindowRect.width / ratio;
        
        m_currentWindowRect.x = AppManager::getInstance().getGuiManager().getWidth()  + 3*margin;
        m_currentWindowRect.y = m_liveRectangle.getHeight() + 2*margin;
        
        m_previewWindowRect.x = m_currentWindowRect.x;
        m_previewWindowRect.y = m_currentWindowRect.y + m_currentWindowRect.height + 2*margin;
    }
    
    
    
    
    
    
    
    
    
    m_currentFboRectangle.setPosition(ofPoint( m_currentWindowRect.x - FRAME_MARGIN, m_currentWindowRect.y - FRAME_MARGIN, 0));
    m_currentFboRectangle.setWidth(m_currentWindowRect.width + 2*FRAME_MARGIN); m_currentFboRectangle.setHeight(m_currentWindowRect.height + 2*FRAME_MARGIN);
    
    m_previewFboRectangle.setPosition(ofPoint( m_previewWindowRect.x - FRAME_MARGIN, m_previewWindowRect.y - FRAME_MARGIN, 0));
    m_previewFboRectangle.setWidth(m_previewWindowRect.width + 2*FRAME_MARGIN); m_previewFboRectangle.setHeight(m_previewWindowRect.height + 2*FRAME_MARGIN);

}

void LayoutManager::onSyphonEnable(bool value)
{
    m_syphonEnable = value;
    if(m_syphonEnable){
        string name = AppManager::getInstance().getSettingsManager().getSyphonName();
        m_syphonServer.setName(name);
    }
    else{
        m_syphonServer.exit();
    }
}



