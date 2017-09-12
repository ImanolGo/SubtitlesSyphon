/*
 *  BulletsVisual.h
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 12/09/17.
 *
 */


#include "AppManager.h"

#include "BulletsVisual.h"

const int BulletsVisual::MAX_NUMBER_BULLETS = 60;       ///< Defines the maximum number of bullets that you can have

BulletsVisual::BulletsVisual(ofVec3f pos, float width, float height, bool centred):
    BasicVisual(pos, width, height), m_singleBulletSize(0),m_totalBulletsWidth(0),m_numberOfBullets(0)
{
    this->setup();
}

BulletsVisual::~BulletsVisual()
{
    //Intentionally left empty
}


void BulletsVisual::setup()
{
    this->setupBulletColor();
    this->setupBulletSize();
}

void BulletsVisual::setupBulletColor()
{
    m_color = ofColor::white;
}

void BulletsVisual::setupBulletSize()
{
    if(m_width>m_height){
        m_singleBulletSize = m_height;
    }
    else{
        m_singleBulletSize = m_width;
    }

    ofLogNotice() <<" BulletsVisual::setupBulletSize-> width = " << m_width;
    ofLogNotice() <<" BulletsVisual::setupBulletSize-> m_singleBulletSize = " << m_singleBulletSize;
}

void BulletsVisual::draw()
{
    ofPushMatrix();
    ofEnableAlphaBlending();

        ofTranslate(m_position);
        ofScale(m_scale.x, m_scale.y);
        ofSetColor(m_color);
        //ofRect(0,0,m_width,m_height);

        this->drawBullets();

    ofDisableAlphaBlending();
    ofPopMatrix();   // recall the pushed style
}


bool BulletsVisual::addCharacter()
{
    if(m_numberOfBullets>=MAX_NUMBER_BULLETS){
        return false;
    }

    //ofLogNotice() <<" BulletsVisual::addCharacter";

    if(m_totalBulletsWidth +  1.5*m_singleBulletSize>m_width){ //If you add one reach the total witdth
        m_numberOfBullets++;
        return true;
    }


    float x = m_totalBulletsWidth + m_singleBulletSize*0.5;
    float y = m_height*0.5;

    ofVec3f position(x,y,0);
    string resourceName = "WhiteBullet";
    ofPtr<ImageVisual> bullet = ofPtr<ImageVisual>(new ImageVisual(position,resourceName,true));
    bullet->setWidth(m_singleBulletSize, true);
    
   // EffectSettings settings; settings.startAnimation = BULLET_ANIMATION_TIME; settings.animationTime = BULLET_ANIMATION_TIME;

    
    //AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(bullet);
    //AppManager::getInstance().getVisualEffectsManager().createFadeEffect(bullet,0, 255,settings);

    m_bullets.push_back(bullet);
    m_numberOfBullets = m_bullets.size();
    m_totalBulletsWidth = 1.5*m_bullets.size()*m_singleBulletSize;

    return true;
}


bool BulletsVisual::removeCharacter()
{
    if(m_bullets.empty()){
        return false;
    }

   // ofLogNotice() <<" BulletsVisual::removeCharacter";

    if(m_numberOfBullets>m_bullets.size()){
        m_numberOfBullets--;
        return true;
    }



    m_bullets.pop_back();
    m_numberOfBullets = m_bullets.size();
    m_totalBulletsWidth = 1.5*m_bullets.size()*m_singleBulletSize;


    return true;

}

void BulletsVisual::drawBullets()
{
    for(ImageVector::iterator it = m_bullets.begin(); it!= m_bullets.end(); ++it){
        (*it)->draw();
    }
}

void BulletsVisual::reset()
{
    m_totalBulletsWidth = 0;
    m_bullets.clear();
}




