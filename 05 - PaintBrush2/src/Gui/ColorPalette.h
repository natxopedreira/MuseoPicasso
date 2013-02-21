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
    void        setScheme(unsigned int _nId);
    void        setVisible(bool _bvisible);
    void        toggleVisible();
    
    void        clear();
    
    bool        getVisible();
    float       getOffset();
    bool        checkColor(ofPoint _pos, ofColor& _color);
    
    void        update();
    void        draw();
    
    int         maxOffSet;
    bool        bVertical;
    
private:
    
    ofImage         background;
    
    vector<ofImage> images;
    vector<ofImage> imagesTop;
    
    vector< vector<ofColor> > schemes;
    
    float           pct, inc, shp;
    float           margins;
    
    bool            bSetup;
    bool            bVisible;
    
    int             width,height,element_offset;
    int             nScheme;
    bool            bChange;
};

#endif
