//
//  Palette.h
//
//  Created by Patricio Gonzalez Vivo on 1/21/13.
//
//

#ifndef PALETTE
#define PALETTE

#include "ofMain.h"

#include "ofxXmlSettings.h"

class ColorPalette : public ofFbo {
public:
    
    ColorPalette();
    
    bool        loadPalette(string _sFile);
    void        setVisible(bool _bvisible);
    void        toggleVisible();
    
    void        clear();
    
    bool        getVisible();
    float       getY();
    bool        checkColor(ofPoint _pos, ofColor& _color);
    
    void        update();
    void        draw();
    
private:
    
    ofImage         background;
    
    vector<ofImage> images;
    vector<ofImage> imagesTop;
    vector<ofColor> colors;
    float           pct, inc, shp;
    float           margins;
    
    bool            bSetup;
    bool            bVisible;
    
    int             width,height,element_offset;
    bool            bChange;
};

#endif
