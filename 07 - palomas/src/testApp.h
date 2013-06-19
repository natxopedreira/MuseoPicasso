#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "DragRect.h"

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
	
    ofVideoGrabber			video;
    ofxCvColorImage			videoColorCvImage;
    ofxCvGrayscaleImage		videoGrayscaleCvImage;
    ofxCvGrayscaleImage		videoPrevFrameImage;
    ofxCvGrayscaleImage		videoDiffImage;

    ofVideoPlayer           pidginLoop, pidginFly;
    
    DragRect                area;
    
    float                   nChange, nToFly;
    
    int                     threshold;
    int                     width, height;
    
    bool                    bDebug, bToFly;
};
