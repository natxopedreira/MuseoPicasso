//
//  SprayCan.cpp
//  04 - Graffity
//
//  Created by Patricio Gonzalez Vivo on 1/30/13.
//
//

#include "SprayCan.h"

SprayCan::SprayCan(){
    brush.loadImage("brush.png");
    
    paintAreaSize = 50;
}

void SprayCan::update(){
    for (int i = 0; i < paintParticles.size(); i++){
        paintParticles[i]->update();
    }
}

void SprayCan::draw(){
    for (int i = 0; i < paintParticles.size(); i++){
        paintParticles[i]->draw();
    }
}

void SprayCan::addPaint(ofPoint _pos,float _w, float _h, int _id) {
    ofPoint size    = ofPoint(abs( paintAreaSize - _w),
                              abs( paintAreaSize - _h),
                              0.0);
    
    ofColor color   = paintColor;
    
	float radio     = (size.x + size.y)*0.5;
    int nLnk        = 0;
	int bDrop       = false;
    
    for (int i = paintParticles.size()-1; i >= 0 ; i--){
        if (nLnk == 0)
            if ( paintParticles[i]->getId() == _id ){
                nLnk = i;
                radio = ( paintParticles[i]->getRadio() + radio ) * 0.5;
                
                if ( ( _pos.distance( paintParticles[i]->getPosition() ) <= 2))
                    bDrop = true;
            }
        }
    
	if ((_pos.x > 0) && (_pos.y > 0)){
        
		PaintParticle *p    = new PaintParticle( _id, _pos, color, radio);
        
		p->setAlpha( 255-ofMap( max(size.x,size.y) - min(size.x,size.y),0,max(size.x,size.y),0,255) );
        p->setBrush(&brush);
        
		if ( nLnk > 0 )
            p->setLnk(paintParticles[nLnk]);
        
		if ( bDrop )
            p->setDrops();
        
		paintParticles.push_back(p);
	}
    
    if (paintParticles.size() > 50){
        delete paintParticles[0];
		paintParticles.erase( paintParticles.begin() );
	}
}

void SprayCan::clean(){
    for (int i = 0; i < paintParticles.size(); i++){
        delete paintParticles[i];
    }
    
    paintParticles.clear();
}