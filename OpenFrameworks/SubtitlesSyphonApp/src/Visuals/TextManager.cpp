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

#include "EngineFont.h"

const string TextManager::UNICODE_FONT_PATH = "fonts/Arial Unicode.ttf";

TextManager::TextManager(): Manager(), m_transitionTime(0.5)
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
    
    this->setupFonts();
    this->setupVisuals();
    this->setupFbos();
    
    ofLogNotice() <<"TextManager::initialized" ;
    
}

void TextManager::setupVisuals()
{
    int margin =  LayoutManager::MARGIN;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    ofPoint pos = ofPoint(width/2, height/2);
    float w = width - 2*margin;
    float h = height - 2*margin;
    string text = " ";
    string fontName = OF_TTF_SANS;
    float size = 30;
    

    auto textVisual = shared_ptr<TextVisual>(new TextVisual(pos,w,h,true));
    textVisual->setText(text, fontName, size, ofColor::white);
    m_textVisuals["CurrentText"] = textVisual;
    
    textVisual = shared_ptr<TextVisual>(new TextVisual(pos,w,h,true));
    textVisual->setText(text, fontName, size, ofColor::white);
    m_textVisuals["PreviousText"] = textVisual;
    
    textVisual = shared_ptr<TextVisual>(new TextVisual(pos,w,h,true));
    textVisual->setText(text, fontName, size, ofColor::white);
    m_textVisuals["CurrentTextPreview"] = textVisual;
    
    textVisual = shared_ptr<TextVisual>(new TextVisual(pos,w,h,true));
    textVisual->setText(text, fontName, size, ofColor::white);
    m_textVisuals["PreviousTextPreview"] = textVisual;

}

void TextManager::setupFbos()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();

    m_currentFbo.allocate(width, height, GL_RGBA);
    m_currentFbo.begin(); ofClear(0,0,0,0); m_currentFbo.end();
    
    m_previusFbo.allocate(width, height, GL_RGBA);
    m_previusFbo.begin(); ofClear(0,0,0,0); m_previusFbo.end();
}

void TextManager::setupFonts()
{
    this->setupFontNames();
}

void TextManager::setupFontNames()
{
    string systemFontsPath = getRootPath(EngineFont::getFontFilePathByName("Helvetica"));
    
    ofLogNotice() <<"TextManager::setupFontNames -> " << systemFontsPath;
    
    ofDirectory dir(systemFontsPath);
    //only show font files
    dir.allowExt("dfont");
    dir.allowExt("otf");
    dir.allowExt("ttf");
    dir.allowExt("ttc");
    
    //populate the directory object
    dir.listDir();
    
    for(int i = 0; i < dir.size(); i++){
        m_fontNames.push_back(this->getNameFromExtenstion(dir.getName(i)));
        m_fontPaths[m_fontNames.back()] = dir.getPath(i);
    }
    
    systemFontsPath = getRootPath(EngineFont::getFontFilePathByName("Arial"));
    
    dir.open(systemFontsPath);
    dir.listDir();
    
    for(int i = 0; i < dir.size(); i++){
        m_fontNames.push_back(this->getNameFromExtenstion(dir.getName(i)));
        m_fontPaths[m_fontNames.back()] = dir.getPath(i);
    }
    
    systemFontsPath = ofFilePath::getUserHomeDir() + "/Library/Fonts/";
    
    dir.open(systemFontsPath);
    dir.listDir();
    
    for(int i = 0; i < dir.size(); i++){
        m_fontNames.push_back(this->getNameFromExtenstion(dir.getName(i)));
        m_fontPaths[m_fontNames.back()] = dir.getPath(i);
    }
    
    std::sort(std::begin(m_fontNames), std::end(m_fontNames));
    
}

string TextManager::getFontAbsolutePath(string name)
{
    if(m_fontPaths.find(name)==m_fontPaths.end()){
        return "";
    }
    else{
        return m_fontPaths.at(name);
    }
}

string TextManager::getRootPath(string path)
{
    auto vectorPath = ofSplitString(path, "/");
    string rootPath = "";
    
    for(int i=1; i<vectorPath.size()-1;i++){
        rootPath += ("/" + vectorPath[i]);
    }
    
    return rootPath;
}

string TextManager::getNameFromExtenstion(string path)
{
    return ofSplitString(path, ".").front();
}

void TextManager::update()
{
}


void TextManager::draw()
{
    this->drawCurrentVisuals();
}


void TextManager::drawCurrentVisuals()
{
    //ofSetColor(255, 0, 0);
    //ofDrawRectangle(0, 0, 1920, 1080);
    m_textVisuals["CurrentText"]->draw();
    m_textVisuals["PreviousText"]->draw();
}

void TextManager::drawPreviousVisuals()
{
    m_textVisuals["CurrentTextPreview"]->draw();
    m_textVisuals["PreviousTextPreview"]->draw();
}

void TextManager::onChangeShowBox(bool value)
{
    for(auto visual: m_textVisuals){
        visual.second->drawBoundingBox(value);
    }
  
}

void TextManager::onChangeSize(int& value)
{
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    int size = (int) ofMap(value, 0, 100, height/100, height/8, true);
    
    for(auto visual: m_textVisuals){
        visual.second->setFontSize(size);
    }
}

void TextManager::onChangeTransitionTime(float& value)
{
    m_transitionTime = value;
}

void TextManager::onChangeWidth(float& value)
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    int text_width = (int) ofMap(value, 0.0, 1.0, 0, width, true);
    
    for(auto visual: m_textVisuals){
        visual.second->setWidth(text_width);
    }
}

void TextManager::onChangeLineHeight(float& value)
{
    for(auto visual: m_textVisuals){
        visual.second->setLineHeight(value);
    }
}

void TextManager::onChangePos(ofVec2f& value)
{
    for(auto visual: m_textVisuals){
        visual.second->setPosition(value);
    }
}


void TextManager::onChangePosX(float& value)
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    
    for(auto visual: m_textVisuals){
        auto pos = visual.second->getPosition();
        float posX = (int) ofMap(value, 0.0, 1.0, 0, width, true);
        pos.x = posX;
        visual.second->setPosition(pos);
    }
}

void TextManager::onChangePosY(float& value)
{
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
   
    for(auto visual: m_textVisuals){
        auto pos = visual.second->getPosition();
        float posY = (int) ofMap(value, 0.0, 1.0, 0, height, true);
        pos.y = posY;
        visual.second->setPosition(pos);
    }
}

void TextManager::onChangePosZ(float& value)
{
    for(auto visual: m_textVisuals){
        auto pos = visual.second->getPosition();
        float posZ = (int) ofMap(value, -1.0, 1.0, -100, 100, true);
        pos.z = posZ;
        visual.second->setPosition(pos);
    }
}


void TextManager::onChangeRotX(float& value)
{
    for(auto visual: m_textVisuals){
        auto rot = visual.second->getRotation();
        rot.x = value;
        visual.second->setRotation(rot);
    }
}

void TextManager::onChangeRotY(float& value)
{
    for(auto visual: m_textVisuals){
        auto rot = visual.second->getRotation();
        rot.y = value;
        visual.second->setRotation(rot);
    }
}

void TextManager::onChangeRotZ(float& value)
{
    for(auto visual: m_textVisuals){
        auto rot = visual.second->getRotation();
        rot.z = value;
        visual.second->setRotation(rot);
    }
}



void TextManager::setColor(ofColor& color)
{
    for(auto visual: m_textVisuals){
        int alpha = visual.second->getAlpha();
        color.a = alpha;
        visual.second->setColor(color);
    }
}

void TextManager::setCurrentText(const string& text)
{
    ofLogNotice() <<"TextManager::PreviousText -> " << m_textVisuals["CurrentText"]->getText();
    ofLogNotice() <<"TextManager::CurrentText -> " << text;
    m_textVisuals["PreviousText"]->setText(m_textVisuals["CurrentText"]->getText());
    m_textVisuals["CurrentText"]->setText(text);
    this->addCurrentCrossFadeAnimations();
}


void TextManager::setNextText(const string& text)
{
    m_textVisuals["PreviousTextPreview"]->setText(m_textVisuals["CurrentTextPreview"]->getText());
    m_textVisuals["CurrentTextPreview"]->setText(text);
    this->addPreviewCrossFadeAnimations();
}

void TextManager::setFontType(const string& name)
{
    string fontPath = this->getFontAbsolutePath(name);
    if(fontPath == ""){
         ofLogNotice() <<"TextManager::setFontType -> No font found with name " << name ;
        return;
    }
    
    for(auto visual: m_textVisuals){
        visual.second->setFontName(fontPath);
    }
    AppManager::getInstance().getGuiManager().setFontLabel(name);
    
    ofLogNotice() <<"TextManager::setFontType -> " << name ;
    
   // m_systemFont = name;
}


void TextManager::addCurrentCrossFadeAnimations()
{
    
    AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_textVisuals["CurrentText"]);
    AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_textVisuals["PreviousText"]);
    
    EffectSettings settings;
    settings.function = LINEAR; settings.type = EASE_OUT;
    settings.startAnimation = 0; settings.animationTime = m_transitionTime;
    
    auto  previousAlpha = m_textVisuals["CurrentText"]->getAlpha();
    m_textVisuals["PreviousText"]->setAlpha(previousAlpha);
    m_textVisuals["CurrentText"]->setAlpha(0);
    
    AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_textVisuals["CurrentText"], 0,255, settings);
    AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_textVisuals["PreviousText"], previousAlpha, 0, settings);
}


void TextManager::addPreviewCrossFadeAnimations()
{
    AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_textVisuals["CurrentTextPreview"]);
    AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_textVisuals["PreviousTextPreview"]);
    
    EffectSettings settings;
    settings.function = LINEAR; settings.type = EASE_OUT;
    settings.startAnimation = 0; settings.animationTime = m_transitionTime;
    
    auto  previousAlpha = m_textVisuals["CurrentTextPreview"]->getAlpha();
    m_textVisuals["PreviousTextPreview"]->setAlpha(previousAlpha);
    m_textVisuals["CurrentTextPreview"]->setAlpha(0);
    
    AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_textVisuals["CurrentTextPreview"], 0,255, settings);
    AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_textVisuals["PreviousTextPreview"], previousAlpha, 0, settings);
}



