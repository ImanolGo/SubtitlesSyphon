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
const string LayoutManager::LAYOUT_FONT =  "fonts/open-sans/OpenSans-Semibold.ttf";
const string LayoutManager::LAYOUT_FONT_LIGHT =  "fonts/open-sans/OpenSans-Light.ttf";

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
    this->setupWindowFrames();
    
    this->createTextVisuals();
    this->createSvgVisuals();
    this->createImageVisuals();
    
    //this->addVisuals();
    
    ofLogNotice() <<"LayoutManager::initialized";

}


void LayoutManager::setupFbo()
{
    int margin = MARGIN;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    
    m_currentFbo.allocate(width, height, GL_RGBA);
    m_currentFbo.begin(); ofClear(0); m_currentFbo.end();
    
    m_previewFbo.allocate(width, height, GL_RGBA);
    m_previewFbo.begin(); ofClear(0); m_previewFbo.end();
 
}

void LayoutManager::resetWindowRects()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    float ratio = width/ height;
    float frame_width = ofGetWidth() - AppManager::getInstance().getGuiManager().getWidth() - 2*MARGIN;
    
    if(frame_width >= ofGetHeight())
    {
        m_currentWindowRect.width = 3*frame_width/5 - 2*MARGIN;
        m_currentWindowRect.height =  m_currentWindowRect.width / ratio;
        
        m_previewWindowRect.width = 2*frame_width/5 - 2*MARGIN;
        m_previewWindowRect.height = m_previewWindowRect.width / ratio;
        
        m_currentWindowRect.x = AppManager::getInstance().getGuiManager().getWidth()  + 3*MARGIN;
        m_currentWindowRect.y = ofGetHeight()*0.5 - m_currentWindowRect.height/2;
        
        m_previewWindowRect.x = m_currentWindowRect.x + 2*MARGIN + m_currentWindowRect.width;
        m_previewWindowRect.y =  ofGetHeight()*0.5 - m_previewWindowRect.height/2;
    }
    else
    {
        m_currentWindowRect.width = frame_width - 2*MARGIN;
        m_currentWindowRect.height =  m_currentWindowRect.width / ratio;
        
        m_previewWindowRect.width = 3*m_currentWindowRect.width/4;
        m_previewWindowRect.height = m_previewWindowRect.width / ratio;
        
        m_currentWindowRect.x = AppManager::getInstance().getGuiManager().getWidth()  + 3*MARGIN;
        m_currentWindowRect.y = m_liveRectangle.getHeight() + 2*MARGIN + m_textVisuals["CurrentSubtitle"]->getHeight();
        
        m_previewWindowRect.x = m_currentWindowRect.x;
        m_previewWindowRect.y = m_currentWindowRect.y + m_currentWindowRect.height + 2*MARGIN  + m_textVisuals["NextSubtitle"]->getHeight();
    }

}
void LayoutManager::setupWindowFrames()
{
    this->resetWindowRects();
    this->resetWindowFrames();
    
    float width = ofGetScreenWidth();
    float height = ofGetScreenHeight()/80;
    
    m_liveRectangle.setWidth(width); m_liveRectangle.setHeight(height);
    
    auto color = AppManager::getInstance().getSettingsManager().getColor("LiveRectangle");
    m_liveRectangle.setColor(color);
    
    color = AppManager::getInstance().getSettingsManager().getColor("FrameRectangle");
    m_currentWindowFrame.setColor(color);  m_previewWindowFrame.setColor(color);
    
    
  
}

void LayoutManager::resetWindowFrames()
{
    m_currentWindowFrame.setPosition(ofPoint( m_currentWindowRect.x - FRAME_MARGIN, m_currentWindowRect.y - FRAME_MARGIN, 0));
    m_currentWindowFrame.setWidth(m_currentWindowRect.width + 2*FRAME_MARGIN); m_currentWindowFrame.setHeight(m_currentWindowRect.height + 2*FRAME_MARGIN);
    
    m_previewWindowFrame.setPosition(ofPoint( m_previewWindowRect.x - FRAME_MARGIN, m_previewWindowRect.y - FRAME_MARGIN, 0));
    m_previewWindowFrame.setWidth(m_previewWindowRect.width + 2*FRAME_MARGIN); m_previewWindowFrame.setHeight(m_previewWindowRect.height + 2*FRAME_MARGIN);
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
        m_syphonServer.publishTexture(&m_currentFbo.getTexture());
    }
}


void LayoutManager::createTextVisuals()
{
    float size = 20;
    float w = m_currentWindowRect.width;
    float h = size;
    float x =  m_currentWindowRect.x + m_currentWindowRect.getWidth()*0.5;
    float y =  m_currentWindowRect.y - h - 2*MARGIN;
    ofPoint pos = ofPoint(x, y);
    string text = "Current Subtitle";
    string fontName = LAYOUT_FONT_LIGHT;
    

    auto textVisual = ofPtr<TextVisual>(new TextVisual(pos,w,h,true));
    textVisual->setText(text, fontName, size, ofColor::white);
    m_textVisuals["CurrentSubtitle"] = textVisual;
    
    
    w = m_previewWindowRect.width;
    x =  m_previewWindowRect.x + m_previewWindowRect.getWidth()*0.5;
    y =  m_previewWindowRect.y - h - 2*MARGIN;
    text = "Next Subtitle";
    textVisual = ofPtr<TextVisual>(new TextVisual(pos,w,h,true));
    textVisual->setText(text, fontName, size, ofColor::white);
    m_textVisuals["NextSubtitle"] = textVisual;
    
}


void LayoutManager::resetWindowTitles()
{
    float x =  m_currentWindowRect.x + m_currentWindowRect.getWidth()*0.5;
    float y =  m_currentWindowRect.y -  m_textVisuals["CurrentSubtitle"]->getHeight()*0.5 - MARGIN;
    ofPoint pos = ofPoint(x, y);
    m_textVisuals["CurrentSubtitle"]->setPosition(pos);
    
    
    pos.x =  m_previewWindowRect.x + m_previewWindowRect.getWidth()*0.5;
    pos.y =  m_previewWindowRect.y - m_textVisuals["NextSubtitle"]->getHeight()*0.5  - MARGIN;
    m_textVisuals["NextSubtitle"]->setPosition(pos);
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
    
    this->drawFbos();
    this->drawText();
    this->drawRectangles();
}


void LayoutManager::drawText()
{
    for(auto textVisual: m_textVisuals){
        textVisual.second->draw();
    }
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
         ofClear(0, 0, 0);
        if(m_syphonToggle)
        {
            AppManager::getInstance().getTextManager().draw();
        }
    
    m_currentFbo.end();
    ofDisableAlphaBlending();
    
    m_currentWindowFrame.draw();
    m_currentFbo.draw(m_currentWindowRect.x,m_currentWindowRect.y,m_currentWindowRect.width,m_currentWindowRect.height);
   
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
    
    m_previewWindowFrame.draw();
    m_previewFbo.draw(m_previewWindowRect.x,m_previewWindowRect.y,m_previewWindowRect.width,m_previewWindowRect.height);
    
}

void LayoutManager::windowResized(int w, int h)
{
    if(!m_initialized){
        return;
    }
    
    this->resetWindowRects();
    this->resetWindowFrames();
    this->resetWindowTitles();
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



