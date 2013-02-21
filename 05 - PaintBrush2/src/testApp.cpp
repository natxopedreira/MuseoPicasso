#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
    gui.setup("brush");
    gui.add(brushWidth.setup("brush_width",25,0,50));
    gui.add(brushNumber.setup("number_of_lines",10,1,50));
    gui.add(lineWidth.setup("line_width", 2, 1,10));
            
    gui.add(brushDamp.setup("damp",0.245,0.0,0.5));
    gui.add(brushK.setup("k",0.0825,0.0,0.5));
    
    gui.add(brushRepRad.setup("repulsion_rad", 5, 0, 20));
    gui.add(brushRepPct.setup("repulsion_pct", 0.5, 0.0, 1.0));
    
    gui.add(colorLerp.setup("color_lerp", 0.5, 0.0, 1.0));
    gui.add(colorRandom.setup("color_random", 0.005, 0.0, 0.1));
    gui.add(alphaThreshold.setup("alpha_threshold", 80.0, 0.0, 100.0));
    
    
    gui.loadFromFile("settings.xml");
    
    ofAddListener(tuioClient.cursorAdded,this,&testApp::_tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&testApp::_tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&testApp::_tuioUpdated);
	tuioClient.start(3333);
    
    iconSize = 100;
    
    canvas.allocate(ofGetScreenWidth(), ofGetScreenHeight());
    canvas.begin();
    ofClear(0,0);
    canvas.end();
    
    palette.loadPalette("settings.xml");
    palette.setVisible(false);
    
    
    paletteLandBtn.set(0, 0, iconSize+30, iconSize+30);
    paletteLandBtn.setImage("landscape.png");
    paletteLandBtn.nId = 0;
    paletteProfBtn.set(0, iconSize+30, iconSize+30, iconSize+30);
    paletteProfBtn.setImage("profile.png");
    paletteProfBtn.nId = 1;
    
    ofAddListener( paletteProfBtn.clickPressed, this, &testApp::showPalette );
    ofAddListener( paletteLandBtn.clickPressed, this, &testApp::showPalette );
    
    
    cleanBtn.set(ofGetWidth()-iconSize,ofGetHeight()-iconSize,iconSize,iconSize);
    ofAddListener( cleanBtn.clickPressed, this, &testApp::cleanCanvas );
    cleanBtn.setImage("icon_close.png");

    idCounter = 0;
    mouseId = -1;

    bDebug  = false;
}

//--------------------------------------------------------------
void testApp::update(){
    tuioClient.getMessage();
    
    for (int i = 0; i < brushes.size(); i++){
        brushes[i]->update();
    }
    
    palette.update();

    canvas.begin();
    if ( palette.getVisible() ){
        for(int i = 0; i < brushes.size(); i++){
            if ( brushes[i]->x > palette.getOffset() ){
                brushes[i]->draw();
            }
        }
        
    } else {
        for(int i = 0; i < brushes.size(); i++){
            brushes[i]->draw();
        }
    }
    canvas.end();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    canvas.draw(0, 0);
    
    ofSetColor(200);
    paletteProfBtn.draw();
    paletteLandBtn.draw();
    cleanBtn.draw();
    
    ofSetColor(255);
    palette.draw();
    
    if (bDebug){
        ofSetColor(255);
        for(int i = 0; i < brushes.size(); i++){
            brushes[i]->drawDebug();
        }
        
        gui.draw();
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'd'){
        bDebug = !bDebug;
    } else if (key == ' '){
        int n = 0;
        cleanCanvas(n);
    } else if (key == 'f'){
        ofToggleFullscreen();
    } else if ( key == 'p'){
        for(int i = 0; i < brushes.size(); i++){
            brushes[i]->clear();
        }
        
        if (!palette.getVisible()){
            palette.clear();
        }
        palette.toggleVisible();
    }
}

void testApp::showPalette(int &_n){
    
    if (!palette.getVisible()){
        palette.setScheme(_n);
        palette.clear();
        for(int i = 0; i < brushes.size(); i++){
            brushes[i]->clear();
        }
        palette.setVisible(true);
    }
}

void testApp::cleanCanvas(int &_n){
    for(int i = 0; i < brushes.size(); i++){
        brushes[i]->clear();
    }
    canvas.begin();
    ofClear(0,0);
    canvas.end();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    ofPoint mouse(x,y);
    
    if ( palette.getVisible() ){
        
        if (  mouseX > palette.getOffset() ){
            palette.setVisible(false);
        } else {
            
            if (palette.checkColor(ofPoint(x,y), color)){
                palette.setVisible(false);
            }
            
        }
        
    } else if ( paletteProfBtn.checkOver(mouse) ){
        
    } else if ( paletteLandBtn.checkOver(mouse)) {
         
    } else if ( cleanBtn.checkOver(mouse) ){
        
    } else {
    
        idCounter++;
        mouseId = idCounter;
        
        Brush * newBrush = new Brush;
        newBrush->init(brushNumber);
        newBrush->setBrushWidth(brushWidth);
        newBrush->setLineWidth(lineWidth);
        newBrush->setColor( color, colorRandom);
        newBrush->nId = mouseId;
        
        newBrush->damp = brushDamp;
        newBrush->k = brushK;
        newBrush->repPct = brushRepPct;
        newBrush->repRad = brushRepRad;
        newBrush->alphaThreshold = alphaThreshold;
        
        newBrush->begin();
        newBrush->set(x,y);
        
        brushes.push_back(newBrush);
    } 
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    for (int i = 0; i < brushes.size(); i++) {
        if (brushes[i]->nId == mouseId){
            brushes[i]->set(x,y);
            return;
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
    for(int i = brushes.size()-1; i >= 0 ; i--){
        if (brushes[i]->nId == mouseId){
            brushes[i]->set(x,y);
            brushes[i]->end();
            brushes[i]->nId = -1;
            
            delete brushes[i];
            brushes.erase(brushes.begin()+i);
            return;
        }
    }
    
}

//----------------------------------------------------------- TUIO
void testApp::_tuioAdded(TuioCursor & tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),
                          tuioCursor.getY()*ofGetHeight(),
                          0.0);
    
    if ( palette.getVisible() ){
        
        if (  mouseX > palette.getOffset() ){
            palette.setVisible(false);
        } else {
            
            if (palette.checkColor(loc, color)){
                palette.setVisible(false);
            }
            
        }
        
    } else if ( paletteProfBtn.checkOver(loc) ){
        
    } else if ( paletteLandBtn.checkOver(loc)) {
        
    } else if ( cleanBtn.checkOver(loc) ){
        
    } else {
        
        Brush * newBrush = new Brush;
        newBrush->init(brushNumber);
        newBrush->setBrushWidth(brushWidth);
        newBrush->setLineWidth(lineWidth);
        newBrush->setColor( color, colorRandom);
        newBrush->nId = tuioCursor.getFingerId();
        
        newBrush->damp = brushDamp;
        newBrush->k = brushK;
        newBrush->repPct = brushRepPct;
        newBrush->repRad = brushRepRad;
        newBrush->alphaThreshold = alphaThreshold;
        
        newBrush->begin();
        newBrush->set(loc.x,loc.y);
        
        brushes.push_back(newBrush);
    }
    
}

void testApp::_tuioUpdated(TuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),
                          tuioCursor.getY()*ofGetHeight(),
                          0.0);
    
    for (int i = 0; i < brushes.size(); i++) {
        if (brushes[i]->nId == tuioCursor.getFingerId()){
            brushes[i]->set(loc.x,loc.y);
            return;
        }
    }
}

void testApp::_tuioRemoved(TuioCursor & tuioCursor){
    ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),
                          tuioCursor.getY()*ofGetHeight(),
                          0.0);
    
    for(int i = brushes.size()-1; i >= 0 ; i--){
        if (brushes[i]->nId == tuioCursor.getFingerId()){
            brushes[i]->set(loc.x,loc.y);
            brushes[i]->end();
            
            delete brushes[i];
            
            brushes.erase(brushes.begin()+i);
            return;
        }
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    cleanBtn.set(w-iconSize,h-iconSize,iconSize,iconSize);
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}