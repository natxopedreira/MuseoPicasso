//
//  DragRect.cpp
//
//  Created by Patricio Gonzalez Vivo on 10/5/12.
//
//

#include "DragRect.h"

DragRect::DragRect(){
    objName = "none";
    
    bEditMode = false;
    
    //  Eventos de Mouse
    //
    ofAddListener(ofEvents().mouseDragged, this, &DragRect::_mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &DragRect::_mouseReleased);
    ofAddListener(ofEvents().draw, this, &DragRect::_draw);
}

bool DragRect::loadSettings(string _fileConfig){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    
    if (XML.loadFile(_fileConfig)){
        fileFound = true;
        
        x = XML.getValue(objName+":area:x", 0);
        y = XML.getValue(objName+":area:y", 0);
        width = XML.getValue(objName+":area:width", 640);
        height = XML.getValue(objName+":area:height", 480);
    }
    
    this->init(x, y, width, height);
    
    return fileFound;
}

bool DragRect::saveSettings(string _fileConfig){
    
    bool fileFound = false;
    
    ofxXmlSettings XML;
    
    if (XML.loadFile(_fileConfig)){
        
        if ( XML.pushTag(objName) ){

            XML.setValue("area:x", x);
            XML.setValue("area:y", y);
            XML.setValue("area:width", width);
            XML.setValue("area:height", height);
            
            XML.popTag();
            
            fileFound = XML.saveFile();
        }
    }
    
    return fileFound;
}

void DragRect::init(int _x, int _y, int _width, int _height){
    
    if (_x == -1)
        _x = this->x;
    
    if (_y == -1)
        _y = this->y;
    
    if (_width == -1)
        _width = this->width;
    
    if (_height == -1)
        _height = this->height;
    
    //  Setea las propiedades del rectangulo que contiene Graffiti
    //
    ofRectangle::set(_x, _y, _width, _height);
}

void DragRect::_draw(ofEventArgs &e){
    //  Draggable area-boxes for Edit modes
    //
    if ( bEditMode ){
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(x, y);
        
        //  Graffiti Area
        //
        ofNoFill();
        ofSetColor(245, 58, 135,200);
        ofRect(0,0,width,height);
        ofFill();
        ofSetColor(245, 58, 135,150);
        ofRect(-7,-7,14,14);
        ofRect(width-7,height-7,14,14);
        ofSetColor(245, 58, 135,255);
        ofDrawBitmapString(objName + " Area",15,15);
        
        ofPopMatrix();
        ofPopStyle();
    }
}

//----------------------------------------------------------- Mouse
void DragRect::_mouseDragged(ofMouseEventArgs &e){
    ofPoint mouse = ofPoint(e.x,e.y);
    
    if (bEditMode){
        //  Drag coorners of graffitiArea
        //
        ofPoint A = ofPoint(x,y);
        
        ofPoint B = ofPoint(x+width,y+height);
        
        if ( A.distance(mouse) < 20 ){
            x += e.x - x;
            y += e.y - y;
            
            this->init(x,y, width, height);
        }
        
        if ( B.distance(mouse) < 20 ){
            width += e.x - x - width;
            height += e.y - y - height;
            
            this->init(x,y, width, height);
        }
    }
}

void DragRect::_mouseReleased(ofMouseEventArgs &e){
    if (bEditMode){
        init(x, y, width, height);
    }
}