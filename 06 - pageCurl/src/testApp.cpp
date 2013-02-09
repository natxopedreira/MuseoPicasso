#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofBackground(0);
    
    ofSetSmoothLighting(true);
    pointLight.setPosition(ofGetWidth()*0.5, ofGetHeight()*0.5, -1000);
    pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    
	material.setShininess(64);
	material.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    
    int width = 480*0.5;
    int height = 640*0.5;
    page.set(ofGetWidth()*0.5, ofGetHeight()*0.5-height*0.5, width, height);
//    page.set(0, 0-height*0.5, width, height);
}

//--------------------------------------------------------------
void testApp::update(){
    page.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    if (ofGetKeyPressed()){
        page.draw(true);
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
    page.setHandAt(ofPoint(x,y));
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