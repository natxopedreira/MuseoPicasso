#include "testApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    width = ofGetScreenWidth();
    height = ofGetScreenHeight();
    iconSize = 100;
	
    ofAddListener(tuioClient.cursorAdded,this,&testApp::_tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&testApp::_tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&testApp::_tuioUpdated);
	tuioClient.start(3333);
    
    string stencilFragShader = STRINGIFY(uniform sampler2DRect imageTex;
                                         uniform sampler2DRect stencilTex;
                                         
                                         void main (void){
                                             vec2 st = gl_TexCoord[0].st;
                                             vec4 image = texture2DRect(imageTex, st);
                                             vec4 stencil = texture2DRect(stencilTex, st);
                                             
                                             float alpha = 1.0 - max(stencil.r,max(stencil.g,stencil.b));
                                             gl_FragColor = vec4(image.rgb, min(alpha,image.a));
                                         } );
    
    stencilShader.setupShaderFromSource(GL_FRAGMENT_SHADER, stencilFragShader);
    stencilShader.linkProgram();
    
    canvas.allocate(width,height, GL_RGBA);
    stencilBuffer.allocate(width,height, GL_RGBA);
    
    palette.loadPalette("settings.xml");
    palette.setVisible(false);
    paletteBtn.set(0, 0, iconSize+30, iconSize+30);
    paletteBtn.setImage("icon_palette.png");
    ofAddListener( paletteBtn.clickPressed, this, &testApp::showPalette );
    
    cleanBtn.set(ofGetWidth()-iconSize,ofGetHeight()-iconSize,iconSize,iconSize);
    ofAddListener( cleanBtn.clickPressed, this, &testApp::cleanCanvas );
    cleanBtn.setImage("icon_close.png");
    
    stencilIdCounter = 0;
    
    addStencil(stencilIdCounter);
}

//--------------------------------------------------------------
void testApp::update(){
    tuioClient.getMessage();
    spray.update();
    palette.update();
    
    //  Draw Canvas
    //
    canvas.begin(0);
    spray.draw();
    
	canvas.end(0);
    
    canvas.setBlendMode( BLEND_NORMAL );
    canvas.update();
    
    //  Stencil Mask
    //
    stencilBuffer.src->begin();
    ofClear(0, 0);
    
    for (int i = 0; i < stencils.size(); i++) {
        stencils[i]->draw();
    }
    
    stencilBuffer.src->end();
    
    
    stencilBuffer.dst->begin();
    ofClear(0,0);
    ofSetColor(255,255);
    
    stencilShader.begin();
    stencilShader.setUniformTexture("imageTex", canvas.getTextureReference() , 0);
    stencilShader.setUniformTexture("stencilTex", stencilBuffer.src->getTextureReference(), 1);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(width, 0); glVertex3f(width, 0, 0);
    glTexCoord2f(width, height); glVertex3f(width,height, 0);
    glTexCoord2f(0,height);  glVertex3f(0,height, 0);
    glEnd();

    stencilShader.end();
    stencilBuffer.dst->end();
    
    ofSetWindowTitle( ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
//    canvas.draw(0, 0);
    stencilBuffer.dst->draw(0, 0);
    
    for (int i = 0; i < stencils.size(); i++) {
        stencils[i]->drawGui();
    }
    
    paletteBtn.draw();
    cleanBtn.draw();
    
    palette.draw();
    
    
}

//----------------------------------------------------------- EVENTOS
void testApp::keyPressed(int key){
    int n = 0;
    
	switch(key) {
		case 'f': ofToggleFullscreen(); break;
        case ' ': cleanCanvas(n);
        case OF_KEY_UP: spray.paintAreaSize++; break;
        case OF_KEY_DOWN: spray.paintAreaSize--; break;
	}
}

void testApp::mousePressed(int x, int y, int button) {
    ofPoint loc(x,y);
    if ( !cleanBtn.checkOver(loc) && !paletteBtn.checkOver(loc)){
        
        if ( !palette.checkColor(loc, spray.paintColor)){
            
            for (int i = 0; i < stencils.size(); i++) {
                if ( stencils[i]->checkClick(ofPoint(x,y) ) ){
                    return;
                }
            }
            
            spray.addPaint(loc);
        }
    }
}

void testApp::mouseDragged(int x, int y, int button){
    
    //  Check Stencils
    //
    
    if ( !palette.checkColor(ofPoint(x,y), spray.paintColor)){
        for (int i = 0; i < stencils.size(); i++) {
            if ( stencils[i]->checkClick(ofPoint(x,y) ) ){
                return;
            }
        }
        
        spray.addPaint( ofPoint(x,y) );
    }
}

void testApp::mouseReleased(){
}

void testApp::windowResized(int w, int h){
    cleanBtn.set(w-iconSize,h-iconSize,iconSize,iconSize);
}

//----------------------------------------------------------- GUI

void testApp::showPalette(int &_n){
    if (!palette.getVisible()){
        palette.clear();
        
        palette.setVisible(true);
    }
}

void testApp::cleanCanvas(int &_n){
    spray.clean();
    canvas.begin();
    ofClear(0,0);
    canvas.end();
    
    canvas.clear();
    
}

void testApp::addStencil(int &_n ){
    
    //  TODO: get file name from palette of Stencils
    //
    string fileName = "banksy.png";
    
    Stencil * newStencil = new Stencil();
    newStencil->loadImage(fileName);
    newStencil->nId = stencilIdCounter++;
    ofAddListener( newStencil->close, this, &testApp::closeStencil);
    
    stencils.push_back(newStencil);
}

void testApp::closeStencil(int &_n ){
    for (int i = stencils.size()-1; i >= 0; i--) {
        if (stencils[i]->nId == _n){
            
            delete stencils[i];
            
            stencils.erase(stencils.begin()+i);
            return;
        }
    }
}

//----------------------------------------------------------- TUIO
void testApp::_tuioAdded(TuioCursor & tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*width,
                          tuioCursor.getY()*height,
                          0.0);
    
    if ( !cleanBtn.checkOver(loc)){
        spray.addPaint(loc,
                       tuioCursor.getWidth()*width,
                       tuioCursor.getHeight()*height,
                       tuioCursor.getSessionId());
    }
}

void testApp::_tuioUpdated(TuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*width,
                          tuioCursor.getY()*height,
                          0.0);
    
    spray.addPaint(loc,
             tuioCursor.getWidth()*width,
             tuioCursor.getHeight()*height,
             tuioCursor.getSessionId());
}

void testApp::_tuioRemoved(TuioCursor & tuioCursor){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo info){

}
