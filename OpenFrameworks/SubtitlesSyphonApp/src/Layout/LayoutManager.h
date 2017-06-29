/*
 *  LayoutManager.h
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */

#pragma once

#include "Manager.h"
#include "TextVisual.h"
#include "SvgVisual.h"
#include "RectangleVisual.h"

#include "ofxSyphon.h"

//========================== class LayoutManager ==============================
//============================================================================
/** \class LayoutManager LayoutManager.h
 *	\brief Class managing the layout of the application
 *	\details It creates an places al the text and elements regarding the layout
 */

class LayoutManager: public Manager
{
public:
    
    static const int MARGIN;
    static const int FRAME_MARGIN;

    //! Constructor
    LayoutManager();

    //! Destructor
    ~LayoutManager();

    //! Set-up the layout
    void setup();
    
    //! Update the layout
    void update();
    
    //! Draw the layout
    void draw();
    
    void onFullScreenChange(bool value);
    
    void windowResized(int w, int h);
    
    void onSyphonToggle(bool value) {m_syphonToggle = value;}
    
    void onSyphonEnable(bool value);

private:


    //! Create the text visuals
    void createTextVisuals();

    //! Create the svg visuals
    void createSvgVisuals();

    //! Create the image visuals
    void createImageVisuals();

    //! Create the background Image
    void createBackground();


    //! Add all visuals as overlays
    void addVisuals();

    //! Set-up the syphon server
    void setupSyphon();
    
    void setupFbo();
    
    void setupRectangle();
    
    //! updates the syphon textture to be published
    void updateSyphonTexture();
    
    void drawFbos();
    
    void drawCurrentFbo();
    
    void drawPreviewFbo();
    
    void drawRectangles();

private:


    typedef  map<string, ofPtr<TextVisual> >      TextMap;            ///< defines a map of Text attached to an identifier
    typedef  map<string, ofPtr<SvgVisual>  >      SvgMap;             ///< defines a map of SvgVisual Map attached to an identifier
    typedef  map<string, ofPtr<ImageVisual>  >    ImageMap;           ///< defines a map of ImageVisual Map attached to an identifier
  
    TextMap             m_textVisuals;             ///< map storing the text visuals attached to a name
    SvgMap              m_svgVisuals;              ///< map storing the svg visuals attached to a name
    ImageMap            m_imageVisuals;            ///< map storing the image visuals attached to a name
    
    ofxSyphonServer     m_syphonServer;
    ofRectangle         m_currentWindowRect;
    ofRectangle         m_previewWindowRect;
    RectangleVisual     m_liveRectangle;
    RectangleVisual     m_currentFboRectangle;
    RectangleVisual     m_previewFboRectangle;
    
    ofFbo               m_currentFbo;
    ofFbo               m_previewFbo;
    ofFbo               m_syphonFbo;
    
    bool                m_syphonToggle;
    bool                m_syphonEnable;

};

//==========================================================================


