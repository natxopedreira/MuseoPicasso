//
//  SprayCan.h
//  04 - Graffity
//
//  Created by Patricio Gonzalez Vivo on 1/30/13.
//
//

#ifndef SPRAYCAN
#define SPRAYCAN

#include "ofMain.h"
#include "PaintParticle.h"

class SprayCan {
public:
    
    SprayCan();
    
    void    addPaint(ofPoint _pos, float _w = 0, float _h = 0, int _id = 0 );
    
    void    clean();
    
    void    update();
    void    draw();
    
    ofColor paintColor;
    float   paintAreaSize;
    
private:
    vector<PaintParticle*>  paintParticles;
    ofImage brush;
};

#endif
