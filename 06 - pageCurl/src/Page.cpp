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
}

int Page::getCols(){
    return width;
}

int Page::getRows(){
    return height;
}

void Page::setHandAt(ofPoint _hand){
    if ( inside(_hand) ){
        hand.x = ofNormalize(_hand.x, x, x+width);
        hand.y = ofNormalize(_hand.y, y, y+height);
        bChange = true;
    } else {
        if ( hand != ofPoint(1.0,1.0,0.0)){
            hand.set(1.0,1.0,0.0);
            bChange = true;
        }
    }
    
    if (bChange){
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
            ay = -ay + getRows();
        }
        
        conicContribution = abs(conicContribution);
        
        // As the user grabs towards the middle-right of the page, we curl the paper by deforming it
        // on to a cylinder. The cylinder radius is taken as the endpoint of the cone parameters:
        // ie: cylRadius = R*sin(theta) where R is the distance to the rightmost point on the page, all the way up.
        //
        cylR = sqrt( getCols()*getCols() + (getRows()*0.5-ay) * (getRows()*0.5-ay));
        cylRadius = cylR * sin(theta);
        
        // This is a hack to fix the bump at the start of a curl. If theta is greater than 50 degrees,
        // cylRadius needs to start approaching "infinity".
        float cylRadiusAt50 = cylR * sin(ofDegToRad(50));
        if (theta > ofDegToRad(50.0f)){
            float x1 = ofDegToRad(90.0f);
            float x2 = ofDegToRad(50.0f);
            cylRadius = STRAIGHT_LINE(x1, 200.0f, x2, cylRadiusAt50, theta);
        }
    }
}

void Page::update(){
    
    if (bChange){
        mesh.clear();
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
        int skip = 10;
        
        for(int j = 0; j < height; j += skip) {
            for(int i = 0; i < width; i += skip) {
                
                ofVec3f nw = ofPoint(x,y) + getCurlPos( ofPoint(i,j,0) );
                ofVec3f ne = ofPoint(x,y) + getCurlPos( ofPoint(i+skip,j,0) );
                ofVec3f sw = ofPoint(x,y) + getCurlPos( ofPoint(i,j+skip,0) );
                ofVec3f se = ofPoint(x,y) + getCurlPos( ofPoint(i+skip,j+skip,0) );
                
                addFace(mesh, nw, ne, se, sw);
            }
        }
        
        bChange = false;
    }
}

ofPoint Page::getCurlPos(ofPoint pos){
    
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
        float beta = cylPos.x / cylRadius;
        
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
void Page::addFace(ofMesh& mesh, ofPoint a, ofPoint b, ofPoint c, ofPoint d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

void Page::draw(bool _bDebug){
    
    if (!_bDebug){
        mesh.draw();
    } else {
        mesh.drawWireframe();
        
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(x, y+height*0.5);
        
        //  Center of the book
        //
        ofSetColor(255, 0, 0);
        float cSize = 5;
        ofLine(-cSize, 0, cSize, 0);
        ofLine(0, -cSize, 0, cSize);
        
        //  Cone Axis
        //
        ofTranslate(0, getRows()*0.5 - ay);
        ofSetColor(0, 255, 0);
        ofLine(-cSize, 0, cSize, 0);
        ofLine(0, -cSize, 0, cSize);
        
        ofPopMatrix();
        
        ofNoFill();
        ofSetColor(255, 0, 0);
        ofCircle(x+hand.x*width, y+hand.y*height, 10);

        ofSetColor(0,0,255);
        ofRect(*this);
        
        ofPopStyle();
    }
}