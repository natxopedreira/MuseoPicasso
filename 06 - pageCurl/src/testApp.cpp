#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
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
    
    book.loadDirectory("pages");
    book.setPosition(ofPoint(ofGetWidth()*0.5, ofGetHeight()*0.5));
}

//--------------------------------------------------------------
void testApp::update(){
    tuioClient.getMessage();
    
#ifndef USE_TUIO
    book.hand = ofPoint(mouseX, mouseY);
#endif
    
    book.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    if (ofGetKeyPressed()){
        tuioClient.drawCursors();
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glDepthMask(GL_TRUE);
    
    ofEnableLighting();
    pointLight.enable();
    material.begin();
    
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    
    glEnable(GL_SMOOTH);
	glShadeModel(GL_SMOOTH);
    
    book.draw( ofGetKeyPressed() );
    
    material.end();
    ofDisableLighting();
    
	glDisable(GL_NORMALIZE);
    glDisable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == OF_KEY_RIGHT) {
        book.setNext();
    } else if ( key == OF_KEY_LEFT){
        book.setPrev();
    } else if ( key == 'f' ){
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
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
    book.hand = loc;
#endif
    
}

void testApp::_tuioRemoved(ofxTuioCursor & tuioCursor){
    ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),
                          tuioCursor.getY()*ofGetHeight(),
                          0.0);
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    book.setPosition(ofPoint(w*0.5, h*0.5));
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}