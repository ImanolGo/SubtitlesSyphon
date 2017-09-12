/*
 *  WindowSettingsManager.cpp
 *  Subtitles Syphon
 *
 *  Created by Imanol Gomez on 11/08/17.
 *
 */

#include "ofMain.h"


#include "WindowSettingsManager.h"

const int WindowSettingsManager::MAX_NUM_WINDOWS = 2; ///Defines the maximum number of windows available

WindowSettingsManager& WindowSettingsManager::getInstance()
{
    // The only instance
    // Guaranteed to be lazy initialized
    // Guaranteed that it will be destroyed correctly
    static WindowSettingsManager    m_instance;
    return m_instance;
    
}

WindowSettingsManager::WindowSettingsManager()
{
    this->setup();
}


WindowSettingsManager::~WindowSettingsManager()
{
    ofLogNotice() <<"WindowSettingsManager::Destructor";
}


void WindowSettingsManager::setup()
{
    ofLogNotice() << "WindowSettingsManager::initialized";
    this->readSettings();
}

void WindowSettingsManager::readSettings()
{
    // Get screen widths and heights from Quartz Services
    // See https://developer.apple.com/library/mac/documentation/GraphicsImaging/Reference/Quartz_Services_Ref/index.html
    
    CGDisplayCount displayCount;
    CGDirectDisplayID displays[32];
    
    // Grab the active displays
    CGGetActiveDisplayList(32, displays, &displayCount);
    int numDisplays= displayCount;
    
    for(int displayID = 0; displayID<numDisplays; displayID++)
    {
        //WindowSettingsPtr settings = WindowSettingsPtr (new ofGLFWWindowSettings());
        ofGLFWWindowSettings settings;
        settings.height = CGDisplayPixelsHigh ( displays[displayID] );
        settings.width = CGDisplayPixelsWide ( displays[displayID] );
        if(displayID == 1){
            settings.decorated = false;
        }
        
        
        CGRect displayBounds= CGDisplayBounds (displays[displayID]);
        settings.setPosition(ofVec2f(displayBounds.origin.x, displayBounds.origin.y));
        
        
        m_windows.push_back(settings);
        
    }
    
    ofGLFWWindowSettings settings = m_windows.front();
    settings.decorated = false;
    settings.setPosition(ofVec2f(settings.width, 0));

    for(int displayID = numDisplays; displayID<MAX_NUM_WINDOWS; displayID++)
    {
        m_windows.push_back(settings);
    }
    
    
    ofLogNotice() << "WindowSettingsManager::readSettings -> Displays detected: " <<  numDisplays;
    
    int i = 0;
    for (auto windowSettings: m_windows)
    {
        ofLogNotice() << "WindowSettingsManager::readSettings -> Window " <<  i << ": x = " << windowSettings.getPosition().x
        << ", y = " << windowSettings.getPosition().y << ", width = " << windowSettings.width << ", height = " << windowSettings.height;
        i++;
    }
    
}

