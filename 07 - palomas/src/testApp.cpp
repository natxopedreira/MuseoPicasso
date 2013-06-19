#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    
    
    width = 640;
	height = 480;
    
	video.initGrabber(width, height);
	video.setDesiredFrameRate(30);

	videoColorCvImage.allocate(width, height);
	videoGrayscaleCvImage.allocate(width, height);
	videoPrevFrameImage.allocate(width, height);
	videoDiffImage.allocate(width, height);
    
    area.bEditMode = true;
    area.objName = "track";
    area.loadSettings();
    
    pidginLoop.loadMovie("01.mov");
    pidginLoop.play();
    
    pidginFly.loadMovie("02.mov");
    
    threshold = 80;     // threshold de la diferencia
    nToFly = 3.0;       // valor para disparar los p‡jaros
    
    bDebug = true;
    bToFly = false;
}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
	
	if (video.isFrameNew()){
		videoColorCvImage.setFromPixels(video.getPixels(), width, height);
		videoGrayscaleCvImage = videoColorCvImage;
		
		videoDiffImage.absDiff(videoGrayscaleCvImage, videoPrevFrameImage);
		videoDiffImage.threshold(threshold);
		
		videoPrevFrameImage = videoGrayscaleCvImage;
        
		nChange = ofMap(videoDiffImage.countNonZeroInRegion(area.x, area.y, area.width, area.height), 0,5000, 0.3, 3);
        
        if (!pidginFly.isPlaying() && nChange >= nToFly){
            pidginFly.play();
        }
	}
    
    pidginLoop.update();
    pidginFly.update();
    
    if (pidginFly.getPosition() > 0.9){
        pidginFly.setPosition(0.0);
        pidginFly.stop();
    }

    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    
    ofSetColor(255);
    if (pidginFly.isPlaying()){
        pidginFly.draw(-pidginFly.getWidth()*0.5,-pidginFly.getHeight()*0.5);
    } else {
        pidginLoop.draw(-pidginLoop.getWidth()*0.5,-pidginLoop.getHeight()*0.5);
    }
    ofPopMatrix();
    
    if (bDebug){
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetColor(255);
        videoGrayscaleCvImage.draw(0,0, 640, 480);
        ofSetColor(255,0,0);
        videoDiffImage.draw(0,0, 640, 480);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        
        ofDrawBitmapString("Threshold "+ ofToString(threshold) + " (UP/DOWN)", 15, height+15);
        ofDrawBitmapString("Change "+ ofToString(nChange) + " of " + ofToString(nToFly) + " (LEFT/RIGHT)", 15, height+30);
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'd'){
        bDebug = !bDebug;
        area.bEditMode = bDebug;
    } else if ( key == OF_KEY_UP){
        threshold++;
    } else if ( key == OF_KEY_DOWN){
        threshold--;
    } else if ( key == OF_KEY_LEFT){
        nToFly -= 0.1;
    } else if ( key == OF_KEY_RIGHT){
        nToFly += 0.1;
    } else if ( key == 'f'){
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

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}