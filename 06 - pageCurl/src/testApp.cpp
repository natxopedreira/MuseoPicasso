#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);
    
    camera.tilt(-30);
    camera.rotate(20, 0, 1, 0);
    
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    
	material.setShininess(64);
	material.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    
    page.path.setFillColor(ofColor(255));
    
}

//--------------------------------------------------------------
void testApp::update(){
    page.rebuild(sin(ofGetElapsedTimef())*2.0);
}

//--------------------------------------------------------------
void testApp::draw(){
    
    camera.begin();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
    ofEnableLighting();
    pointLight.enable();
    material.begin();
    
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    
    ofSetColor(255);
    page.draw(ofGetKeyPressed());
    
    material.end();
    ofDisableLighting();
    camera.end();
    
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

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}