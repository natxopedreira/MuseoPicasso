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
    
    
    imgA.loadImage("00.jpeg");
    imgB.loadImage("01.jpeg");
    
    int width = imgA.getWidth();
    int height = imgB.getHeight();
    page.set(ofGetWidth()*0.5, ofGetHeight()*0.5-height*0.5, width, height);
    page.A = &(imgA.getTextureReference());
    page.B = &(imgB.getTextureReference());
    
}

//--------------------------------------------------------------
void testApp::update(){
    tuioClient.getMessage();
    
#ifndef USE_TUIO
    page.setHandAt(ofPoint(mouseX, mouseY));
#endif
    
    page.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    if (ofGetKeyPressed()){
        page.draw(true);
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
    
    page.draw();
    
    material.end();
    ofDisableLighting();
    
	glDisable(GL_NORMALIZE);
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
//    page.setNormHandAt(ofPoint(tuioCursor.getX(),tuioCursor.getY()));
    page.setHandAt(loc);
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