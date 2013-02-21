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
    
    maxOffSet = 200;
    margins = 50;
    
    nScheme = 0;
    
    background.loadImage("wood.jpg");
    
    bVertical = true;
}

bool ColorPalette::loadPalette(string _sFile){
    
    bool success = false;
    
    ofxXmlSettings XML;
    
    if ( XML.loadFile(_sFile)){
        if ( XML.pushTag("palette")) {
            
            bVertical = XML.getValue("vertical", bVertical);
            maxOffSet = XML.getValue("offset", maxOffSet);
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
            
            
            int totalSchemes = XML.getNumTags("schemes");
            for (int j = 0; j < totalSchemes; j++ ){
                XML.pushTag("schemes",j);
                
                vector <ofColor> newScheme;
                
                for (int i = 0; i < XML.getNumTags("color"); i++){
                    string hexString = XML.getValue("color", "ee1b12", i );
                    int hexInt = ofHexToInt( hexString );
                    
                    ofColor color;
                    color.setHex( hexInt );
                    newScheme.push_back(color);
                }
                
                schemes.push_back( newScheme );
                XML.popTag();
            }
        }
    }
}

void ColorPalette::setScheme(unsigned int _nId){
    if (_nId < schemes.size()){
        
        if (_nId != nScheme){
            nScheme = _nId;
            clear();
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

float ColorPalette::getOffset(){
    float yPct = powf(pct, shp);

    return yPct * maxOffSet;    
}

void ColorPalette::update(){
    if ( ( height != ofGetWindowHeight() ) || ( width != ofGetWindowWidth() ) ) {
        bSetup = false;
    }
    
    if (!bSetup){
        
        height = ofGetWindowHeight();
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
    
    if (bVertical) {
        if ( _pos.x < getOffset() ){
            
            if (( _pos.y > margins ) && (_pos.y < height-margins)){
                
                float totalHeight = height-margins*2.0;
                float elementHeight = totalHeight/schemes[nScheme].size();
                
                int index = (_pos.y-margins)/elementHeight;
                
                _color = schemes[nScheme][index];
                
                overSomething = true;
            }
        }
    } else {
        if ( _pos.y < getOffset() ){
            
            if (( _pos.x > margins ) && (_pos.x < width-margins)){
                
                float totalWidth = width-margins*2.0;
                float elementWidth = totalWidth/schemes[nScheme].size();
                
                int index = (_pos.x-margins)/elementWidth;
                
                _color = schemes[nScheme][index];
                
                overSomething = true;
            }
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

    if (bVertical){
        
        float totalHeight = height-margins*2.0;
        float elementHeight = totalHeight/schemes[nScheme].size();
        float y = margins+elementHeight*0.5;
        
        for (int i = 0; i < schemes[nScheme].size(); i++){
            ofSetColor( schemes[nScheme][i] );
            int imageIndex = i%images.size();
            images[imageIndex].setAnchorPercent(0.5, 0.5);
            images[imageIndex].draw(maxOffSet*0.5,y);
    
            y += elementHeight;
        }
        
    } else {
        
        float totalWidth = width-margins*2.0;
        float elementWidth = totalWidth/schemes[nScheme].size();
        float x = margins+elementWidth*0.5;
        
        for (int i = 0; i < schemes[nScheme].size(); i++){
            ofSetColor( schemes[nScheme][i] );
            int imageIndex = i%images.size();
            images[imageIndex].setAnchorPercent(0.5, 0.5);
            images[imageIndex].draw(x,maxOffSet*0.5);
            
            //        ofSetColor(255);
            //        int imageTopIndex = i%imagesTop.size();
            //        imagesTop[imageTopIndex].setAnchorPercent(0.5, 0.5);
            //        imagesTop[imageTopIndex].draw(x,height*0.5);
            
            x += elementWidth;
        }
    }
    
    
    end();
    ofPopStyle();
}

void ColorPalette::draw(){
    if ( pct > 0.0 ){
        if (bVertical){
            background.draw(-background.getWidth() + getOffset(),0);
            ofFbo::draw(-maxOffSet + getOffset(), 0);
        } else {
            background.draw(0, -background.getHeight() + getOffset());
            ofFbo::draw(0, -maxOffSet + getOffset());
        }
        
    }
}