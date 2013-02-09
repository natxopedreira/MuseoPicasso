//
//  Page.h
//
//  Based on Eric Brumer article about Project Austin:
//
//      http://blogs.msdn.com/b/vcblog/archive/2012/09/27/10348494.aspx
//

#pragma once

#include "ofMain.h"

// Helper macro for a straight line F(x) that passes through {x1, y1} and {x2, y2}.
// We can't make this a template function (C++ doesn't let you have float literals
// as template parameters).
#define STRAIGHT_LINE(x1, y1, x2, y2, x) (((y2 - y1) / (x2 - x1)) * (x - x1) + y1)

//--------------------------------------------------------------
//--------------------------------------------------------------
class Page : public ofRectangle {
public:
    
    Page();

    float   getTransition();
    void    setHandAt(ofPoint _hand);
    
    void    update();
    void    draw(bool _bDebug = false);
    
    int     meshDefinition;
    
private:
    void    addFace(ofMesh& mesh, ofPoint a, ofPoint b, ofPoint c, ofPoint d);
    void    addFace(ofMesh& mesh, ofPoint a, ofPoint b, ofPoint c);
    ofPoint getCurlPos(ofPoint pos);

    ofMesh  mesh;
    ofPoint hand;
    
    float   theta;				// angle of right-cone
    float   ay;					// location on y axis of cone axis
    float   angle;				// rotation about y axis
    float   conicContribution;	// how much of (south tip cone == -1, cylinder == 0, north tip cone == 1)
    
    float   posNegOne;
    
    float   cylR;
    float   cylRadius;
    
    bool    bChange;
};
