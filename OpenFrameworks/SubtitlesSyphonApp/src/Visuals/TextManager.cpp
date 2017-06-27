/*
 *  StageManager.cpp
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */



#include "ofMain.h"

#include "StageManager.h"
#include "AppManager.h"


StageManager::StageManager(): Manager(), m_trail(false), m_liquify(false), m_brightnessEffect(false), m_frequency(0.6), m_amplitude(30.0), m_speed(0.5)
{
    //Intentionally left empty
}


StageManager::~StageManager()
{
    ofLogNotice() <<"StageManager::Destructor" ;
}

void StageManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    
    //this->setupImages();
    this->setupFbo();
    this->setupShader();
    //this->setupScrollView();
    this->setupSyphon();
    
    ofLogNotice() <<"StageManager::initialized" ;
    
}

void StageManager::setupFbo()
{
    int margin =  LayoutManager::MARGIN;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    float ratio = width/ height;
    
    m_windowRect.width = ofGetWidth() - AppManager::getInstance().getGuiManager().getWidth() - 4*margin;
    m_windowRect.height =  m_windowRect.width / ratio;
    
    m_windowRect.x = 3*margin + AppManager::getInstance().getGuiManager().getWidth() ;
    m_windowRect.y = ofGetHeight()*0.5 - m_windowRect.height*0.5;
    
    m_fbo.allocate(width, height, GL_RGBA);
    m_fbo.begin(); ofClear(0,0,0,0); m_fbo.end();
    
    m_syphonFbo.allocate(width, height, GL_RGB);
    m_syphonFbo.begin(); ofClear(255,0,0); m_syphonFbo.end();
    
    m_fbo.getTexture().getTextureData().bFlipTexture = true;
    m_syphonFbo.getTexture().getTextureData().bFlipTexture = true; 
}

void StageManager::setupShader()
{
    m_liquifyShader.load("shaders/shadersGL3/LiquifyShader.vert", "shaders/shadersGL3/LiquifyShader.frag" );
    
    m_frequency = 0.6;
    m_amplitude = 40.0;
    m_speed = 1.0;
}

void StageManager::setupImages()
{
    string resourceName = "BlackBackground";
    ofPoint position;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_costumeImage =  ofPtr<ImageVisual> (new ImageVisual(position,resourceName));
    m_costumeImage->setWidth(width, false);
    m_costumeImage->setHeight(height, false);
    //m_costumeImage->setHeight(700,true);
}

void StageManager::setupSyphon()
{
    string name = AppManager::getInstance().getSettingsManager().getSyphonName();
    m_syphonServer.setName(name);
    
    ofLogNotice() <<"VideoOutputManager::setupSyphon << Setting up Syphon server: " << name;
}

void StageManager::update()
{
   // m_scrollView.update();
    
    this->updateSyphonTexture();
}


void StageManager::updateSyphonTexture()
{
    m_syphonServer.publishFBO(&m_syphonFbo);
}


void StageManager::draw()
{
    
    ofEnableAlphaBlending();
    m_fbo.begin();
    ofPushStyle();
    //ofClear(0, 0, 0);
    
    
    if(m_trail){
        ofFill();
        //ofSetColor(0,0,0, 5);
        //ofDrawRectangle(0,0,m_fbo.getWidth(),m_fbo.getHeight());
        ofSetColor(255, 30);
    }
    else{
        ofSetColor(255);
    }
    
        AppManager::getInstance().getSceneManager().draw();
    
    ofPopStyle();
    m_fbo.end();
    ofDisableAlphaBlending();
    
    
    m_syphonFbo.begin();
    if(m_liquify){
        m_liquifyShader.begin();
        float time = ofGetElapsedTimef();
        float energy = 1.3*AppManager::getInstance().getAudioManager().getBrightness();
        m_liquifyShader.setUniform1f( "time", time );	//Passing float parameter "time" to shader
        m_liquifyShader.setUniform1f("frequency", m_frequency);
        m_liquifyShader.setUniform1f("amplitude", m_amplitude*energy);
        m_liquifyShader.setUniform1f("speed", m_speed*energy);
    }
    
    int brightness = 255;
    if(m_brightnessEffect){
        float volume = AppManager::getInstance().getAudioManager().getBrightness();
        brightness  = ofMap(volume, 0.0, 0.5, 0, 255, true);
        //brightness  = ofClamp(brightness,0, 255);
    }
    
     ofSetColor(brightness);
     m_fbo.draw(0,0);
    
     if(m_liquify){
         m_liquifyShader.end();
     }
    
    m_syphonFbo.end();
    
    m_syphonFbo.draw(m_windowRect.x,m_windowRect.y,m_windowRect.width,m_windowRect.height);

}

void  StageManager::windowResized(int w, int h)
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


