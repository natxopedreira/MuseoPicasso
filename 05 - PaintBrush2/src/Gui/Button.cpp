//
//  Button.cpp
//
//
#include "Button.h"

void SimpleBtn::setImage(string _sFile){
    image.loadImage(_sFile);
}

bool SimpleBtn::checkOver(ofPoint _pos){
    if (inside(_pos)){
        ofNotifyEvent(clickPressed, nId);
        return true;
    } else {
        return false;
    }
}

void SimpleBtn::draw(){
    image.draw(*this);
}