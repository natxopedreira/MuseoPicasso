#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "DragRect.h"
#include "ofxUI.h"
#include "ofxInteractiveViewPort.h"


//#define IPVIDEO

#ifdef IPVIDEO
    #include "ofxIpVideoGrabber.h"
#endif

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

#ifdef IPVIDEO
    ofxIpVideoGrabber       videoIp;
#else
    ofVideoGrabber			video;
#endif




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
    ofxInteractiveViewPort  viewport;
    ofFbo   fboSalida;

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
