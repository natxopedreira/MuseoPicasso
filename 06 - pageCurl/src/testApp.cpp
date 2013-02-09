#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);
    
    light.enable();
	light.setPosition(+500, 0, 0);
    
    int width = 480*0.5;
    int height = 640*0.5;
    page.set(ofGetWidth()*0.5, ofGetHeight()*0.5-height*0.5, width, height);
}

//--------------------------------------------------------------
void testApp::update(){
    page.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    
    page.draw(true);
    
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