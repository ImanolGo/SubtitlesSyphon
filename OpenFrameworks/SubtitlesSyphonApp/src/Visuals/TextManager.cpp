/*
 *  TextManager.cpp
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */



#include "ofMain.h"

#include "TextManager.h"
#include "AppManager.h"


TextManager::TextManager(): Manager()
{
    //Intentionally left empty
}


TextManager::~TextManager()
{
    ofLogNotice() <<"TextManager::Destructor" ;
}

void TextManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    

    this->setupText();
    
    ofLogNotice() <<"TextManager::initialized" ;
    
}

void TextManager::setupText()
{
    int margin =  LayoutManager::MARGIN;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    ofPoint pos = ofPoint(width/2, height/2);
    
    m_textVisual.setWidth(width - 2*margin); m_textVisual.setHeight(height - 2*margin);
    m_textVisual.setPosition(pos); m_textVisual.setCentred(true);
    
    
    string text = " ";
    
    string fontName = OF_TTF_SANS;
    float size = 30;
    
    m_textVisual.setText(text, fontName, size, ofColor::white);
    
}


void TextManager::update()
{
}


void TextManager::draw()
{
    m_textVisual.draw();
}


void TextManager::onChangeShowBox(bool value)
{
    m_textVisual.drawBoundingBox(value);
}

void TextManager::onChangeSize(int& value)
{
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    int size = (int) ofMap(value, 0, 100, height/100, height/10, true);
    m_textVisual.setFontSize(size);
}

void TextManager::onChangeWidth(float& value)
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    int text_width = (int) ofMap(value, 0.0, 1.0, 0, width, true);
    
    m_textVisual.setWidth(text_width);
}

void TextManager::onChangeLineHeight(float& value)
{
    m_textVisual.setLineHeight(value);
}

void TextManager::onChangePos(ofVec2f& value)
{
    m_textVisual.setPosition(value);
}


void TextManager::onChangePosX(float& value)
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    auto pos = m_textVisual.getPosition();
    float posX = (int) ofMap(value, 0.0, 1.0, 0, width, true);
    pos.x = posX;
    m_textVisual.setPosition(pos);
}

void TextManager::onChangePosY(float& value)
{
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    auto pos = m_textVisual.getPosition();
    float posY = (int) ofMap(value, 0.0, 1.0, 0, height, true);
    pos.y = posY;
    m_textVisual.setPosition(pos);
}

void TextManager::onChangePosZ(float& value)
{
    auto pos = m_textVisual.getPosition();
    float posZ = (int) ofMap(value, -1.0, 1.0, -100, 100, true);
    pos.z = posZ;
    m_textVisual.setPosition(pos);
}



void TextManager::setColor(const ofColor& color)
{
    m_textVisual.setColor(color);
}

void TextManager::setText(const string& text)
{
    m_textVisual.setText(text);
}

void TextManager::setFontType(const string& name)
{
    m_textVisual.setFontName(name);
    AppManager::getInstance().getGuiManager().setFontLabel(name);
}


