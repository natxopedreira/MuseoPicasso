//
//  ColorPalette.cpp
//
//  Created by Patricio Gonzalez Vivo on 1/21/13.
//
//

#include "ColorPalette.h"

ColorPalette::ColorPalette(){
    bSetup = false;
    bVisible = false;
    
    inc = 0.06145;
    shp = 0.3;
    height = 200;
    margins = 50;
    
//    background.loadImage("wood.jpg");
}

bool ColorPalette::loadPalette(string _sFile){
    
    bool success = false;
    
    ofxXmlSettings XML;
    
    if ( XML.loadFile(_sFile)){
        if ( XML.pushTag("palette")) {
            
            height = XML.getValue("height", height);
            margins = XML.getValue("margins", margins);
            
            for (int i = 0; i < XML.getNumTags("img"); i++){
                ofImage image;
                if ( image.loadImage( XML.getValue("img", "01.png", i ) ) ){
                    images.push_back(image);
                }
            }
            
            for (int i = 0; i < XML.getNumTags("imgtop"); i++){
                ofImage image;
                if ( image.loadImage( XML.getValue("imgtop", "01.png", i ) ) ){
                    imagesTop.push_back(image);
                }
            }
            
            for (int i = 0; i < XML.getNumTags("color"); i++){
                string hexString = XML.getValue("color", "ee1b12", i );
                int hexInt = ofHexToInt( hexString );
                
                ofColor color;
                color.setHex( hexInt );
                colors.push_back(color);
            }
        }
    }
}

void ColorPalette::setVisible(bool _bVisible){
    bVisible = _bVisible;
}

void ColorPalette::toggleVisible(){
    bVisible = !bVisible;
}

bool ColorPalette::getVisible(){
    if (pct >= 0.5){
        return true;
    } else {
        return false;
    }
}

float ColorPalette::getY(){
    float yPct = powf(pct, shp);
    return yPct * height;
}

void ColorPalette::update(){
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

bool ColorPalette::checkColor(ofPoint _pos, ofColor& _color){
    bool overSomething = false;
    if ( _pos.y < getY() ){
        
        if (( _pos.x > margins ) && (_pos.x < width-margins)){
        
            float totalWidth = width-margins*2.0;
            float elementWidth = totalWidth/colors.size();
            
            int index = (_pos.x-margins)/elementWidth;
            
            _color = colors[index];
            
            overSomething = true;
        }
    } 
    
    setVisible(false);
    return overSomething;
    
}

void ColorPalette::clear(){
    //  Construct FBO with colors;
    //
    ofPushStyle();
    begin();
    ofClear(0,0);

    float totalWidth = width-margins*2.0;
    float elementWidth = totalWidth/colors.size();
    float x = margins+elementWidth*0.5;
    
    for (int i = 0; i < colors.size(); i++){
        ofSetColor( colors[i] );
        int imageIndex = i%images.size();
        images[imageIndex].setAnchorPercent(0.5, 0.5);
        images[imageIndex].draw(x,height*0.5);
        
        ofSetColor(255);
        int imageTopIndex = i%imagesTop.size();
        imagesTop[imageTopIndex].setAnchorPercent(0.5, 0.5);
        imagesTop[imageTopIndex].draw(x,height*0.5);
        
        x += elementWidth;
    }
    
    end();
    ofPopStyle();
}

void ColorPalette::draw(){
    if ( pct > 0.0 ){
//        background.draw(0, -background.getHeight() + getY());
        ofFbo::draw(0, -height + getY());
    }
}