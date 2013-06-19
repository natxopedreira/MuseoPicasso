//
//  DragRect.cpp
//
//  Created by Patricio Gonzalez Vivo on 10/5/12.
//
//

#ifndef DRAGRECT
#define DRAGRECT

#include "ofMain.h"

#include "ofxXmlSettings.h"

class DragRect : public ofRectangle {
public:
    DragRect();
    
    bool    loadSettings(string _fileConfig = "config.xml");
    bool    saveSettings(string _fileConfig = "config.xml");
    
    void    init(int _x = -1, int _y = -1, int _width = -1, int _height = -1);
    
    bool    bEditMode;
    string  objName;

protected:
    void    _mouseDragged(ofMouseEventArgs &e);
    void    _mouseReleased(ofMouseEventArgs &e);
    void    _draw(ofEventArgs &e);
};

#endif
