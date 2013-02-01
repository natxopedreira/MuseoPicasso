/*
 *  PaintParticle.h
 *
 *  Created by Patricio Gonzalez Vivo on 08/08/10.
 *  Copyright 2010 http://www.patriciogonzalezvivo.com All rights reserved.
 *
 */

#ifndef PAINTPARTICLE
#define PAINTPARTICLE

#include "ofMain.h"

class PaintParticle {
public:
    
    PaintParticle(){	
        idN = NULL;
        lnk = NULL;
        
        loc.set(0,0);
        acc.set(0,0);
        vel.set(0,0);
        radio = 5;
        
        width = 0;
        height = 0;
        
        color.set(255);
        bDrops = false;
        bFirst = true;
    };
    
    PaintParticle(int _id, ofPoint _pos, ofColor _color, float _radio ){
        idN     = _id;
        lnk     = NULL;
        
        loc		= _pos;
        acc.set(0,0);
        vel.set(0,0);
        
        color	= _color;
        radio	= _radio;
        width	= _radio*2.0;
        height	= _radio*2.0;
        
        bDrops = false;
        bFirst = true;
    };

	void	setLnk( const PaintParticle *toLnk ){ lnk = toLnk; };
	void    setAlpha( int _alpha){ color.a = _alpha; };
	void	setBrush( ofImage *_brush){ brush = _brush;};
    void    setDrops( bool _drops = true){ bDrops = _drops; };
	
    ofPoint getPosition(){ return loc; };
    float   getRadio(){return radio; };
    int     getId(){return idN; };
    
	void	update(){
        if (bDrops && !bFirst)
            if ((timer > 1) && (radio >= 1)){
                loc.y++;
                radio -= 0.002;
                timer -= 0.5;
                color.a--;
            }
    };
    
	void	draw(){
        ofPushStyle();
        
        ofFill();
        ofSetColor(color);
        ofEnableAlphaBlending();
        
        if (bFirst){
            ofPoint position = loc;
            
            if ((width == 0) && (height == 0)){
                
                if (( lnk != NULL )){
                    ofPoint lastPosition = lnk->loc;
                    ofSetLineWidth(radio);
                    if (ofDist(position.x, position.y, lastPosition.x, lastPosition.y) < 50 )
                        ofLine(position,lastPosition);
                }
                
                glLineWidth(0);
                ofCircle(position,radio*0.4);
            } else {
                brush->draw(position.x-width*.5,position.y-height*0.5,width,height);
                radio = 15;
            }
            
            if (bDrops){
                radio = ofRandom(0,radio*0.2);
                timer = ofRandom(radio*15);
            }
            
            bFirst = false;
            
        } else if (bDrops){
            if (timer > 1 ){
                brush->draw(loc.x-radio,loc.y-radio,radio*2.0,radio*2.0);
            }
        }
        ofPopStyle();
    };

    const	PaintParticle* lnk;
    
private:
    ofImage *brush;
    ofColor	color;
	ofPoint	loc, acc, vel;
	float	width, height, radio, timer;
	int		idN;	
	bool	bDrops, bFirst;	
};
#endif