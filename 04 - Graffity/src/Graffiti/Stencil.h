//
//  Stencil.h
//  04 - Graffity
//
//  Created by Patricio Gonzalez Vivo on 1/30/13.
//
//

#ifndef STENCIL
#define STENCIL

#include "ofMain.h"

class Stencil : public ofRectangle {
public:
    
    Stencil();
    
    void            loadImage(string _imgFile);
    bool            checkClick(ofPoint _pos);
    
    void            update();
    void            draw();
    void            drawGui();
    
    ofEvent<int>    close;
    
    int             iconSize;
    int             nId;
    
private:
    ofImage img;
    
    ofImage iconMove;
    ofImage iconClose;
    ofImage iconResize;
};

#endif
