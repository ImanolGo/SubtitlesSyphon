
 /*
 *  TextField.h
 *  Subtitles Syphon
 *
 *  Created by Imanol Gomez on 12/09/17.
 *
 */

#pragma once

#include "RectangleVisual.h"
#include "TextVisual.h"

#include "BasicVisual.h"
#include "ImageVisual.h"
#include "BulletsVisual.h"


//=========================== class TextField ==============================
//============================================================================
/** \class TextField TextField.h
 *	\brief Represents a text field
 *	\details Graphical interface used to write on the password and forward
 * 		 the string
 */

class TextField: public BasicVisual
{
    static const double CURSOR_ANIMATION_TIME;  ///< cursor's animation time

    public:

        //! Constructor
        TextField(ofVec3f pos, bool centred = false);

        //! Destructor
        virtual ~TextField();

        //! Draws the text field visual
        virtual void draw();

    
        //! Callback functions when key is pressed
        void keyPressed(ofKeyEventArgs &e);
    
        //! Callback functions when key is released
        void keyReleased(ofKeyEventArgs &e);

    protected:

        //! set up the text field visual
        virtual void setup();

        //! Add listeners
        virtual void addListeners();

        //! Callback function used when a reset application event is given
        void resetTextField();

        //! Draws all visuals
        void drawVisuals();

        //! Updates the text visual and returns true if it was correctly done
        bool updateText(int key);

        //! Adds a character to the password
        bool addCharacter(int key);

        //! Removes a character from the password
        bool removeCharacter(int key);

        //! Updates the background text visual color
        void updateBackgroundTextColor();

        //! Create the text visuals
        void createTextVisuals();

        //! Create the background text
        void createBackgroundText();

        //! Create the rectangle visuals
        void createRectangleVisuals();

        //! Create the image visuals
        void createImageVisuals();

        //! Create the text field image
        void createTextFieldImage();

        //! Create the bullets visual that will represent the encoded password
        void createBullets();

        //! Create the cursor visual
        void createCursor();

        //! Updates the cursor's animation
        void updateCursorAnimation();

        //! Updates the cursor's position
        void updateCursorPosition();

        //! Resets the cursor's animation
        void resetCursorAnimation();

    protected:

        typedef  map<string, ofPtr<TextVisual> >        TextMap;            ///< defines a map of Texts attached to an identifier
        typedef  map<string, ofPtr<ImageVisual> >       ImageMap;           ///< defines a map of Images attached to an identifier
        typedef  map<string, ofPtr<RectangleVisual> >   RectangleMap;       ///< defines a map of Rectangles attached to an identifier
        typedef  vector<int>                            AsciiVector;        ///< defines a vector of ASCII according to ISO 8859-1
        typedef  map<int,string>                        AsciiMap;           ///< Defines a map of ASCII related to their charachter

        TextMap                 m_textVisuals;       ///< text to be visualized
        ImageMap                m_imageVisuals;      ///< images to be visualized
        RectangleMap            m_rectangleVisuals;  ///< rectangles used for the frame
        string                  m_asciiText;         ///< vector saving the ascii values of the text
        AsciiMap                m_asciiGerman;       ///< saves a map of all the german latin extended characters
        ofPtr<BulletsVisual>    m_bullets;           ///< visual representing the visualy encoded (with bullets •) password
        ofPtr<ImageVisual>      m_textField;           ///< visual representing the visualy encoded (with bullets •) password

        bool              m_centred;            ///< determines whether the visual is centred or not
        bool              m_initialized;        ///< determines whether the visual is initialized or not
        double            m_cursorElapsedTime;  ///< the elapsed time of the cursor animation
        float             m_margin;             ///< saves the margin between frame and text

};

