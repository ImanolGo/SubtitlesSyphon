/*
 *  SubtitlesSyphonApp.cpp
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */



#include "AppManager.h"

#include "SubtitlesSyphonApp.h"

//--------------------------------------------------------------
void SubtitlesSyphonApp::setup(){
    ofSetDataPathRoot("../Resources/data/");
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void SubtitlesSyphonApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void SubtitlesSyphonApp::draw(){
    AppManager::getInstance().draw();
}

void SubtitlesSyphonApp::exit()
{
    ofLogNotice() <<"SubtitlesSyphonApp::exit";
    AppManager::getInstance().exit();
}

//--------------------------------------------------------------
void SubtitlesSyphonApp::keyPressed(int key){

}

//--------------------------------------------------------------
void SubtitlesSyphonApp::keyReleased(int key){

}

//--------------------------------------------------------------
void SubtitlesSyphonApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void SubtitlesSyphonApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void SubtitlesSyphonApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void SubtitlesSyphonApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void SubtitlesSyphonApp::windowResized(int w, int h)
{
    AppManager::getInstance().getLayoutManager().windowResized(w,h);
}

//--------------------------------------------------------------
void SubtitlesSyphonApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void SubtitlesSyphonApp::dragEvent(ofDragInfo dragInfo)
{
    AppManager::getInstance().getSubtitlesManager().loadFiles(dragInfo.files);
}
