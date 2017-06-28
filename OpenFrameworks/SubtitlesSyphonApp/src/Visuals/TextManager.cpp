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
    
    
    string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi vel tempor diam, ut laoreet risus. Mauris est orci, scelerisque et accumsan in, porttitor eget ipsum. Pellentesque eleifend justo sapien, sit amet vulputate quam vehicula quis. Aenean eu condimentum velit, ut eleifend quam. Morbi faucibus erat in suscipit aliquet. Integer arcu nibh, porttitor a arcu sagittis, luctus laoreet mauris. Etiam tincidunt pulvinar metus, fringilla ultricies urna condimentum in. Praesent in neque enim. Phasellus mattis congue fringilla. Donec condimentum vitae lectus vel laoreet. Praesent lobortis ac massa eget elementum. Ut non magna rutrum quam sollicitudin posuere et a sem. Integer id arcu sit amet neque luctus semper. Donec faucibus rutrum risus.";
    
    string fontName = "Lucida Grande";
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


void TextManager::onChangePosX (float& value)
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    auto pos = m_textVisual.getPosition();
    float posX = (int) ofMap(value, 0.0, 1.0, 0, width, true);
    pos.x = posX;
    m_textVisual.setPosition(pos);
}

void TextManager::onChangePosY (float& value)
{
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    auto pos = m_textVisual.getPosition();
    float posY = (int) ofMap(value, 0.0, 1.0, 0, height, true);
    pos.y = posY;
    m_textVisual.setPosition(pos);
}


void TextManager::setColor(const ofColor& color)
{
    m_textVisual.setColor(color);
}
