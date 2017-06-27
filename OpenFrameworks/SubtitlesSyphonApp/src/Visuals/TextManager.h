/*
 *  StageManager.h
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */


#pragma once

#include "Manager.h"
#include "ImageVisual.h"
#include "ofxSyphon.h"

//========================== class StageManager ==============================
//============================================================================
/** \class StageManager StageManager.h
 *	\brief Class managing the stage visuals
 *	\details it creates and updates the stage visuals and its output
 */


class StageManager: public Manager
{
    
public:
    
    //! Constructor
    StageManager();
    
    //! Destructor
    ~StageManager();
    
    //! Setup the Halo Manager
    void setup();
    
    //! Update the Halo Manager
    void update();
    
    //! Draw the Halo Manager
    void draw();
    
    float getWidth() const {return m_costumeImage->getOriginalWidth();}
    
    float getHeight() const {return m_costumeImage->getOriginalHeight();}
    
    void setupFbo();
    
    void  onChangeTrailEffect(bool value) {m_trail = value;}
    
    void  onChangeLiquifyEffect(bool value) {m_liquify = value;}
    
    void onChangeBrihtnessEffect(bool value){m_brightnessEffect = value;}
    
    void  windowResized(int w, int h);
    
private:
    
    void setupImages();
    
    void setupShader();

    //! Set-up the syphon server
    void setupSyphon();
    
    //! updates the syphon textture to be published
    void updateSyphonTexture();
    
    
    ofPtr<ImageVisual>  m_costumeImage;

    ofxSyphonServer     m_syphonServer;
    ofRectangle         m_windowRect;
    
    ofFbo               m_fbo;
    ofFbo               m_syphonFbo;
    ofShader            m_liquifyShader;
    
    bool                m_trail;
    bool                m_liquify;
    bool                m_brightnessEffect;
  
    float               m_frequency, m_amplitude, m_speed; //shader parameters
};



