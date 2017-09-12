#include "ofMain.h"
#include "WindowSettingsManager.h"
#include "SubtitlesSyphonApp.h"

//========================================================================
int main( ){
    
//    ofGLFWWindowSettings settingsGL;
//    settingsGL.width = 900;
//    settingsGL.height = 900;
//    settingsGL.doubleBuffering = true;
//    settingsGL.windowMode = OF_WINDOW;
//    settingsGL.setGLVersion(2, 1);
//    
//    ofCreateWindow(settingsGL);
//    
//    ofRunApp( new SubtitlesSyphonApp());
//    
    
    WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();

    
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(windowSettings[0]);
    
    windowSettings[1].shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> secondWindow = ofCreateWindow(windowSettings[1]);
    
    //secondWindow->setVerticalSync(false);
    
    shared_ptr<SubtitlesSyphonApp> mainApp(new SubtitlesSyphonApp);
    ofAddListener(secondWindow->events().draw,mainApp.get(),&SubtitlesSyphonApp::drawScreen2);
    
    ofRunApp(mainWindow, mainApp);
    
    //mainWindow->setWindowShape(size, size);
    ofRunMainLoop();
//
}
