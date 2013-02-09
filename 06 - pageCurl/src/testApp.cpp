#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofBackground(0);
    
    ofAddListener(tuioClient.cursorAdded,this,&testApp::_tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&testApp::_tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&testApp::_tuioUpdated);
	tuioClient.start(3333);
    
    ofSetSmoothLighting(true);
    pointLight.setPosition(ofGetWidth()*0.5, ofGetHeight()*0.5, -1000);
    pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    
	material.setShininess(64);
	material.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    
    int width = 480*0.5;
    int height = 640*0.5;
    page.set(ofGetWidth()*0.5, ofGetHeight()*0.5-height*0.5, width, height);
}

//--------------------------------------------------------------
void testApp::update(){
    tuioClient.getMessage();
    
    page.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    if (ofGetKeyPressed()){
        page.draw(true);
        tuioClient.drawCursors();
    }
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
    ofEnableLighting();
    pointLight.enable();
    material.begin();
    
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    
    page.draw();
    
    material.end();
    ofDisableLighting();
    
    glDisable(GL_DEPTH_TEST);
    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
#ifndef USE_TUIO
    page.setHandAt(ofPoint(x,y));
#endif
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//----------------------------------------------------------- TUIO
void testApp::_tuioAdded(ofxTuioCursor & tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),
                          tuioCursor.getY()*ofGetHeight(),
                          0.0);

}

void testApp::_tuioUpdated(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),
                          tuioCursor.getY()*ofGetHeight(),
                          0.0);
    
#ifdef USE_TUIO
    page.setNormHandAt(ofPoint(tuioCursor.getX(),tuioCursor.getY()));
#endif
    
}

void testApp::_tuioRemoved(ofxTuioCursor & tuioCursor){
    ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),
                          tuioCursor.getY()*ofGetHeight(),
                          0.0);
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}