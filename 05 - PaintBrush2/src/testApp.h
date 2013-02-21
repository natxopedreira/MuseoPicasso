#pragma once

#define CORRECTNORMAL   // MAKE IT GRAYSCALE 

#include "ofMain.h"

#include "ofxGui.h"
#include "ofxFX.h"

#include "ofxOsc.h"
#include "TuioClient.h" // Special implementation of ofxTuio that get the width and height of blobs

#include "Brush.h"
#include "ColorPalette.h"
#include "Button.h"

//  Reference:
//              http://www.youtube.com/watch?v=ow7eEWh37iU
//              http://www.youtube.com/watch?v=UOMI1JKfWwc
//

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void showPalette(int &_n );
    void cleanCanvas(int &_n );
    
    void _tuioAdded(TuioCursor & tuioCursor);
	void _tuioRemoved(TuioCursor & tuioCursor);
	void _tuioUpdated(TuioCursor & tuioCursor);

    //  Input
    //
    TuioClient      tuioClient;
    ofxOscReceiver  oscReceiver;
    
    //  GUI
    //
    ofxPanel        gui;
    ofxIntSlider    brushWidth;
    ofxIntSlider    brushNumber;
    ofxFloatSlider  brushDamp;
    ofxFloatSlider  brushK;
    ofxFloatSlider  brushRepRad;
    ofxFloatSlider  brushRepPct;
    ofxFloatSlider  lineWidth;
    ofxFloatSlider  colorLerp;
    ofxFloatSlider  colorRandom;
    ofxFloatSlider  alphaThreshold;

    vector<Brush*>  brushes;
    ofFbo           canvas;
    
    ColorPalette    palette;
    
    SimpleBtn       paletteProfBtn;
    SimpleBtn       paletteLandBtn;
    SimpleBtn       cleanBtn;

    ofColor         color;
    int             iconSize, idCounter, mouseId;
    bool            bDebug;
};
