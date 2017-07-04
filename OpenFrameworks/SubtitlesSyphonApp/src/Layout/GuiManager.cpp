/*
 *  GuiManager.cpp
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */

#include "ofMain.h"

#include "AppManager.h"
#include "GuiManager.h"
#include "GuiTheme.h"


const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "SUBTITLES SYPHON GUI";
const int GuiManager::GUI_WIDTH = 350;


GuiManager::GuiManager(): Manager(), m_showGui(true), m_subLabel(NULL), m_fontLabel(NULL)
{
    //! Intentionally left empty
}

GuiManager::~GuiManager()
{
    this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
	if(m_initialized)
		return;
    
    Manager::setup();

    this->setupGuiParameters();
    this->setupSubtitlesGui();
    this->setupTextGui();
    this->setupColorGui();
    this->setupGuiEvents();
    this->loadGuiValues();
    
    ofxDatGuiLog::quiet();
    
    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
    m_gui.setPosition(ofxDatGuiAnchor::TOP_LEFT);
    m_gui.setAssetPath(ofToDataPath("fonts/"));
    //m_gui.setAssetPath("../Resources/data/fonts/");
    m_gui.setTheme(new GuiTheme());
    

    int margin =  LayoutManager::MARGIN;
    m_gui.setAutoDraw(false);
    auto pos = m_gui.getPosition();
    m_gui.setPosition(pos.x + margin, pos.y + margin);
    m_gui.addHeader(GUI_SETTINGS_NAME, false);
    
    m_gui.addFRM();
    auto toggle = m_gui.addToggle("Fullscreen");
    toggle->setChecked(true);
    toggle = m_gui.addToggle("Syphon");
    toggle->setChecked(true);
    toggle = m_gui.addToggle("Blackout");
    toggle->setChecked(true);
    //m_gui.addButton("* Save GUI");
    
    auto oscManager = &AppManager::getInstance().getOscManager();
    
    m_oscPort.set("OSC Port: ", 5678, 1024, 50000);
    m_oscPort.addListener(oscManager, &OscManager::onChangePort);
    m_parameters.add(m_oscPort);
    
    m_gui.addSlider(m_oscPort);
    
    m_gui.addBreak();
}

void GuiManager::setupSubtitlesGui()
{
    auto subtitles = &AppManager::getInstance().getSubtitlesManager();
    
    m_subCol.set("Column: ", 0, 0, 0);
    m_subCol.addListener(subtitles, &SubtitlesManager::onChangeCol);
    //m_parameters.add(m_subCol);
    
    m_subRow.set("Row: ", 0, 0, 0);
    m_subRow.addListener(subtitles, &SubtitlesManager::onChangeRow);
    //m_parameters.add(m_subRow)
    
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = m_gui.addFolder("SUBTITLES", ofColor::green);
    folder->addLabel("FILE:");
    folder->addButton("* LOAD");
    folder->addButton("* RELOAD");
    folder->addButton("* NEXT");
    folder->addButton("* PREVIOUS");
    folder->addSlider(m_subCol);
    folder->addSlider(m_subRow);
    folder->expand();
    
     m_subLabel = m_gui.getLabel("FILE:");
}


void GuiManager::setupTextGui()
{
    auto textManager = &AppManager::getInstance().getTextManager();
    
    m_textTransitionTime.set("Transition Time", 0.5, 0, 2);
    m_textTransitionTime.addListener(textManager, &TextManager::onChangeTransitionTime);
    m_parameters.add(m_textTransitionTime);
    
    m_textSize.set("Font Size", 10, 1, 100);
    m_textSize.addListener(textManager, &TextManager::onChangeSize);
    m_parameters.add(m_textSize);
    
    m_textWidth.set("Width", 0.9, 0.0, 1.0);
    m_textWidth.addListener(textManager, &TextManager::onChangeWidth);
    m_parameters.add(m_textWidth);
    
    m_textLineHeight.set("Line Height", 1.0, 0.0, 3.0);
    m_textLineHeight.addListener(textManager, &TextManager::onChangeLineHeight);
    m_parameters.add(m_textLineHeight);
    
    m_textX.set("X", 0.5, 0.0, 1.0);
    m_textX.addListener(textManager, &TextManager::onChangePosX);
    m_parameters.add(m_textX);
    
    m_textY.set("Y", 0.5, 0.0, 1.0);
    m_textY.addListener(textManager, &TextManager::onChangePosY);
    m_parameters.add(m_textY);
    
    m_textZ.set("Z", 0.0, -1.0, 1.0);
    m_textZ.addListener(textManager, &TextManager::onChangePosZ);
    m_parameters.add(m_textZ);

    
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = m_gui.addFolder("TEXT", ofColor::cyan);
    folder->addLabel("Font Name:");
    folder->addSlider(m_textTransitionTime);
    folder->addSlider(m_textSize);
    folder->addSlider(m_textWidth);
    folder->addSlider(m_textLineHeight);
    folder->addSlider(m_textX);
    folder->addSlider(m_textY);
    folder->addToggle("Show Box");
    folder->expand();
    
    m_fontLabel = m_gui.getLabel("Font Name:");
}



void GuiManager::setupColorGui()
{
    m_red.set("R: ", 255, 0, 255);
    m_red.addListener(this, &GuiManager::onChangeColor);
    m_parameters.add(m_red);
    
    m_green.set("G: ", 255, 0, 255);
    m_green.addListener(this, &GuiManager::onChangeColor);
    m_parameters.add(m_green);
    
    m_blue.set("B: ", 255, 0, 255);
    m_blue.addListener(this, &GuiManager::onChangeColor);
    m_parameters.add(m_blue);
    
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = m_gui.addFolder("COLOR", ofColor::purple);
    folder->addSlider(m_red);
    folder->addSlider(m_green);
    folder->addSlider(m_blue);
    folder->expand();
}


void GuiManager::setupGuiEvents()
{
    m_gui.onDropdownEvent(this, &GuiManager::onDropdownEvent);
    m_gui.onColorPickerEvent(this, &GuiManager::onColorPickerEvent);
    m_gui.onButtonEvent(this, &GuiManager::onButtonEvent);
    m_gui.onToggleEvent(this, &GuiManager::onToggleEvent);
    m_gui.onMatrixEvent(this, &GuiManager::onMatrixEvent);
}

void GuiManager::update()
{
    m_gui.update();
}

void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    this->drawRectangle();
    
    //ofEnableSmoothing();
    ofEnableAlphaBlending();
        m_gui.draw();
    ofDisableAlphaBlending();
    //ofDisableSmoothing();
    
}


void GuiManager::saveGuiValues()
{
    ofXml xml;
    xml.serialize(m_parameters);
    xml.save(GUI_SETTINGS_FILE_NAME);
}

void GuiManager::loadGuiValues()
{
    ofXml xml(GUI_SETTINGS_FILE_NAME);
    xml.deserialize(m_parameters);
}


void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}

void GuiManager::drawRectangle()
{
    int margin =  LayoutManager::MARGIN;
    ofPushStyle();
    ofSetColor(15);
    ofDrawRectangle( m_gui.getPosition().x - margin, 0, m_gui.getWidth() + 2*margin, ofGetHeight());
    ofPopStyle();
}


void GuiManager::onChangeColor(int& value)
{
    m_currentColor.r = m_red;  m_currentColor.g = m_green; m_currentColor.b = m_blue;
    AppManager::getInstance().getTextManager().setColor(m_currentColor);
}


void GuiManager::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "SCENES")
    {
    //AppManager::getInstance().getSceneManager().changeScene(e.child);
        //m_gui.getDropdown(e.target->getName())->expand();
      //  m_gui.getDropdown(e.target->getName())->setLabel("SCENES:" + e.target->getLabel());
    }
    
}

void GuiManager::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getName() << " Selected" << endl;
    
    if (e.target->getName() == "COLOR MODE"){
        
    }
    
}

void GuiManager::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "* LOAD")
    {
        this->openSystemDialog();
    }

    else if(e.target->getName() == "* Save GUI")
    {
        this->saveGuiValues();
    }
    
    else if(e.target->getName() == "* RELOAD")
    {
        AppManager::getInstance().getSubtitlesManager().reloadFile();
    }
    
    else if(e.target->getName() == "* NEXT")
    {
        this->nextRow();
    }
    
    else if(e.target->getName() == "* PREVIOUS")
    {
        this->previousRow();
    }
}


void GuiManager::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "Show Box")
    {
        AppManager::getInstance().getTextManager().onChangeShowBox(e.target->getChecked());
    }
    
    else if(e.target->getName() == "Fullscreen")
    {
        AppManager::getInstance().getLayoutManager().onFullScreenChange(e.target->getChecked());
    }
    
    else if(e.target->getName() == "Syphon")
    {
        ofLogNotice()<< "GuiManager::onToggleEvent-> Syphon : " << e.target->getChecked();
        AppManager::getInstance().getLayoutManager().onSyphonEnable(e.target->getChecked());
    }
    else if(e.target->getName() == "Blackout")
    {
        ofLogNotice()<< "GuiManager::onToggleEvent-> Enable : " << e.target->getChecked();
        AppManager::getInstance().getLayoutManager().onSyphonToggle(e.target->getChecked());
    }

}

void GuiManager::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
    //cout << "onMatrixEvent " << e.child << " : " << e.enabled << endl;
    //cout << "onMatrixEvent " << e.target->getLabel() << " : " << e.target->getSelected().size() << endl;
    if(e.target->getLabel() == "Types")
    {
        //AppManager::getInstance().getAnimationsManager().onSetSelectedAnimations( e.target->getSelected());
    }
}

void GuiManager::openSystemDialog()
{
    //Open the Open File Dialog
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a csv file");
    
    //Check if the user opened a file
    if (openFileResult.bSuccess){
        
        ofLogNotice() << "GuiManager::openSystemDialog-> User selected a file: " << openFileResult.getName();
        
        AppManager::getInstance().getSubtitlesManager().loadFile(openFileResult.getPath());
        
    }else {
        ofLogNotice() << "GuiManager::openSystemDialog-> User hit cancel";
    }
}


void GuiManager::nextRow()
{
    if(m_subRow.get() < m_subRow.getMax())
    {
        m_subRow = m_subRow.get()  + 1;
    }
}

void GuiManager::previousRow()
{
    if(m_subRow.get() > m_subRow.getMin())
    {
        m_subRow = m_subRow.get()  - 1;
    }
}


void GuiManager::setNumCols(int value)
{
    if(value>0){
        m_subCol.setMax(value);
        m_gui.getSlider(m_subCol.getName())->setMax(value-1);
    }
}

void GuiManager::setNumRows(int value)
{
    if(value>0){
        m_subRow.setMax(value);
        m_gui.getSlider(m_subRow.getName())->setMax(value-1);
    }
}

void GuiManager::resetSubtitles()
{
    m_subRow = m_subRow.getMin();
    m_subCol = m_subCol.getMin();
}

void GuiManager::setSubtitlesName(const string& name)
{
    if(m_subLabel == NULL){
        return;
    }
    m_subLabel->setLabel("FILE: " + name);
}

void GuiManager::setFontLabel(const string& name)
{
    if(m_fontLabel == NULL){
        return;
    }
    
    m_fontLabel->setLabel("Font Name: " + name);
}


void GuiManager::setTextSize(int value)
{
    m_textSize = ofClamp(value, 0, 100);
}

void GuiManager::setTextLineHeight(float value)
{
    m_textLineHeight = ofClamp(value, 0, 3);
}


void GuiManager::setTextWidth(float value)
{
    m_textWidth = ofClamp(value, 0.0, 1.0);
}


void GuiManager::setTextPosX(float value)
{
    m_textX = ofClamp(value, 0.0, 1.0);
}


void GuiManager::setTextPosY(float value)
{
    m_textY = ofClamp(value, 0.0, 1.0);
}


void GuiManager::setColorR(int value)
{
    m_red = ofClamp(value, 0, 255);
}


void GuiManager::setColorG(int value)
{
    m_green = ofClamp(value, 0, 255);
}

void GuiManager::setColorB(int value)
{
    m_blue = ofClamp(value, 0, 255);
}

void GuiManager::setSubCol(int value)
{
    m_subCol = ofClamp(value, m_subCol.getMin(), m_subCol.getMax());
}


void GuiManager::setSubRow(int value)
{
    m_subRow = ofClamp(value, m_subRow.getMin(), m_subRow.getMax());
}

void GuiManager::setTextTransitionTime(float value)
{
    m_textTransitionTime = ofClamp(value, m_textTransitionTime.getMin(), m_textTransitionTime.getMax());
}

void GuiManager::setSyphonToggle(bool value)
{
    auto toggle = m_gui.getToggle("Syphon");
    toggle->setChecked(value);
    AppManager::getInstance().getLayoutManager().onSyphonEnable(value);
}

void GuiManager::setSyphonEnable(bool value)
{
    auto toggle = m_gui.getToggle("Blackout");
    toggle->setChecked(value);
    AppManager::getInstance().getLayoutManager().onSyphonToggle(value);
}

void GuiManager::setShowBox(bool value)
{
    auto toggle = m_gui.getToggle("Show Box");
    toggle->setChecked(value);
    AppManager::getInstance().getTextManager().onChangeShowBox(value);
}

