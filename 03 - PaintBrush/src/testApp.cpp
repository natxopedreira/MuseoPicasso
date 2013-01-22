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
    
    gui.add(colorPalleteLerp.setup("color_Pallet_lerp", 0.5, 0.0, 1.0));
    gui.add(colorCanvasLerp.setup("color_Canvas_lerp", 0.1, 0.0, 1.0));
    gui.add(colorRandom.setup("color_random", 0.005, 0.0, 0.02));
    
    gui.loadFromFile("settings.xml");
    
    canvas.allocate(ofGetScreenWidth(), ofGetScreenHeight());
    canvas.begin();
    ofClear(0,0);
    canvas.end();
    
    width = ofGetScreenWidth();
    height = 200;
    
    palleteImg.loadImage("pallete.png");
    
    pallete.allocate(width, height);
    pallete.begin();
    ofClear(0, 0);
    ofSetColor(255);
    palleteImg.draw(0, 0, width, height);
    
    pallete.end();
    
    bDebug  = false;
    bPallete= true;
}

//--------------------------------------------------------------
void testApp::update(){
    brush.update();

    canvas.begin();
    if (bPallete){
        if (mouseY > height){
            brush.draw();
            
        }
    } else {
        brush.draw();
    }
    canvas.end();
    
    if (bPallete){
        pallete.begin();
        
        brush.draw();
        
        pallete.end();
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    canvas.draw(0, 0);
    
    if (bPallete){
        pallete.draw(0, 0);
    }
    
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
        bPallete = !bPallete;
        
        pallete.begin();
        ofClear(0, 0);
        ofSetColor(255);
        palleteImg.draw(0, 0, width, height);
        pallete.end();
        
        brush.clear();
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
    if (bPallete){
        brush.pickColorFrom( pallete.getTextureReference(),colorPalleteLerp,colorRandom );
    } else {
        brush.pickColorFrom( canvas.getTextureReference(),colorCanvasLerp,colorRandom );
    }
    
    brush.set(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (bPallete){
        if (mouseY > height){
            brush.clear();
            bPallete = false;
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
    
    pallete.begin();
    ofClear(0, 0);
    ofSetColor(255);
    palleteImg.draw(0, 0, ofGetWindowWidth(), height);
    pallete.end();
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}