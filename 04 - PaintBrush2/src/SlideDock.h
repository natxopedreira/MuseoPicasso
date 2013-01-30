//
//  SlideDock.h
//
//  Created by Patricio Gonzalez Vivo on 1/27/13.
//
//

#ifndef SLIDEDOCK
#define SLIDEDOCK

#include "ofMain.h"
#include "ofxXmlSettings.h"

class SlideDock {
public:
    
    SlideDock();
    
    void        toggleVisible();
    void        setVisible(bool _bvisible);
    
    bool        isVisible();
    
    float       getY();
    
    void        update();
    void        draw();
    
private:
    ofImage         background;
    
    float           pct, inc, shp;
    int             width,height;
    
    bool            bSetup, bVisible;
};

#endif
