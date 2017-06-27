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
    
public:
    
    //! Constructor
    TextManager();
    
    //! Destructor
    ~TextManager();
    
    //! Setup the Halo Manager
    void setup();
    
    //! Update the Halo Manager
    void update();
    
    //! Draw the Halo Manager
    void draw();
    
    void  onChangeSize(int& value);
    
    void  onChangeWidth(float& value);
    
    void  onChangeLineHeight(float& value);
    
    void  onChangePos(ofVec2f& value);
    
    void  onChangePosX (float& value);
    
    void  onChangePosY (float& value);
    
    void  onChangeShowBox (bool value);
    
    
private:
    
    void setupText();
    
private:
    
    TextVisual  m_textVisual;    

};



