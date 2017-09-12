
/*
 *  TextField.cpp
 *  Subtitles Syphon
 *
 *  Created by Imanol Gomez on 12/09/17.
 *
 */

#include "AppManager.h"

#include "TextField.h"

const double TextField::CURSOR_ANIMATION_TIME = 0.5;

TextField::TextField(ofVec3f pos, bool centred):
    BasicVisual(pos, 0, 0),
    m_centred(centred),
    m_cursorElapsedTime(0),
    m_margin(0),
    m_initialized(false)
{
    this->setup();
}

TextField::~TextField()
{
    //Intentionally left empty
}


void TextField::setup()
{
    if(m_initialized){
        return;
    }

    this->createImageVisuals();
    this->createTextVisuals();
    this->createRectangleVisuals();
    this->createBullets();
    this->addListeners();

    m_initialized = true;
}

void TextField::createImageVisuals()
{
    this->createTextFieldImage();
}


void TextField::createTextFieldImage()
{
    ofVec3f position(0,0,0);

    string resourceName = "TextFieldImage";
    ofPtr<ImageVisual> imageVisual = ofPtr<ImageVisual>(new ImageVisual(position,resourceName));
    m_width = imageVisual->getWidth();
    m_height = imageVisual->getHeight();
    m_margin = m_height*0.2;

    m_imageVisuals[resourceName] = imageVisual;
}


void TextField::createTextVisuals()
{
    this->createBackgroundText();
}

void TextField::createBackgroundText()
{
    float x = 2*m_margin;
    float y = 2*m_margin;
    float w = m_width - 4*m_margin;
    float h = m_height - 2*m_margin;
    float fontSize = 34;
    ofVec3f pos(x,y,0);

    ofColor color = ofColor::white;
    string fontName = "fonts/open-sans/OpenSans-Bold.ttf";

    string backgroundText = "Enter password";
    ofPtr<TextVisual> textVisual = ofPtr<TextVisual>(new TextVisual(pos,w,h));
    textVisual->setText(backgroundText,fontName,fontSize,color);
    h = textVisual->getHeight();
    pos.y = m_height*0.5 - h*0.5;
    textVisual->setPosition(pos);

    m_textVisuals["backgroundText"] = textVisual;
}

void TextField::createBullets()
{
    float w = m_width - 2*m_margin;
    float h = m_height*0.2;
    float x = m_margin;
    float y = m_height*0.5 - h*0.5;
    ofVec3f pos(x,y,0);

    m_bullets = ofPtr<BulletsVisual>(new BulletsVisual(pos,w,h));
}

void TextField::createRectangleVisuals()
{
    this->createCursor();
}

void TextField::createCursor()
{
    float w = 2;
    float h = m_height - 2*m_margin;

    float x = m_margin + w*0.5;
    float y = m_height*0.5;

    ofVec3f pos(x,y,0);
    ofColor color = ofColor::white;

    ofPtr<RectangleVisual> rectangleVisual = ofPtr<RectangleVisual>(new RectangleVisual(pos,w,h,true));
    rectangleVisual->setColor(color);
    m_rectangleVisuals["cursor"] = rectangleVisual;
}

void TextField::draw()
{
    ofPushMatrix();
    ofEnableAlphaBlending();

        ofTranslate(m_position);
        if(m_centred){
            ofTranslate(-m_width*0.5, -m_height*0.5);
        }

        ofScale(m_scale.x, m_scale.y);
        ofSetColor(m_color);

        this->updateCursorAnimation();
        this->drawVisuals();


    ofDisableAlphaBlending();
    ofPopMatrix();   // recall the pushed style
}

void TextField::addListeners()
{
    ofRegisterKeyEvents(this); // this will enable the InteractiveVisual class to listen to the key events.

}

 
    
void TextField::keyPressed(ofKeyEventArgs &e)
{
    int key = e.key;
    ofLogVerbose() <<"TextField::keyPressed-> key = " << key;

    if(this->updateText(key))
    {
        this->updateCursorPosition();
        this->updateBackgroundTextColor();
    }
}

void TextField::keyReleased(ofKeyEventArgs &e)
{
    int key = e.key;
    
    ofLogVerbose() <<"KeyboardManager::keyReleased-> " + ofToString(key);
    
}


void TextField::drawVisuals()
{
    for(ImageMap::iterator it = m_imageVisuals.begin(); it!= m_imageVisuals.end(); ++it){
         it->second->draw();
    }

    for(RectangleMap::iterator it = m_rectangleVisuals.begin(); it!= m_rectangleVisuals.end(); ++it){
         it->second->draw();
    }

    for(TextMap::iterator it = m_textVisuals.begin(); it!= m_textVisuals.end(); ++it){
        it->second->draw();
    }

    m_bullets->draw();
}

bool TextField::updateText(int key)
{
    if (key==OF_KEY_DEL || key == OF_KEY_BACKSPACE){
        return this->removeCharacter(key);
	}

    else if (key >=32 && key <=255){
        return this->addCharacter(key);
	}

	else{
        return false;
	}
}

bool TextField::removeCharacter(int key)
{
    if(!m_bullets->removeCharacter()){
        return false;
    }

    if(!m_asciiText.empty()){
        m_asciiText.pop_back();
    }

    return true;
}

bool TextField::addCharacter(int key)
{
    if(!m_bullets->addCharacter()){
        return false;
    }

    m_asciiText.push_back(key);

    return true;
}

void TextField::updateBackgroundTextColor()
{
    float bulletsWidth = m_bullets->getTotalBulletsWidth();
    ofColor color = m_textVisuals["backgroundText"]->getColor();

    if( bulletsWidth > 0){
        color.a = 0;
    }
    else{
        color.a = 255;
    }

    m_textVisuals["backgroundText"]->setColor(color);
}


void TextField::updateCursorAnimation()
{
    ofColor color = m_rectangleVisuals["cursor"]->getColor();

    double dt = ofGetLastFrameTime();
	m_cursorElapsedTime += dt;

	if(m_cursorElapsedTime >= CURSOR_ANIMATION_TIME) {
       m_cursorElapsedTime = 0;

       if(color.a<255){
            color.a = 255;
       }
       else{
        color.a = 0;
       }
	}

    m_rectangleVisuals["cursor"]->setColor(color);
}

void TextField::updateCursorPosition()
{
    ofVec3f position  = m_rectangleVisuals["cursor"]->getPosition();
    position.x = m_margin + m_bullets->getTotalBulletsWidth();

    EffectSettings settings; settings.startAnimation = BULLET_ANIMATION_TIME;
    
    AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_rectangleVisuals["cursor"]);
    AppManager::getInstance().getVisualEffectsManager().createMoveEffect(m_rectangleVisuals["cursor"],position, settings);

    //m_rectangleVisuals["cursor"]->setPosition(position);

    this->resetCursorAnimation();
}

void TextField::resetCursorAnimation()
{
   ofColor color = m_rectangleVisuals["cursor"]->getColor();
   m_cursorElapsedTime = 0;
   color.a = 255;
   m_rectangleVisuals["cursor"]->setColor(color);
}


void TextField::resetTextField()
{
    ofLogNotice()<<"TextField::resetTextField";

    m_bullets->reset();
    m_asciiText.clear();

    this->updateCursorPosition();
    this->updateBackgroundTextColor();
}




