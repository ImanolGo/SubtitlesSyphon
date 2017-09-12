/*
 *  SettingsManager.cpp
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */

#include "ofMain.h"

#include "SettingsManager.h"


const string SettingsManager::APPLICATION_SETTINGS_FILE_NAME = "xmls/ApplicationSettings.xml";


SettingsManager::SettingsManager(): Manager(), m_appHeight(0.0), m_appWidth(0.0), m_needsPassword(false)
{
    //Intentionally left empty
}


SettingsManager::~SettingsManager()
{
    ofLogNotice() <<"SettingsManager::Destructor" ;
}


void SettingsManager::setup()
{
    if(m_initialized)
        return;
    
    ofLogNotice() <<"SettingsManager::initialized" ;
    
    Manager::setup();
    
    if(this->loadSettingsFile()){
        this->loadAllSettings();
    }
}

void SettingsManager::loadAllSettings()
{
    this->setWindowProperties();
    this->setDebugProperties();
    this->setNetworkProperties();
    this->loadTextureSettings();
    this->loadSvgSettings();
    this->loadVideoSettings();
    this->loadColors();
}

bool SettingsManager::loadSettingsFile()
{
    
    if(!m_xml.load(APPLICATION_SETTINGS_FILE_NAME)){
        ofLogNotice() <<"SettingsManager::loadSettingsFile-> unable to load file: " << APPLICATION_SETTINGS_FILE_NAME ;
        return false;
    }
    
    ofLogNotice() <<"SettingsManager::loadSettingsFile->  successfully loaded " << APPLICATION_SETTINGS_FILE_NAME ;
    return true;
}

void SettingsManager::setDebugProperties()
{
    m_xml.setTo("//");
    
    string ofPath = "//of_settings/debug";
    if(m_xml.exists(ofPath)) {
        m_xml.setTo(ofPath);
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes = m_xml.getAttributes();
        
        bool showCursor = ofToBool(attributes["showCursor"]);
        if(showCursor){
            ofShowCursor();
        }
        else{
            ofHideCursor();
        }
        
        bool setVerbose = ofToBool(attributes["setVerbose"]);
        if(setVerbose){
            ofSetLogLevel(OF_LOG_VERBOSE);
        }
        else{
            ofSetLogLevel(OF_LOG_NOTICE);
        }
        
        m_needsPassword = ofToBool(attributes["password"]);
        
        ofLogNotice() <<"SettingsManager::setDebugProperties->  needs password: " << m_needsPassword ;
        
        ofLogNotice() <<"SettingsManager::setDebugProperties->  successfully loaded the OF general settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setOFProperties->  path not found: " << ofPath ;
}

void SettingsManager::setWindowProperties()
{
    m_xml.setTo("//");
    
    string windowPath = "//of_settings/window";
    if(m_xml.exists(windowPath)) {
        m_xml.setTo(windowPath);
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes = m_xml.getAttributes();
        string title = attributes["title"];
        m_appWidth = ofToInt(attributes["width"]);
        m_appHeight= ofToInt(attributes["height"]);
        ofSetWindowShape(m_appWidth,m_appHeight);
        
        //m_appWidth = ofGetScreenWidth();
        //m_appHeight = ofGetScreenHeight();
        
        int x = ofToInt(attributes["x"]);
        int y = ofToInt(attributes["y"]);
        bool fullscreen = ofToBool(attributes["fullscreen"]);
        
        ofSetFullscreen(fullscreen);
        //ofSetWindowShape(m_appWidth,m_appHeight);
        if(!fullscreen){
            //ofSetWindowPosition(x,y);
            int x = ofGetScreenWidth()/2 - m_appWidth/2;
            int y = ofGetScreenHeight()/2 - m_appHeight/2;
            ofSetWindowPosition(x,y);
        }
        ofSetWindowTitle(title);
        
        ofLogNotice() <<"SettingsManager::setWindowProperties->  successfully loaded the window settings" ;
        ofLogNotice() <<"SettingsManager::setWindowProperties->  title = "<< title<<", width = " << m_appWidth <<", height = "
        <<m_appHeight <<", x = "<<x<<", y = "<<y;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setWindowProperties->  path not found: " << windowPath ;
}
void SettingsManager::setNetworkProperties()
{
    m_xml.setTo("//");
    
    string networkPath = "//of_settings/network";
    if(m_xml.exists(networkPath)) {
        m_xml.setTo(networkPath);
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes = m_xml.getAttributes();
        
        m_portOscSend  = ofToInt(attributes["portOscSend"]);
        m_portOscReceive  = ofToInt(attributes["portOscReceive"]);
        m_ipAddress  = ofToString(attributes["ipAddress"]);
        m_syphonName = ofToString(attributes["syphon"]);
        m_midiName = ofToString(attributes["midi_name"]);
        
        ofLogNotice() <<"SettingsManager::setNetworkProperties->  successfully loaded the network settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setNetworkProperties->  path not found: " << networkPath ;
}

void SettingsManager::loadColors()
{
    m_xml.setTo("//");
    
    string colorsSettingsPath = "//colors";
    if(m_xml.exists(colorsSettingsPath)) {
        
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes;
        
        colorsSettingsPath = "//colors/color[0]";
        m_xml.setTo(colorsSettingsPath);
        do {
            
            attributes = m_xml.getAttributes();
            
            int r = ofToInt(attributes["r"]);
            int g = ofToInt(attributes["g"]);
            int b = ofToInt(attributes["b"]);
            int a = ofToInt(attributes["a"]);
            
            ofPtr<ofColor> color = ofPtr<ofColor> (new ofColor(r,g,b,a));
            m_colors[attributes["name"]] = color;
            
            
            ofLogNotice() <<"SettingsManager::loadColors->  color = " << attributes["name"] <<", r = " << r
            <<", g = "<< g << ", b = " << b << ", a = " << a ;
        }
        while(m_xml.setToSibling()); // go to the next node
        
        
        ofLogNotice() <<"SettingsManager::loadColors->  successfully loaded the applications colors" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadColors->  path not found: " << colorsSettingsPath ;
}

void SettingsManager::loadTextureSettings()
{
    m_xml.setTo("//");
    
    string resourcesPath = "//textures";
    if(m_xml.exists(resourcesPath)) {
        
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes;
        
        resourcesPath = "//textures/texture[0]";
        m_xml.setTo(resourcesPath);
        do {
            
            attributes = m_xml.getAttributes();
            m_texturesPath[attributes["name"]] = attributes["path"];
            
            ofLogNotice() <<"SettingsManager::loadTextureSettings->  texture = " << attributes["name"]
            <<", path = "<< attributes["path"] ;
        }
        while(m_xml.setToSibling()); // go to the next texture
        
        
        ofLogNotice() <<"SettingsManager::loadTextureSettings->  successfully loaded the resource settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadTextureSettings->  path not found: " << resourcesPath ;
}

ofColor SettingsManager::getColor(const string& colorName)
{
    ofColor color;
    if(m_colors.find(colorName)!= m_colors.end()){
        color.setHex(m_colors[colorName]->getHex());
    }
    
    return color;
}


void SettingsManager::loadSvgSettings()
{
    m_xml.setTo("//");
    
    string svgPath = "//svgs";
    if(m_xml.exists(svgPath)) {
        
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes;
        
        svgPath = "//svgs/svg[0]";
        m_xml.setTo(svgPath);
        do {
            
            attributes = m_xml.getAttributes();
            m_svgResourcesPath[attributes["name"]] = attributes["path"];
            
            ofLogNotice() <<"SettingsManager::loadSvgSettings->  svg = " << attributes["name"]
            <<", path = "<< attributes["path"] ;
        }
        while(m_xml.setToSibling()); // go to the next svg
        
        
        ofLogNotice() <<"SettingsManager::loadSvgSettings->  successfully loaded the resource settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadSvgSettings->  path not found: " << svgPath ;
}

void SettingsManager::loadVideoSettings()
{
    m_xml.setTo("//");
    
    string path = "//videos";
    if(m_xml.exists(path)) {
        
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes;
        
        path = "//videos/video[0]";
        m_xml.setTo(path);
        do {
            
            attributes = m_xml.getAttributes();
            m_videoResourcesPath[attributes["name"]] = attributes["path"];
            
            ofLogNotice() <<"SettingsManager::loadVideoSettings->  video = " << attributes["name"]
            <<", path = "<< attributes["path"] ;
        }
        while(m_xml.setToSibling()); // go to the next svg
        
        
        ofLogNotice() <<"SettingsManager::loadSvgSettings->  successfully loaded the resource settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadSvgSettings->  path not found: " << path ;
}








