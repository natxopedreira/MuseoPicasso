#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "DragRect.h"
#include "ofxUI.h"

enum estado{
    APP_STATE_HUNTER,
    APP_STATE_PHOVIEWER
};

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
    
    void exit();
	
    ofVideoGrabber			video;
    ofxCvColorImage			videoColorCvImage;
    ofxCvGrayscaleImage		videoGrayscaleCvImage;
    ofxCvGrayscaleImage		videoPrevFrameImage;
    ofxCvGrayscaleImage		videoDiffImage;

    ofVideoPlayer           pidginLoop, pidginFly;
    
    DragRect                area;
    
    float                   nChange, nToFly;

    
    float                     threshold;
    int                     width, height;
    
    bool                    bDebug, bToFly;
    
    // -- natxo
    
    ofVideoPlayer   photoViewer;
    estado  aplicacionEnCurso;
    void        updateStateHunter();
    
    
    /// --- gui
    ofxUISuperCanvas    * gui;
    void guiEvent(ofxUIEventArgs &e);
    
    float                   mapInMin,mapInMax,mapOutMax,mapOutMin;
    ofImage     diffMov,videoGui;
    ofFbo       videoAreaGui;
    void        initGui();
    
    float       horaInicio, horaFin;
    
    ofPoint     areaSupIzda, areaInfDcha;
};
