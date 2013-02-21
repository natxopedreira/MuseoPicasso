//
//  Page.cpp
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#include "Page.h"

//--------------------------------------------------------------
Page::Page(){
    meshDefinition = 20;
    
    front = NULL;
    back = NULL;
}

float Page::getTransition(){
    return ofMap(angle, 0.0, 1.2, 1.0, 0.0);
}

void Page::setHandAt(ofPoint _hand, float lerpPct){
    if ( inside(_hand) ){
        hand.x = ofLerp(hand.x,ofNormalize(_hand.x, x, x+width),lerpPct);
        hand.y = ofLerp(hand.y,ofNormalize(_hand.y, y, y+height),lerpPct);
        bChange = true;
    } else {
        
        if ( (hand != ofPoint(1.0,1.0)) && ( hand != ofPoint(0.0,0.5))){
        
            if (hand.x >= 0.5){
                hand.x = ofLerp(hand.x, 1.0, lerpPct);
                hand.y = ofLerp(hand.y, 1.0, lerpPct);
            } else {
                hand.x = ofLerp(hand.x, 0.0, lerpPct);
                hand.y = ofLerp(hand.y, 0.5, lerpPct);
            }
            
            bChange = true;
        }
        
    }
}

void Page::setNormHandAt(ofPoint _hand, float lerpPct){
    if ( hand != _hand ){
        hand.x = ofLerp(hand.x, _hand.x, lerpPct );
        hand.y = ofLerp(hand.y, _hand.y, lerpPct );
        bChange = true;
    }
}

void Page::update(){
    
    if (bChange){
        //  Update angles and parameters for making the Cone and the cylinder
        //
        if (hand.x > 0.95f){
            theta = STRAIGHT_LINE(1.0f,    90.0f,   0.95f,  60.0f, hand.x);
            ay =    STRAIGHT_LINE(1.0f,   -20.0f,   0.95f,  -5.0f, hand.x);
            angle = 0.0;
        } else if (hand.x > 0.8333f) {
            theta = STRAIGHT_LINE(0.95f,   60.0f, 0.8333f,  55.0f, hand.x);
            ay =    STRAIGHT_LINE(0.95f,   -5.0f, 0.8333f,  -4.0f, hand.x);
            angle = STRAIGHT_LINE(0.95f,    0.0f, 0.8333f,  13.0f, hand.x);
        } else if (hand.x > 0.3333f) {
            theta = STRAIGHT_LINE(0.8333f, 55.0f, 0.3333f,  45.0f, hand.x);
            ay =    STRAIGHT_LINE(0.8333f, -4.0f, 0.3333f,  -10.0f, hand.x);
            angle = STRAIGHT_LINE(0.8333f, 13.0f, 0.3333f,  35.0f, hand.x);
        } else if (hand.x > 0.1666f) {
            theta = STRAIGHT_LINE(0.3333f, 45.0f, 0.1666f,  25.0f, hand.x);
            ay =    STRAIGHT_LINE(0.3333f, -10.0f, 0.1666f, -30.0f, hand.x);
            angle = STRAIGHT_LINE(0.3333f, 35.0f, 0.1666f,  60.0f, hand.x);
        } else {            
            theta = STRAIGHT_LINE(0.1666f, 25.0f,    0.0f,  20.0f, hand.x);
            ay =    STRAIGHT_LINE(0.1666f, -30.0f,   0.0f, -40.0f, hand.x);
            angle = STRAIGHT_LINE(0.1666f, 60.0f,    0.0f,  95.0f, hand.x);
        }
        
        theta = ofDegToRad(theta);
        angle = ofDegToRad(angle);
        conicContribution = (hand.y-0.5)*2.0;
        
        // Flipping from top corner or bottom corner?
        //
        if (conicContribution > 0){
            // flipping from top corner
            //
            posNegOne = 1.0f;
        } else {
            // flipping from bottom corner
            //
            posNegOne = -1.0f;
            ay = -ay + height;
        }
        
        conicContribution = abs(conicContribution);
        
        // As the user grabs towards the middle-right of the page, we curl the paper by deforming it
        // on to a cylinder. The cylinder radius is taken as the endpoint of the cone parameters:
        // ie: cylRadius = R*sin(theta) where R is the distance to the rightmost point on the page, all the way up.
        //
        cylR = sqrt( width*width + (height*0.5-ay) * (height*0.5-ay));
        cylRadius = cylR * sin(theta);
        
        // This is a hack to fix the bump at the start of a curl. If theta is greater than 50 degrees,
        // cylRadius needs to start approaching "infinity".
        float cylRadiusAt50 = cylR * sin(ofDegToRad(50));
        if (theta > ofDegToRad(50.0f)){
            float x1 = ofDegToRad(90.0f);
            float x2 = ofDegToRad(50.0f);
            cylRadius = STRAIGHT_LINE(x1, 200.0f, x2, cylRadiusAt50, theta);
        }
        
        //  Re-build mesh
        //
        mesh.clear();
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
        //  FRONT
        //
        for(int j = 0; j < height; j += meshDefinition) {
            for(int i = 0; i < width; i += meshDefinition) {
                
                vectorFace face;
                
                //  texCoords
                //
                face.a.set(i,height-j,0);
                face.b.set(i+meshDefinition,height-j,0);
                face.c.set(i+meshDefinition,height-j-meshDefinition,0);
                face.d.set(i,height-j-meshDefinition,0);
                
                //  Vectex
                //
                face.A = getCurlPos( ofPoint(i,j,0) );
                face.B = getCurlPos( ofPoint(i+meshDefinition,j,0) );
                face.C = getCurlPos( ofPoint(i+meshDefinition,j+meshDefinition,0) );
                face.D = getCurlPos( ofPoint(i,j+meshDefinition,0) );
                
                
                addFace(mesh, face);
            }
        }
        
        bChange = false;
    }
}

ofPoint Page::getCurlPos(ofPoint pos , float _zOffset ){
    
    // Be careful with units here. "pos" range from [0, COLS] & [0, ROWS] which are just the vertex indices.
    // 'ay' is in the coordinate system of the vertex indices. The actual values of the vertices are in
    // page units (pageMin[X,Y], pageMax[X,Y]).
    
    //  Calculate position on CONE
    //
    ofPoint conePos(pos);
    {
        // Compute conical parameters
        float R = sqrt(pos.x * pos.x + (pos.y - ay)*(pos.y - ay));
        float r = R * sin(theta);
        float beta  = asin(pos.x / R) / sin(theta);
        
        // Cone it up
        conePos.x = r * sin(beta);
        conePos.y = R + ay * posNegOne - r * (1 - cos(beta)) * sin(theta);
        conePos.z = r * (1 - cos(beta)) * cos(theta);
        
        // Then rotate by angle about the y axis
        conePos.x = conePos.x * cos(angle) - conePos.z * sin(angle);
        conePos.z = conePos.x * sin(angle) + conePos.z * cos(angle);
        
        // Transform coordinates to the page
        conePos.x = conePos.x ;
        conePos.y = (-posNegOne * conePos.y) + height;
        conePos.z = conePos.z;
    }
    
    //  Calculate position on CYLINDER
    //
    ofPoint cylPos(pos);
    {
        float beta = cylPos.x / (cylRadius + _zOffset );
        
        // Rotate (0,0,0) by beta around line given by x = 0, z = cylRadius.
        // aka Rotate (0,0,-cylRadius) by beta, then add cylRadius back to z coordinate
        cylPos.z = -cylRadius;
        cylPos.x = -cylPos.z * sin(beta);
        cylPos.z = cylPos.z * cos(beta);
        cylPos.z += cylRadius;
        
        // Then rotate by angle about the y axis
        cylPos.x = cylPos.x * cos(angle) - cylPos.z * sin(angle);
        cylPos.z = cylPos.x * sin(angle) + cylPos.z * cos(angle);
        
        // Transform coordinates to the page
        cylPos.x = cylPos.x;
        cylPos.y = -cylPos.y + height;
        cylPos.z = cylPos.z;
    }
    
    // Combine CONE & CYLINDER
    //
    return conePos * conicContribution + cylPos * (1.0-conicContribution);
}

//Some helper functions
//--------------------------------------------------------------
void Page::addFace(ofMesh& mesh, ofPoint a, ofPoint b, ofPoint c) {
	ofVec3f normal = ((b - a).cross(c - a)).normalize();
	mesh.addNormal(normal);
	mesh.addVertex(a);
	mesh.addNormal(normal);
	mesh.addVertex(b);
	mesh.addNormal(normal);
	mesh.addVertex(c);
}

//--------------------------------------------------------------
void Page::addFace(ofMesh& mesh, vectorFace& _face) {
	addFace(mesh, _face.A, _face.B, _face.C);
    mesh.addTexCoord(_face.a);
    mesh.addTexCoord(_face.b);
    mesh.addTexCoord(_face.c);
    
	addFace(mesh, _face.A, _face.C, _face.D);
    mesh.addTexCoord(_face.a);
    mesh.addTexCoord(_face.c);
    mesh.addTexCoord(_face.d);
}

void Page::draw(bool _bDebug){
    
        
    if (!_bDebug){
        ofPushMatrix();
        ofPushStyle();
    
        ofTranslate(x, y);

        front->bind();
        ofSetColor(ofFloatColor(1.0,getTransition()));
        mesh.draw();
        front->unbind();
        
        float pct = ofMap(getTransition(),0.0,0.1,1.0,0.0);
        ofSetColor(ofFloatColor(1.0, pct ) );
        ofRectangle backSide(*this);
        ofPoint centerRight = getCurlPos( ofPoint(width,height*0.5) );
        backSide.x = ofMap(centerRight.x,-0.0,-300,0,-this->width,true);
        backSide.y = 0;
        backSide.width = abs(backSide.x);
        
        if (back !=NULL){
    
            back->draw(backSide);
        } else {
            ofSetColor(255);
            ofRect(backSide);
        }
        
        ofPopStyle();
        ofPopMatrix();
    } else {
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(x, y);
        ofSetColor(ofFloatColor(1.0,getTransition()));
        
        mesh.drawWireframe();
        
        //  Center of the book
        //
        ofTranslate(0, height*0.5);
        ofSetColor(255, 0, 0);
        float cSize = 5;
        ofLine(-cSize, 0, cSize, 0);
        ofLine(0, -cSize, 0, cSize);
    
        ofPopMatrix();
        ofSetColor(255, 0, 0);
        ofCircle(x+hand.x*width, y+hand.y*height, 10);
        
        ofNoFill();
        
        ofSetColor(0,0,255);
        ofRect(*this);
        
        ofPopStyle();
    }
}