/*
 *  EngineFont.h
 *
 *  Created by Imanol Gomez on 23/02/15.
 *
 */


#include <iostream>
#include "EngineFont.h"

EngineFont::EngineFont():m_lineHeight(1.0),m_batchDrawing(false),m_textBlockAlignment(OF_TEXT_ALIGN_LEFT)
{
	//Intentionally left empty
}

EngineFont::~EngineFont()
{
	//Intentionally left empty
}

bool EngineFont::setup( string fontFile, float fontSize, float lineHeightPercent)
{

    //m_trueTypeFont.setEncoding(OF_ENCODING_UTF8);

	m_lineHeight = lineHeightPercent;

	bool bAntiAliased=true;
    bool bFullCharacterSet=true;
    bool makeContours=false;
    float simplifyAmt=0.3;
    int dpi=0.0;
    
    string filePathName = getFilePathName(fontFile);
    
	//if(m_trueTypeFont.load(fontFile,fontSize,bAntiAliased,bFullCharacterSet,makeContours,simplifyAmt,dpi)){
    if(m_trueTypeFont.load(filePathName,fontSize,true, true)){
	    ofLogNotice() << "EngineFont::setup-> font loaded " << fontFile << " with size " << fontSize;
        return true;
	}
	else{
         ofLogNotice() << "EngineFont::setup-> Can't load font " << fontFile << "!!";
        return false;
	}

    return false;
}


string EngineFont::getFilePathName(string _fontname)
{
    std::string fontname = _fontname;
    string filename = ofToDataPath(fontname,true);
    ofFile fontFile(filename,ofFile::Reference);
    
    if(!fontFile.exists()){
#ifdef TARGET_LINUX
        filename = getFontFilePathByName(fontname);
#elif defined(TARGET_OSX)
        if(fontname==OF_TTF_SANS){
            fontname = "Helvetica Neue";
        }else if(fontname==OF_TTF_SERIF){
            fontname = "Times New Roman";
        }else if(fontname==OF_TTF_MONO){
            fontname = "Menlo Regular";
        }
        filename = getFontFilePathByName(fontname);
#elif defined(TARGET_WIN32)
        if(fontname==OF_TTF_SANS){
            fontname = "Arial";
        }else if(fontname==OF_TTF_SERIF){
            fontname = "Times New Roman";
        }else if(fontname==OF_TTF_MONO){
            fontname = "Courier New";
        }
        filename = getFontFilePathByName(fontname);
#endif
        if(filename == "" ){
            ofLogError("ofTrueTypeFont") << "loadFontFace(): couldn't find font \"" << fontname << "\"";
        }
        ofLogVerbose("ofTrueTypeFont") << "loadFontFace(): \"" << fontname << "\" not a file in data loading system font from \"" << filename << "\"";
    }
    
    return filename;
}




void EngineFont::draw( string text, float x, float y)
{
    if(!m_trueTypeFont.isLoaded()){
        return;
    }

    m_trueTypeFont.drawString(text,x,y);
}

void EngineFont::drawMultiLine( string text, float x, float y)
{
    if(!m_trueTypeFont.isLoaded()){
        return;
    }

    stringstream ss(text);
    string s;
    int line = 0;
    float yy = y;
    int fontSize = m_trueTypeFont.getFontSize();
    ///int fontSize = m_trueTypeFont.getSize();


    while ( getline(ss, s, '\n') ) {
        //cout << s << endl;
        yy = y + fontSize * m_lineHeight* FONT_HEIGHT_MULT * line;
        m_trueTypeFont.drawString(s.c_str(),x,yy);
        line ++;
    }
}

ofRectangle EngineFont::drawMultiLineColumn( string text, float x, float y, float columnWidth)
{

    ofRectangle totalArea = ofRectangle(x,y,0,0);

    if(!m_trueTypeFont.isLoaded()){
        return totalArea;
    }

    //int fontSize = m_trueTypeFont.getSize();
    int fontSize = m_trueTypeFont.getFontSize();

	vector<string>splitLines;
    ofRectangle r;

    ofUTF8Ptr start = ofUTF8::beginPtr(text);
    ofUTF8Ptr iter = ofUTF8::beginPtr(text);
    ofUTF8Ptr lineStart = iter;
    ofUTF8Ptr lastSpace;
    ofUTF8Ptr stop = ofUTF8::endPtr(text);

    string thisLine = "";
    bool foundSpace = false;
    bool foundNewLine = false;
    while(iter < stop) {

        ofUniChar c = ofUTF8::getNext(iter); // get the next unichar and iterate
        if ( ofUnicode::isSpace(c) ){
            foundSpace = true;
            lastSpace = iter;
        }
        if ( ofTextConverter::toUTF8(c) == "\n" ){
            foundNewLine = true;
        }
        thisLine += ofTextConverter::toUTF8(c);
        r = m_trueTypeFont.getStringBoundingBox(thisLine.c_str(), 0,0);

        if ( r.width > columnWidth || foundNewLine ) { //we went too far, lets jump back to our closest space
            if(foundNewLine){
                if (thisLine == "\n"){ //if the whole line is only \n, replace for space to avoid weird things
                    thisLine = " ";
                }else{	//otherwise remove the "\n"
                    thisLine = thisLine.substr(0, thisLine.length()-1);
                }
                splitLines.push_back(thisLine);

            }else{
                if (foundSpace){
                    //cout << "## foundSpace! (" << thisLine << ")" << endl;
                    string finalLine = walkAndFill(lineStart, iter, lastSpace);
                    splitLines.push_back(finalLine);
                    iter = lastSpace;
                }else{
                    //cout << "## no Space! (" << thisLine << ")" << endl;
                    splitLines.push_back(thisLine);
                }
            }
            //reset counter vars
            lineStart = iter;
            r.width = 0;
            thisLine = "";
            foundSpace = foundNewLine = false;
        }else{
            if(iter == stop){ //last line!
                string finalLine = walkAndFill(lineStart, iter, stop);
                splitLines.push_back(finalLine);
                break;
            }
        }
    }

    for(int i = 0; i < splitLines.size(); i++)
    {
        float yy = m_lineHeight * FONT_HEIGHT_MULT * fontSize * i;

        ofPushMatrix();

            if(m_textBlockAlignment == OF_TEXT_ALIGN_LEFT)
            {
                ofTranslate(0, yy);
            }

            else if(m_textBlockAlignment == OF_TEXT_ALIGN_CENTER)
            {
                ofRectangle r = m_trueTypeFont.getStringBoundingBox(splitLines[i], 0,0);
                ofTranslate(-r.getWidth()*0.5, yy);
            }
            else if(m_textBlockAlignment == OF_TEXT_ALIGN_RIGHT)
            {
                ofRectangle r = m_trueTypeFont.getStringBoundingBox(splitLines[i], 0,0);
                ofTranslate(-r.getWidth(), yy);
            }
            else
            {
                ofTranslate(0, yy);
            }

            m_trueTypeFont.drawString(splitLines[i],x,y);

        ofPopMatrix();

        totalArea = totalArea.getUnion( m_trueTypeFont.getStringBoundingBox(splitLines[i], x, y + yy));
    }

	return totalArea;
}

string EngineFont::walkAndFill(ofUTF8Ptr begin, ofUTF8Ptr & iter, ofUTF8Ptr end){

	string finalLine = "";
	ofUTF8Ptr i = begin;
	while (i < iter) { // re-fill the finalLine from the begining to the last Space
		finalLine += ofTextConverter::toUTF8(ofUTF8::getNext(i)); // get the next unichar and iterate
		if(i == end){
			break;
		}
	}
	return finalLine;
}


void EngineFont::setLineHeight(float percent){
	m_lineHeight = percent;
}


string EngineFont::getFontFilePathByName(string fontName)
{
#ifdef TARGET_OSX
    
    CFStringRef targetName = CFStringCreateWithCString(nullptr, fontName.c_str(), kCFStringEncodingUTF8);
    CTFontDescriptorRef targetDescriptor = CTFontDescriptorCreateWithNameAndSize(targetName, 0.0);
    CFURLRef targetURL = (CFURLRef) CTFontDescriptorCopyAttribute(targetDescriptor, kCTFontURLAttribute);
    string fontPath = "";
    
    if(targetURL) {
        UInt8 buffer[PATH_MAX];
        CFURLGetFileSystemRepresentation(targetURL, true, buffer, PATH_MAX);
        fontPath = std::string((char *)buffer);
        CFRelease(targetURL);
    }
    
    CFRelease(targetName);
    CFRelease(targetDescriptor);
    
    return fontPath;
    
#endif
    
#ifdef TARGET_WIN32
    
    map<std::string, std::string> fonts_table;
    
    LONG l_ret;
    
    const wchar_t *Fonts = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
    
    HKEY key_ft;
    l_ret = RegOpenKeyExW(HKEY_LOCAL_MACHINE, Fonts, 0, KEY_QUERY_VALUE, &key_ft);
    if (l_ret != ERROR_SUCCESS){
        ofLogError("ofTrueTypeFont") << "initWindows(): couldn't find fonts registery key";
        return;
    }
    
    DWORD value_count;
    DWORD max_data_len;
    wchar_t value_name[2048];
    BYTE *value_data;
    
    
    // get font_file_name -> font_face mapping from the "Fonts" registry key
    
    l_ret = RegQueryInfoKeyW(key_ft, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &value_count, nullptr, &max_data_len, nullptr, nullptr);
    if(l_ret != ERROR_SUCCESS){
        ofLogError("ofTrueTypeFont") << "initWindows(): couldn't query registery for fonts";
        return;
    }
    
    // no font installed
    if (value_count == 0){
        ofLogError("ofTrueTypeFont") << "initWindows(): couldn't find any fonts in registery";
        return;
    }
    
    // max_data_len is in BYTE
    value_data = static_cast<BYTE *>(HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, max_data_len));
    if(value_data == nullptr) return;
    
    char value_name_char[2048];
    char value_data_char[2048];
    /*char ppidl[2048];
     char fontsPath[2048];
     SHGetKnownFolderIDList(FOLDERID_Fonts, 0, nullptr, &ppidl);
     SHGetPathFromIDList(ppidl,&fontsPath);*/
    std::string fontsDir = getenv ("windir");
    fontsDir += "\\Fonts\\";
    for (DWORD i = 0; i < value_count; ++i)
    {
        DWORD name_len = 2048;
        DWORD data_len = max_data_len;
        
        l_ret = RegEnumValueW(key_ft, i, value_name, &name_len, nullptr, nullptr, value_data, &data_len);
        if(l_ret != ERROR_SUCCESS){
            ofLogError("ofTrueTypeFont") << "initWindows(): couldn't read registry key for font type";
            continue;
        }
        
        wcstombs(value_name_char,value_name,2048);
        wcstombs(value_data_char,reinterpret_cast<wchar_t *>(value_data),2048);
        std::string curr_face = value_name_char;
        std::string font_file = value_data_char;
        curr_face = curr_face.substr(0, curr_face.find('(') - 1);
        fonts_table[curr_face] = fontsDir + font_file;
    }
    
    
    HeapFree(GetProcessHeap(), 0, value_data);
    
    l_ret = RegCloseKey(key_ft);

    
    if(fonts_table.find(fontname)!=fonts_table.end()){
        return fonts_table[fontname];
    }
    for(map<std::string,std::string>::iterator it = fonts_table.begin(); it!=fonts_table.end(); it++){
        if(ofIsStringInString(ofToLower(it->first),ofToLower(fontname))) return it->second;
    }
    return "";
    
#endif
    
#ifdef TARGET_LINUX
   
    std::string filename;
    FcPattern * pattern = FcNameParse((const FcChar8*)fontName.c_str());
    FcBool ret = FcConfigSubstitute(0,pattern,FcMatchPattern);
    if(!ret){
        ofLogError() << "linuxFontPathByName(): couldn't find font file or system font with name \"" << fontname << "\"";
        return "";
    }
    FcDefaultSubstitute(pattern);
    FcResult result;
    FcPattern * fontMatch=nullptr;
    fontMatch = FcFontMatch(0,pattern,&result);
    
    if(!fontMatch){
        ofLogError() << "linuxFontPathByName(): couldn't match font file or system font with name \"" << fontname << "\"";
        FcPatternDestroy(fontMatch);
        FcPatternDestroy(pattern);
        return "";
    }
    FcChar8	*file;
    if (FcPatternGetString (fontMatch, FC_FILE, 0, &file) == FcResultMatch){
        filename = (const char*)file;
    }else{
        ofLogError() << "linuxFontPathByName(): couldn't find font match for \"" << fontname << "\"";
        FcPatternDestroy(fontMatch);
        FcPatternDestroy(pattern);
        return "";
    }
    FcPatternDestroy(fontMatch);
    FcPatternDestroy(pattern);
    return filename;
    
#endif


}



