/*
 *  BulletsVisual.h
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 12/09/17.
 *
 */


#pragma once


#include "BasicVisual.h"
#include "ImageVisual.h"


//=========================== class BulletsVisual ==============================
//============================================================================
/** \class BulletsVisual BulletsVisual.h
 *	\brief Represents a visually encoded password
 *	\details The visual substites every character with a bullet character
 * 		 to publicly show a password
 */


static const double BULLET_ANIMATION_TIME = 0.1; ///< Defines the animation time needed for the bullet visuals

class BulletsVisual: public BasicVisual
{
    static const int MAX_NUMBER_BULLETS;       ///< Defines the maximum number of bullets that you can have

    public:

        //! Constructor
        BulletsVisual(ofVec3f pos, float width, float height, bool centred = false);

        //! Destructor
        virtual ~BulletsVisual();

        //! set up the text field visual
        virtual void setup();

        //! Draws the text field visual
        virtual void draw();

        //! Resets the bullet visuals to the starting state
        virtual void reset();

        //! Add one character to the password
        virtual bool addCharacter();

        //! Removes one character to the password
        virtual bool removeCharacter();

         //! Draws the text field visual
        float getTotalBulletsWidth() const {return m_totalBulletsWidth;}

    protected:

        //! Sets up the bullet's color
        virtual void setupBulletColor();

        //! Sets up the bullet's size
        virtual void setupBulletSize();

        //! Draws all the bullets from the list
        virtual void drawBullets();


    protected:

        typedef  vector< ofPtr<ImageVisual> >    ImageVector;      ///< defines a vector of Image Visuals

        ImageVector       m_bullets;            ///< Defines a list of bullet characters
        int               m_numberOfBullets;    ///< Defines the current number of bullets even if there are not shown
        bool              m_centred;            ///< determines whether the visual is centred or not

        float             m_singleBulletSize;    ///< stores a single bullet size
        float             m_totalBulletsWidth;   ///< stores the total width from all bullets

};

