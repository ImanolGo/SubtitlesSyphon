/*
 *  TextManager.h
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */


#pragma once

#include "Manager.h"
#include "TextVisual.h"

//========================== class TextManager ==============================
//============================================================================
/** \class TextManager TextManager.h
 *	\brief Class managing the text
 *	\details it renders the current subtitles according to a width, height and position
 */


class TextManager: public Manager
{
    static const string UNICODE_FONT_PATH; //The Apps unicode font path
    
public:
    
    //! Constructor
    TextManager();
    
    //! Destructor
    ~TextManager();
    
    //! Setup the Text Manager
    void setup();
    
    //! Update the Text Manager
    void update();
    
    //! Draw the Text Manager
    void draw();
    
    void drawPreviousVisuals();
    
    void drawCurrentVisuals();
    
    void  onChangeSize(int& value);
    
    void  onChangeWidth(float& value);
    
    void  onChangeLineHeight(float& value);
    
    void  onChangePos(ofVec2f& value);
    
    void  onChangePosX (float& value);
    
    void  onChangePosY (float& value);
    
    void  onChangePosZ (float& value);
    
    void  onChangeShowBox (bool value);
    
    void onChangeTransitionTime(float& value);
    
    void setColor(ofColor& color);
    
    void setCurrentText(const string& text);
    
    void setNextText(const string& text);
    
    void setFontType(const string& name);
    
    void setFont(string value);
    
    const vector<string>& getSystemFonts() const {return m_fontNames;}
    
private:
    
    void setupFonts();
    
    void setupFontNames();
    
    void setupFbos();
    
    void setupVisuals();
    
    void addCurrentCrossFadeAnimations();
    
    void addPreviewCrossFadeAnimations();
    
    string getRootPath(string path);
    
    string getNameFromExtenstion(string path);
    
    string getFontAbsolutePath(string name);
    
private:
    
    typedef  map<string, shared_ptr<TextVisual>> TextVisualMap;
    typedef  vector<string>     FontNameVector;
    typedef  map<string,string>   FontPaths;
    
    TextVisualMap   m_textVisuals;
    FontNameVector  m_fontNames;
    FontPaths       m_fontPaths;
    
    ofFbo       m_currentFbo;
    ofFbo       m_previusFbo;
    float       m_transitionTime;
    
    string      m_systemFont;
    string      m_unicodeFont;

};



