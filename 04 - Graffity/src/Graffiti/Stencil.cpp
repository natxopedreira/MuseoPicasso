//
//  Stencil.cpp
//  04 - Graffity
//
//  Created by Patricio Gonzalez Vivo on 1/30/13.
//
//

#include "Stencil.h"

Stencil::Stencil(){
    iconSize = 50;
    
    iconMove.loadImage("icon_move.png");
    iconClose.loadImage("icon_close.png");
    iconResize.loadImage("icon_resize.png");
}

void Stencil::loadImage(string _imgFile){
    
    if (img.loadImage(_imgFile)){
        width = img.getWidth();
        height = img.getHeight();
        
        x = ofGetWidth()*0.5 - width*0.5;
        y = ofGetHeight()*0.5 - height*0.5;
    }
    
}

bool Stencil::checkClick(ofPoint _pos){
    
    ofPoint A = ofPoint(x,y);               //  Move Icon
    ofPoint B = ofPoint(x+width,y+height);  //  Re-Size Icon
    ofPoint C = ofPoint(x+width,y);         //  Close Icon
    
    if ( A.distance(_pos) < iconSize ){
        x += _pos.x - x;
        y += _pos.y - y;
        
        this->set(x,y, width, height);
        
        return true;
    } else if ( B.distance(_pos) < iconSize ){
        width += _pos.x - x - width;
        height += _pos.y - y - height;
        
        this->set(x,y, width, height);
        
        return true;
    } else if ( C.distance( _pos) < iconSize ){
        ofNotifyEvent(close, nId);
        
        return true;
    } else if ( inside(_pos) ){
        return false;
    } else {
        return false;
    }
}

void Stencil::update(){
    
}

void Stencil::draw(){
    img.draw(*this);
}

void Stencil::drawGui(){
    ofPoint A = ofPoint(x,y);               //  Move Icon
    ofPoint B = ofPoint(x+width,y+height);  //  Re-Size Icon
    ofPoint C = ofPoint(x+width,y);         //  Close Icon
    
    ofPushStyle();
    
    ofSetColor(255,30);
    img.draw(*this);
    
    ofSetColor(255);
    iconMove.setAnchorPercent(0.5, 0.5);
    iconMove.draw(A, iconSize, iconSize);
    iconResize.setAnchorPercent(0.5, 0.5);
    iconResize.draw(B, iconSize, iconSize);
    iconClose.setAnchorPercent(0.5, 0.5);
    iconClose.draw(C, iconSize, iconSize);
    
    ofPopStyle();
}