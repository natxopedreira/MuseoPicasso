//
//  Palette.cpp
//
//  Created by Patricio Gonzalez Vivo on 1/21/13.
//
//

#include "Palette.h"

Palette::Palette(){
    bSetup = false;
    bVisible = false;
    
    inc = 0.06145;
    shp = 0.3;
    height = 200;
    
    background.loadImage("wood.jpg");
    
}

void Palette::loadPalette(string _sFile){
    
    ofColor color;
    color.set(255);
    colors.push_back(color);
    color.set(255,0,0);
    colors.push_back(color);
    color.set(0,255,0);
    colors.push_back(color);
    color.set(0,0,255);
    colors.push_back(color);
    color.set(0);
    colors.push_back(color);
    
    ofImage image;
    image.loadImage("01.png");
    images.push_back(image);
    image.loadImage("02.png");
    images.push_back(image);
    image.loadImage("03.png");
    images.push_back(image);
}

void Palette::setVisible(bool _bVisible){
    bVisible = _bVisible;
}

void Palette::toggleVisible(){
    bVisible = !bVisible;
}

bool Palette::getVisible(){
    if (pct >= 0.9){
        return true;
    } else {
        return false;
    }
}

float Palette::getY(){
    float yPct = powf(pct, shp);
    return yPct * height;
}

void Palette::update(){
    if ( width != ofGetWindowWidth()){
        bSetup = false;
    }
    
    if (!bSetup){
        width = ofGetWindowWidth();
        allocate(width, height);
        pct = 0.0f;
        
        clear();
        
        bSetup = true;
    }
    
    if ( bVisible ){
        if ( pct < 1.0 ){
            pct += inc;
        }
    } else {
        if ( pct > 0.0 ){
            pct -= inc;
        }
    }
}

void Palette::clear(){
    //  Construct FBO with colors;
    //
    ofPushStyle();
    begin();
    ofClear(0,0);
    int x = 0;
    for (int i = 0; i < colors.size(); i++){
        ofSetColor( colors[i] );
        int imageIndex = i%images.size();
        int imageWidth = (height/images[imageIndex].getHeight()) * images[imageIndex].getWidth();
        images[imageIndex].draw(x,0, imageWidth, height);
        x += imageWidth;
    }
    end();
    ofPopStyle();
}

void Palette::draw(){
    if ( pct > 0.0 ){
        background.draw(0, -background.getHeight() + getY());
        ofFbo::draw(0, -height + getY());
    }
}