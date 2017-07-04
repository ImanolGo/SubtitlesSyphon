/*
 *  GuiManager.h
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */

#pragma once

#include "Manager.h"
#include "ofxDatGui.h"

//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *	\brief Class managing the application´s grapical user interface (GUI)
 *	\details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    static const string GUI_SETTINGS_FILE_NAME;
    static const string GUI_SETTINGS_NAME;
    static const int GUI_WIDTH;
    
public:

    //! Constructor
    GuiManager();

    //! Destructor
    ~GuiManager();

    //! Set-up the gui
    void setup();
    
    //! Update the gui
    void update();
    
    //! Draw the gui
    void draw();
    
    void saveGuiValues();
    
    void loadGuiValues();
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    int getWidth() {return GUI_WIDTH;}
    
    int getHeight() {return m_gui.getHeight();}
    
    ofPoint  getPosition() {return m_gui.getPosition();}
    
    void onChangeColor(int& value);
    
    void nextRow();
    
    void previousRow();
    
    void resetSubtitles();
    
    void setNumCols(int value);
    
    void setNumRows(int value);
    
    void setSubtitlesName(const string& name);
    
    void setFontLabel(const string& name);
    
    void setTextSize(int value);
    
    void setTextLineHeight(float value);
    
    void setTextTransitionTime(float value);
    
    void setTextWidth(float value);
    
    void setTextPosX(float value);
    
    void setTextPosY(float value);
    
    void setColorR(int value);
    
    void setColorG(int value);
    
    void setColorB(int value);
    
    void setSubCol(int value);
    
    void setSubRow(int value);
    
    void setSyphonToggle(bool value);
    
    void setSyphonEnable(bool value);
    
    void setShowBox(bool value);
    
private:
    
    void setupGuiParameters();
    
    void setupTextGui();
    
    void setupColorGui();
    
    void setupSubtitlesGui();
    
    void setupGuiEvents();
    
    void drawRectangle();
    
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    
    void onToggleEvent(ofxDatGuiToggleEvent e);
    
    void onMatrixEvent(ofxDatGuiMatrixEvent e);
    
    void openSystemDialog();
    

private:
    

    ofxDatGui               m_gui;
    
    ofParameterGroup        m_parameters;
    
    ofParameter<int>        m_oscPort;
    
    ofParameter<int>        m_textSize;
    ofParameter<float>      m_textWidth;
    ofParameter<float>      m_textLineHeight;
    ofParameter<float>      m_textX;
    ofParameter<float>      m_textY;
    ofParameter<float>      m_textZ;
    ofParameter<float>      m_textTransitionTime;
    ofxDatGuiLabel*         m_fontLabel;
    
    ofParameter<int>        m_red, m_green,m_blue;
    ofColor                 m_currentColor;
    
    ofParameter<int>        m_subCol;
    ofParameter<int>        m_subRow;
    ofxDatGuiLabel*         m_subLabel;
   
    bool        m_showGui;  //It defines the whether the gui should be shown or not
    
};

//==========================================================================


