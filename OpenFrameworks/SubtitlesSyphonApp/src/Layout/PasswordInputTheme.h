/*
 *  PasswordInputTheme.h
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 01/08/17.
 *
 */


#pragma once

#include "ofxDatGuiTheme.h"
#include "LayoutManager.h"

//========================== class PasswordInputTheme ==============================
//============================================================================
/** \class PasswordInputTheme PasswordInputTheme.h
 *	\brief Costume password input theme
 *	\details Based on ofxDatGuiTheme
 */


class PasswordInputTheme : public ofxDatGuiTheme{
    
public:
    
    PasswordInputTheme()
    {
        layout.width = 800;
        layout.height = 40;
        layout.labelWidth = 150;
        layout.upperCaseLabels = false;
        layout.textInput.forceUpperCase = false;
        //font.file =  AssetPath + "fonts/open-sans/OpenSans-Regular.ttf";
        font.file = LayoutManager::LAYOUT_FONT;
        font.size = 16;
        color.matrix.normal.button = hex(0x6E6E6E);
        color.matrix.hover.button = hex(0x9C9DA1);
        color.matrix.selected.button = hex(0x2FA1D6);
        
        init();
    }
};
