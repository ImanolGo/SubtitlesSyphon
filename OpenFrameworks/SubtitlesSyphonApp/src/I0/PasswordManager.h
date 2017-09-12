/*
 *  PasswordManager.h
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 01/08/17.
 *
 */

#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "TextField.h"

//========================== class PasswordManager =======================================
//==============================================================================
/** \class PasswordManager PasswordManager.h
 *	\brief class for managing the initial password
 *	\details It blocks the application to start until the correct password is given
 */


class PasswordManager: public Manager
{
    static const string KEY;

public:
    //! Constructor
    PasswordManager();

    //! Destructor
    virtual ~PasswordManager();

    //! setups the subtitles
    void setup();
    
    void update();
    
    void draw();
    
    bool getAllowApp() const {return m_allowApp;}
    
    void windowResized(int w, int h);
    
    void enterPassword(const string& password);
    
private:
    
    void setupInput();
    
private:
    
    bool    m_allowApp; ///< Defines whether to allow the application to continue or not
    
    ofPtr<TextField>       m_textField;
    ofFbo                  m_fbo;
    ofRectangle            m_boudingBox;
};

