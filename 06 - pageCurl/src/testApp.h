#pragma once

#include "ofMain.h"

#include "Book.h"

//#define USE_TUIO
#include "ofxOsc.h"
#include "ofxTuioClient.h"

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
	
    void _tuioAdded(ofxTuioCursor & tuioCursor);
	void _tuioRemoved(ofxTuioCursor & tuioCursor);
	void _tuioUpdated(ofxTuioCursor & tuioCursor);
    
    //  Input
    //
    ofxTuioClient   tuioClient;
    ofxOscReceiver  oscReceiver;
    
    ofLight pointLight;
    ofMaterial material;
    
    Book    book;
};
