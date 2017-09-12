/*
 *  OscManager.cpp
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */


#include "OscManager.h"
#include "SettingsManager.h"
#include "AppManager.h"


const string OscManager::OSC_PARENT_ADDRESS = "/subs";

OscManager::OscManager(): Manager()
{
    //Intentionally left empty
}

OscManager::~OscManager()
{
   ofLogNotice() << "OscManager::destructor";
}


//--------------------------------------------------------------

void OscManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupOscReceiver();
    this->setupOscSender();
    
    ofLogNotice() <<"OscManager::initialized" ;
}

void OscManager::setupOscReceiver()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getOscPortReceive();
    ofLogNotice() <<"OscManager::setupOscReceiver -> listening for osc messages on port  " << portReceive;
    m_oscReceiver.setup(portReceive);
   
    
}

void OscManager::setupOscSender()
{
    int portSend = AppManager::getInstance().getSettingsManager().getOscPortSend();
    string host = AppManager::getInstance().getSettingsManager().getIpAddress();
    m_oscSender.setup(host, portSend);
    ofLogNotice() <<"OscManager::setupOscSender -> open osc connection " << host << ":" << portSend;
    
}


void OscManager::onChangePort(int& value)
{
    ofLogNotice() <<"OscManager::onChangePort -> listening for osc messages on port  " << value;
    m_oscReceiver.setup(value);
}

void OscManager::update()
{
    // check for waiting messages
    while(m_oscReceiver.hasWaitingMessages())
    {
        // get the next message
        ofxOscMessage m;
        m_oscReceiver.getNextMessage(m);
        
        if(m.getAddress() == OSC_PARENT_ADDRESS + "/column")
        {
            int value = m.getArgAsInt(0);
            AppManager::getInstance().getGuiManager().setSubCol(value);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/row")
        {
            int value = m.getArgAsInt(0);
            AppManager::getInstance().getGuiManager().setSubRow(value);
        }

        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/color")
        {
            int r = m.getArgAsInt(0); int g = m.getArgAsInt(1); int b = m.getArgAsInt(2);
            AppManager::getInstance().getGuiManager().setColorR(r);
            AppManager::getInstance().getGuiManager().setColorG(g);
            AppManager::getInstance().getGuiManager().setColorB(b);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/textsize")
        {
            int value = m.getArgAsInt(0);
            AppManager::getInstance().getGuiManager().setTextSize(value);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/lineheight")
        {
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setTextLineHeight(value);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/transitiontime")
        {
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setTextTransitionTime(value);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/textwidth")
        {
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setTextWidth(value);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/position")
        {
            float x = m.getArgAsFloat(0);  float y = m.getArgAsFloat(1);
            AppManager::getInstance().getGuiManager().setTextPosX(x);
            AppManager::getInstance().getGuiManager().setTextPosY(y);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/font")
        {
            string value  = m.getArgAsString(0);
            AppManager::getInstance().getTextManager().setFontType(value);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/syphon")
        {
            bool value  = m.getArgAsInt(0)>0;
            AppManager::getInstance().getGuiManager().setSyphonToggle(value);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/enable")
        {
            bool value  = m.getArgAsInt(0)>0;
            AppManager::getInstance().getGuiManager().setSyphonEnable(value);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/showbox")
        {
            bool value  = m.getArgAsInt(0)>0;
            AppManager::getInstance().getGuiManager().setShowBox(value);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/reload")
        {
            bool value  = m.getArgAsInt(0)>0;
            AppManager::getInstance().getSubtitlesManager().reloadFile();
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/updatefile")
        {
            bool value  = m.getArgAsInt(0)>0;
            AppManager::getInstance().getSubtitlesManager().reloadFile();
        }
        
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/next")
        {
            bool value  = m.getArgAsInt(0)>0;
            AppManager::getInstance().getGuiManager().nextRow();
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/previous")
        {
            bool value  = m.getArgAsInt(0)>0;
            AppManager::getInstance().getGuiManager().previousRow();
        }
          //ofLogNotice() <<"OscManager::received -> " << this->getMessageAsString(m);
    }
}


string OscManager::getMessageAsString(const ofxOscMessage& m) const
{
    string msg_string;
    msg_string = m.getAddress();
    for(int i = 0; i < m.getNumArgs(); i++){
        // get the argument type
        msg_string += " ";
        // display the argument - make sure we get the right type
        if(m.getArgType(i) == OFXOSC_TYPE_INT32){
            msg_string += ofToString(m.getArgAsInt32(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
            msg_string += ofToString(m.getArgAsFloat(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
            msg_string += m.getArgAsString(i);
        }
        else{
            msg_string += "unknown";
        }
    }
    
    return msg_string;
}











