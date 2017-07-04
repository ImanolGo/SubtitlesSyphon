/*
 *  SubtitlesManager.cpp
 *  Subtitles Syphon 
 *
 *  Created by Imanol Gomez on 27/06/17.
 *
 */

#include "SubtitlesManager.h"
#include "AppManager.h"


SubtitlesManager::SubtitlesManager(): Manager(), m_currentRow(0), m_currentCol(0)
{
    //Intentionally left empty
}

SubtitlesManager::~SubtitlesManager()
{
   ofLogNotice() << "SubtitlesManager::destructor";
}


//--------------------------------------------------------------

void SubtitlesManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    //this->loadFile("csvs/transtest.csv");
    
    ofLogNotice() <<"SubtitlesManager::initialized" ;
}

bool SubtitlesManager::loadFile(string path)
{
    // Load a CSV File.
    if(m_csv.load(path))
    {
        ofLogNotice() <<"SubtitlesManager::loadFile -> Successfully loaded: "<< path ;
        ofLogNotice() <<"SubtitlesManager::loadFile -> csv rows: "<< m_csv.getNumRows() << ", csv cols: " << m_csv.getNumCols();
        
        AppManager::getInstance().getGuiManager().setNumCols(m_csv.getNumCols());
        AppManager::getInstance().getGuiManager().setNumRows(m_csv.getNumRows());
        AppManager::getInstance().getGuiManager().setSubtitlesName(this->getName(path));
        AppManager::getInstance().getGuiManager().resetSubtitles();
        return true;
        
    }
    else
    {
        ofLogNotice() <<"SubtitlesManager::loadFile -> Unable to load : "<< path ;
         return false;
    }
}

bool SubtitlesManager::reloadFile()
{
    return loadFile(m_csv.getPath());
}


void SubtitlesManager::loadFiles(vector <string>& files)
{
    for(unsigned int i = 0; i < files.size(); i++)
    {
        if(this->loadFile(files[i]))
        {
            break;
        }
    }
}

void SubtitlesManager::onChangeCol(int& value)
{
    if(value<0 || value>= m_csv.getNumCols())
    {
        //ofLogNotice() <<"SubtitlesManager::onChangeCol -> out of col range: "<< value;
        return;
    }
    
    if(m_currentCol == value){
        return;
    }
    
    m_currentCol = value;
    
    AppManager::getInstance().getTextManager().setCurrentText(m_csv.getRow(m_currentRow).getString(m_currentCol));
    //ofLogNotice() <<"SubtitlesManager::onChangeCol -> text "<< m_csv.getRow(m_currentRow).getString(m_currentCol);
    
    
    int nextRow = m_currentRow + 1;
    string nextText = "";
    if(nextRow < m_csv.getNumRows())
    {
        nextText = m_csv.getRow(nextRow).getString(m_currentCol);
    }
    
    AppManager::getInstance().getTextManager().setNextText(nextText);
}

void SubtitlesManager::onChangeRow(int& value)
{
    if(value<0 || value>= m_csv.getNumRows())
    {
        //ofLogNotice() <<"SubtitlesManager::onChangeRow -> out of row range: "<< value;
        return;
    }
    
    if(m_currentRow == value){
        return;
    }
    
    ofLogNotice() <<"SubtitlesManager::onChangeCol -> "<< m_csv.getRow(m_currentRow).getString(m_currentCol);
    
    m_currentRow = value;
    AppManager::getInstance().getTextManager().setCurrentText(m_csv.getRow(m_currentRow).getString(m_currentCol));
    
    
    int nextRow = m_currentRow + 1;
    string nextText = "";
    if(nextRow < m_csv.getNumRows())
    {
        nextText = m_csv.getRow(nextRow).getString(m_currentCol);
    }
    
    AppManager::getInstance().getTextManager().setNextText(nextText);
}


string SubtitlesManager::getName(const string& path)
{
    return ofSplitString(path, "/").back();
}



