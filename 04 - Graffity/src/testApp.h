#ifndef _TESTAPP
#define _TESTAPP

#include "ofMain.h"

#include "ofxOsc.h"
#include "TuioClient.h" // Special implementation of ofxTuio that get the width and height of blobs

#include "ofxFX.h"

#include "SprayCan.h"
#include "Stencil.h"

#include "Button.h"
#include "ColorPalette.h"


class testApp : public ofBaseApp{
public:
	void        setup();
	void        update();
	void        draw();

	void        keyPressed  (int key);
	void        mousePressed(int x, int y, int button);
	void        mouseDragged(int x, int y, int button);
    void        mouseReleased();
	void        windowResized(int w, int h);
	void        dragEvent(ofDragInfo dragInfo);

    void        _tuioAdded(TuioCursor & tuioCursor);
	void        _tuioRemoved(TuioCursor & tuioCursor);
	void        _tuioUpdated(TuioCursor & tuioCursor);
    
    void        showPalette(int &_n );
    void        cleanCanvas(int &_n );
    
    void        addStencil(int &_n );
    void        closeStencil(int &_n );
    
    //  Input
    //
    TuioClient      tuioClient;
    ofxOscReceiver  oscReceiver;
    
    //  Stencil
    //
    ofShader        stencilShader;
    ofxSwapBuffer   stencilBuffer;
    vector<Stencil*> stencils;
    int             stencilIdCounter;
    
    //  Graffiti
    //
    SprayCan        spray;
    ofxBlend        canvas;
    
    ColorPalette    palette;
    
    SimpleBtn       paletteBtn;
    SimpleBtn       cleanBtn;
    
    int             iconSize, width, height;
    bool            bDebug;
};

#endif
