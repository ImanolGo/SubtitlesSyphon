#include "ofMain.h"
#include "SubtitlesSyphonApp.h"

//========================================================================
int main( ){

    ofGLFWWindowSettings settingsGL;
    settingsGL.width = 900;
    settingsGL.height = 900;
    settingsGL.doubleBuffering = true;
    settingsGL.windowMode = OF_WINDOW;
    settingsGL.setGLVersion(3, 2);
    
    ofCreateWindow(settingsGL);

	ofRunApp( new SubtitlesSyphonApp());

}
