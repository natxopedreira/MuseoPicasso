#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
    gui.setup("brush");
    gui.add(brushWidth.setup("width",25,0,50));
    gui.add(brushNumber.setup("number",10,1,50));
    gui.add(brushDamp.setup("damp",0.245,0.0,0.5));
    gui.add(brushK.setup("k",0.0825,0.0,0.5));
    
    gui.add(brushRepRad.setup("repulsion_rad", 5, 0, 20));
    gui.add(brushRepPct.setup("repulsion_pct", 0.5, 0.0, 1.0));
    
    gui.add(colorLerp.setup("color_Pallet_lerp", 0.5, 0.0, 1.0));
    gui.add(colorRandom.setup("color_random", 0.005, 0.0, 0.02));
    
    gui.loadFromFile("settings.xml");
    
    canvas.allocate(ofGetScreenWidth(), ofGetScreenHeight());
    canvas.begin();
    ofClear(0,0);
    canvas.end();
    
    palette.loadPalette("palette.css");
    palette.setVisible(false);
    
    bDebug  = false;
}

//--------------------------------------------------------------
void testApp::update(){
    brush.update();
    palette.update();

    canvas.begin();
    if ( palette.getVisible() ){
        if ( mouseY > palette.getY() ){
            brush.draw();
        }
    } else {
        brush.draw();
    }
    canvas.end();
    
    if ( palette.getVisible() ){
        if ( mouseY < palette.getY() ){
            palette.begin();
            brush.draw();
            palette.end();
        }
    }
    
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
//    ofBackground(0);
    
    canvas.draw(0, 0);
    
    palette.draw();
    
    if (bDebug){
        ofSetColor(255);
        brush.drawDebug();
        
        gui.draw();
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'd'){
        bDebug = !bDebug;
    } else if (key == ' '){
        brush.clear();
        canvas.begin();
        ofClear(0,0);
        canvas.end();
    } else if (key == 'f'){
        ofToggleFullscreen();
    } else if ( key == 'p'){
        brush.clear();
        palette.toggleVisible();
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
    if ( palette.getVisible() ){
        brush.pickColorFrom( palette.getTextureReference(),colorLerp,colorRandom );
    } 
    
    brush.set(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (palette.getVisible()){
        if (  mouseY > palette.getY() ){
            brush.clear();
            palette.setVisible(false);
        }
    }
    
    brush.init(brushNumber);
    brush.setWidth(brushWidth);
    
    brush.damp = brushDamp;
    brush.k = brushK;
    brush.repPct = brushRepPct;
    brush.repRad = brushRepRad;
    
    brush.begin();
    brush.set(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    brush.set(x, y);
    brush.end();
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