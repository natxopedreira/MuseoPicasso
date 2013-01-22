//
//  Palette.h
//
//  Created by Patricio Gonzalez Vivo on 1/21/13.
//
//

#ifndef PALETTE
#define PALETTE

#include "ofMain.h"

class Palette : public ofRectangle {
public:
    
    void        load(string _sFile);
    void        setVisible(bool _bvisible);
    
    ofTexture&  getTextureReference();
    bool        isVisible();
    
    void        update();
    void        draw();
    
private:
    float       pct;
};

#endif
