/*
 *  PasswordManager.cpp
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 01/08/17
 *
 */

#include "PasswordManager.h"
#include "AppManager.h"
#include "PasswordInputTheme.h"


const string PasswordManager::KEY = "willemjantjes";

PasswordManager::PasswordManager(): Manager(), m_allowApp(true), m_textField(NULL)
{
    //Intentionally left empty
}

PasswordManager::~PasswordManager()
{
   ofLogNotice() << "PasswordManager::destructor";
}


//--------------------------------------------------------------

void PasswordManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    this->setupInput();
    
    ofLogNotice() <<"PasswordManager::initialized" ;
}

void PasswordManager::setupInput()
{
    m_textField = ofPtr<TextField>(new TextField(ofVec2f(0,0),false));

    m_allowApp = !AppManager::getInstance().getSettingsManager().getNeedsPassword();
    ofLogNotice() <<"PasswordManager::setupInput -> Allow password: " <<  m_allowApp;
    
    m_fbo.allocate( m_textField->getWidth(),  m_textField->getHeight());
    m_fbo.begin(); ofClear(0); m_fbo.end();
    
    m_boudingBox.width = ofGetWidth() - LayoutManager::MARGIN*4;
    float ratio = m_textField->getHeight()/ m_textField->getWidth();
    m_boudingBox.height = m_boudingBox.width*ratio;
    
    m_boudingBox.x = ofGetWidth()*0.5 -  m_boudingBox.width*0.5;
    m_boudingBox.y = ofGetHeight()*0.5 -  m_boudingBox.height*0.5;
}



void PasswordManager::update()
{
    if(m_allowApp){
        return;
    }
    
    if(!m_textField){
        return;
    }
    
    ofEnableAlphaBlending();
    m_fbo.begin();
    ofClear(0);
         m_textField->draw();
    m_fbo.end();
    ofDisableAlphaBlending();
    
}

void PasswordManager::draw()
{
    if(m_allowApp){
        return;
    }
    
    if(!m_textField){
        return;
    }
    
    m_fbo.draw(m_boudingBox);
    //m_textField->draw();
    
}

void PasswordManager::enterPassword(const string& password)
{
    ofLogNotice() <<"PasswordManager::enterPassword: " <<  password;

    if(password == KEY){
        m_allowApp = true;
        AppManager::getInstance().getLayoutManager().setFullScreen();
        AppManager::getInstance().getGuiManager().loadGuiValues();
    }

}

void PasswordManager::windowResized(int w, int h)
{
    if(!m_textField){
        return;
    }
    
    m_boudingBox.width = ofGetWidth() - LayoutManager::MARGIN*4;
    float ratio = m_textField->getHeight()/ m_textField->getWidth();
    m_boudingBox.height = m_boudingBox.width*ratio;
    
    m_boudingBox.x = ofGetWidth()*0.5 -  m_boudingBox.width*0.5;
    m_boudingBox.y = ofGetHeight()*0.5 -  m_boudingBox.height*0.5;
}

