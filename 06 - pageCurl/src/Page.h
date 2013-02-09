//
//  Page.h
//
//  Original Code from Eric Brumer, Project Austin:
//
//      http://blogs.msdn.com/b/vcblog/archive/2012/09/27/10348494.aspx
//

#pragma once

#include "ofMain.h"

// Helper macro for a straight line F(x) that passes through {x1, y1} and {x2, y2}.
// We can't make this a template function (C++ doesn't let you have float literals
// as template parameters).
#define STRAIGHT_LINE(x1, y1, x2, y2, x) (((y2 - y1) / (x2 - x1)) * (x - x1) + y1)

struct coneParameters {
    coneParameters() {}
    coneParameters(float t, float a, float ang, float c) : theta(t), ay(a), angle(ang), conicContribution(c) {}
    
    void set( float x, float y){
        
//        if not ( abs(x) < 1.001 && abs(y) < 1.001) )
//            return;
        
        float theta, ay, angle;
        
        if (x > 0.95f){
            theta = STRAIGHT_LINE(1.0f,    90.0f,   0.95f,  60.0f, x);
            ay =    STRAIGHT_LINE(1.0f,   -20.0f,   0.95f,  -5.0f, x);
            angle = 0.0;
        } else if (x > 0.8333f) {
            theta = STRAIGHT_LINE(0.95f,   60.0f, 0.8333f,  55.0f, x);
            ay =    STRAIGHT_LINE(0.95f,   -5.0f, 0.8333f,  -4.0f, x);
            angle = STRAIGHT_LINE(0.95f,    0.0f, 0.8333f,  13.0f, x);
        } else if (x > 0.3333f) {
            theta = STRAIGHT_LINE(0.8333f, 55.0f, 0.3333f,  45.0f, x);
            ay =    STRAIGHT_LINE(0.8333f, -4.0f, 0.3333f,  -10.0f, x);
            angle = STRAIGHT_LINE(0.8333f, 13.0f, 0.3333f,  35.0f, x);
        } else if (x > 0.1666f) {
            theta = STRAIGHT_LINE(0.3333f, 45.0f, 0.1666f,  25.0f, x);
            ay =    STRAIGHT_LINE(0.3333f, -10.0f, 0.1666f, -30.0f, x);
            angle = STRAIGHT_LINE(0.3333f, 35.0f, 0.1666f,  60.0f, x);
        } else {
            theta = STRAIGHT_LINE(0.1666f, 25.0f,    0.0f,  20.0f, x);
            ay =    STRAIGHT_LINE(0.1666f, -30.0f,   0.0f, -40.0f, x);
            angle = STRAIGHT_LINE(0.1666f, 60.0f,    0.0f,  95.0f, x);
        }
    }
    
    float theta;				// angle of right-cone
    float ay;					// location on y axis of cone axis
    float angle;				// rotation about y axis
    float conicContribution;	// how much of (south tip cone == -1, cylinder == 0, north tip cone == 1)
};

//--------------------------------------------------------------
//--------------------------------------------------------------
class Page{
public:
    
    Page();

    void rebuild();
    void rebuild(float bendPct);
    void rebuild(float bendTopPct, float bendBottomPct);
    void remesh();

    void startUserCurl(float x, float y);
    
    void draw( bool _bDebug = false );

    ofPoint pos;

    ofPath  path;
    ofMesh  mesh;
    
    int     pageSize;
};
