/*
 *  SubtitlesManager.h
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */

#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ofxCsv.h"

//========================== class SubtitlesManager =======================================
//==============================================================================
/** \class SubtitlesManager SubtitlesManager.h
 *	\brief class for managing the subtitles
 *	\details It reads from a CSV file with the subtitles and changes the text whenever a new line is selected
 */


class SubtitlesManager: public Manager
{

public:
    //! Constructor
    SubtitlesManager();

    //! Destructor
    virtual ~SubtitlesManager();

    //! setups the subtitles
    void setup();
    
    //! loads a file given a path
    bool loadFile(string path);
    
    //! loads a file given a list of files
    void loadFiles(vector <string>& files);
    
    void onChangeCol(int& value);
    
    void onChangeRow(int& value);
    
private:
    
    string getName(const string& path);
    
private:
    
    ofxCsv m_csv;
    
    int m_currentRow;
    int m_currentCol;
};

