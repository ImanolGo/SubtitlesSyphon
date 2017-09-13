/*
 *  AppManager.h
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */

#pragma once

#include "ofMain.h"

#include "managers.h"


//========================== class AppManager ==============================
//============================================================================
/** \class AppManager AppManager.h
 *	\brief Class managing the whole application
 *	\details it set-ups, updates and renders all the different managers used for the application
 */

class AppManager: public Manager
{
public:

    //! Destructor
    ~AppManager();

    //! Returns the singleton instance.
    static AppManager& getInstance();

    //! Compares two transition objects
    void setup();

    //! updates the logic
    void update();

    //! calls the view manager to draw
    void draw();
    
    //! calls the view manager to draw onto the second display
    void drawDisplay();
    
    //! calls at exit
    void exit();

    //==========================================================================

    //! Returns the settings manager
    SettingsManager& getSettingsManager() { return m_settingsManager; }
    
    //! Returns the layout manager
    LayoutManager& getLayoutManager() { return m_layoutManager; }
    
    //! Returns the resource manager
    ResourceManager& getResourceManager() { return m_resourceManager; }
    
    //! Returns the view manager
    ViewManager&  getViewManager() { return m_viewManager; }
    
    //! Returns the visual effects manager
    VisualEffectsManager&  getVisualEffectsManager() { return m_visualEffectsManager; }
    
    //! Returns the GUI manager
    GuiManager&   getGuiManager() { return m_guiManager; }
    
    //! Returns the  OSC manager
    OscManager&  getOscManager() { return m_oscManager; }
    
    //! Returns the  text manager
    TextManager&  getTextManager() { return m_textManager; }
    
    //! Returns the  subtitles manager
    SubtitlesManager&  getSubtitlesManager() { return m_subtitlesManager; }
    
    //! Returns the  password manager
    PasswordManager&  getPasswordManager() { return m_passwordManager; }
    
    //==========================================================================
    
    void toggleDebugMode();
    
    void setDebugMode(bool showDebug);
    
    void onEnableDisplay(bool value);
    

private:

     //! Constructor
     AppManager();

	//! Stop the compiler generating methods of copy the object
	 AppManager(AppManager const& copy);              // Don't Implement

    //! Prevent operator= being generated.
     AppManager& operator=(AppManager const& copy);     // Don't implement

    //==========================================================================

    //! Set-up all the managers
    void setupManagers();

    //! Set-up openFrameworks
    void setupOF();
    
    void setupDisplay();

    //! update all the managers
    void updateManagers();

private:


    SettingsManager          m_settingsManager;          ///< Manages the application's settings
    ResourceManager          m_resourceManager;          ///< Manages the application's resources
    ViewManager              m_viewManager;              ///< Manages visuals
    VisualEffectsManager     m_visualEffectsManager;     ///< Manages visual effects
    LayoutManager            m_layoutManager;            ///< Manages the layout
    KeyboardManager          m_keyboardManager;          ///< Manages the keyboard
    MouseManager             m_mouseManager;             ///< Manages the mouse
    GuiManager               m_guiManager;               ///< Manages the graphical user interface
    OscManager               m_oscManager;               ///< Manages the OSC messages
    TextManager              m_textManager;              ///< Manages the text to be rendered
    SubtitlesManager         m_subtitlesManager;         ///< Manages the current subtitles
    PasswordManager          m_passwordManager;          ///< Manages the password input and app allowance
    
    bool                            m_debugMode;
    shared_ptr<ofAppBaseWindow>     m_displayWindow;
};

//==========================================================================


