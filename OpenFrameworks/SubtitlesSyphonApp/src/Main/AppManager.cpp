/*
 *  AppManager.cpp
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */

#include "ofMain.h"


#include "AppManager.h"

AppManager& AppManager::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	static AppManager    m_instance;
    return m_instance;

}

AppManager::AppManager(): Manager(), m_debugMode(false), m_displayWindow(NULL)
{
   //Intentionally left empty
}


AppManager::~AppManager()
{
    ofLogNotice() <<"AppManager::Destructor";
}


void AppManager::setup()
{
	if(m_initialized)
		return;

    
    ofLogNotice() << "AppManager::initialized";

	Manager::setup();
    
    this->setupOF();
    this->setupDisplay();
	this->setupManagers();
    
    ofSetLogLevel(OF_LOG_NOTICE);
    
    //setDebugMode(m_debugMode);
}

void AppManager::setupOF()
{
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetVerticalSync(true);
    ofSetEscapeQuitsApp(false);
    ofSetDataPathRoot("../Resources/data/");
    ofEnableAntiAliasing();
    ofSetBackgroundAuto(true);
}


void AppManager::setupDisplay()
{
//    auto windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();
//    m_displayWindow =  ofCreateWindow(windowSettings.back());
//    ofAddListener(m_displayWindow->events().draw, this, &AppManager::drawDisplay);
//    m_displayWindow->setWindowShape(0, 0);
    
    auto windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();
    
    m_displayShape.x = windowSettings.back().getPosition().x;
    m_displayShape.y = windowSettings.back().getPosition().y;
    m_displayShape.width = windowSettings.back().width;
    m_displayShape.height = windowSettings.back().height;
}

void AppManager::setupManagers()
{
    m_settingsManager.setup();
    m_resourceManager.setup();
    m_viewManager.setup();
    m_visualEffectsManager.setup();
    m_layoutManager.setup();
    m_keyboardManager.setup();
    m_mouseManager.setup();
    m_passwordManager.setup();
    m_oscManager.setup();
    m_subtitlesManager.setup();
    m_textManager.setup();
    m_guiManager.setup();
    
}

void AppManager::update()
{
    if(!m_initialized)
        return;
    
    if(!m_passwordManager.getAllowApp()){
        m_passwordManager.update();
        return;
    }
    
    m_oscManager.update();
    m_visualEffectsManager.update();
    m_viewManager.update();
    m_layoutManager.update();
    m_textManager.update();
    m_guiManager.update();
}


void AppManager::draw()
{
    if(!m_initialized)
        return;
    
    auto color = AppManager::getInstance().getSettingsManager().getColor("BackgroundColor");
    ofBackground(color);
    
    
    if(!m_passwordManager.getAllowApp()){
        m_passwordManager.draw();
        return;
    }
    
    
    //ofBackground(50,50,50);
   // m_viewManager.draw();
    m_layoutManager.draw();
    m_guiManager.draw();

}

void AppManager::drawDisplay()
{
    ofClear(0,0,0);
    this->setDisplayShape();
    
    if(!m_initialized)
        return;
    
    if(!m_passwordManager.getAllowApp()){
        return;
    }
    
    auto& fbo = m_layoutManager.getCurrentFbo();
    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}


void AppManager::toggleDebugMode()
{
    m_debugMode = !m_debugMode;
    setDebugMode(m_debugMode);
}


void AppManager::exit()
{
    m_guiManager.saveGuiValues();
}

void AppManager::setDebugMode(bool showDebug)
{
    m_debugMode = showDebug;
    
    ofLogNotice()<<"AppManager::setDebugMode-> " << m_debugMode;
    
    if(m_debugMode){
        ofSetLogLevel(OF_LOG_VERBOSE);
    }
    else{
        ofSetLogLevel(OF_LOG_NOTICE);
    }
    

    m_guiManager.showGui(m_debugMode);
    
}


void AppManager::onEnableDisplay(bool value)
{
   
    if(value)
    {
        auto windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();
        
        m_displayShape.width = windowSettings.back().width;
        m_displayShape.height = windowSettings.back().height;
    
    }
    else
    {
        m_displayShape.width = 0;
        m_displayShape.height = 0;
    }

}

void AppManager::setDisplayShape()
{
    ofSetWindowShape(m_displayShape.width, m_displayShape.height);
}


