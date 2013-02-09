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
    pageSize = 160;
    pos.set(0,20,0);
    rebuild();
}

// Given {x, y} on the coordinate system of the page (_pageMin[X,Y] to _pageMax[X,Y]),
// this generates appropriate curl parameters {_theta, _ay, _angle, _conicContribution}
// and sets _userCurl to true. It also halts any auto curling that might be happening.
void Page::startUserCurl(float x, float y){
    
    // Scale x & y so they are parametrized to normal ranges
    
    // x ranges from -Xmax to Xmax   (lhs to rhs)
    x = x / (2.0f * _pageMaxX);     // x ranges from -0.5 to 0.5     (lhs to rhs)
    x = x + 0.5f;                   // x ranges from 0 to 1          (lhs to rhs)
    x = min(x, 1.0f);               // x ranges from 1 to 0          (rhs to lhs)
    x = max(x, 0.0f);
    
    // y ranges from -Ymax to Ymax   (bottom to top)
    y = y / (_pageMaxY);            // y ranges from -1 to 1         (bottom to top)
    y = min(y, 1.0f);
    y = max(y, -1.0f);
    
    // Transform {x, y} to {theta, ay, angle, conicContribution}
    
    coneParameters cone;
    cone.set(x, y);
    
    {
        // keep some history so we know if we're going left or right.
        // TRACK 498: use this for inertia when doing auto curling.
        
        if (_prevTimestamp[0] < 0.0f)
        {
            // history is not available, so just make it seem like
            // we've been at the same point for a while
            
            _userCurlingRight = true;   // hack to make backwards curling not flicker
            _prevX[0] = _prevX[1] = x;
            _prevY[0] = _prevY[1] = y;
            _prevTimestamp[0] = _prevTimestamp[1] = constants::general::currentTime();
            _currentCone = cone;
        }
        else
        {
            _userCurlingRight = (x + 0.001) > _prevX[0];
            
            _prevX[1] = _prevX[0];
            _prevY[1] = _prevY[0];
            _prevTimestamp[1] = _prevTimestamp[0];
            
            _prevX[0] = x;
            _prevY[0] = y;
            _prevTimestamp[0] = ofGetElapsedTimef();
        }
        
        _nextCone = cone;
        _userCurl = true;
        _autoCurl = false;
    }
}

// This is the meat. Given the four cone input parameters, map all the vertices of the page to look
// like the pages are turning. The idea is from a brilliant paper: "Turning Pages of 3D Electronic
// Books" by Hong et al, PARC. IEEE Symposium on 3d User Interfaces 2006.
//      http://www.parc.com/content/attachments/turning-pages-3D.pdf
//
// In the paper they show how mapping a sheet of paper to a cone whose side is on the spine of the
// book looks just like curling a page by its corner. The cone's parameters are theta (angle of a
// right cone) and Ay (the location on the y-axis of the apex of the cone... this value is always
// negative). Then, as the page turns, they rotate the whole piece of paper by 'angle'. This turns a
// page by the top right or bottom right... but what about turning the page by the middle right?
//
// I'm extending their idea by saying that if you're turning the page from the middle right
// (conicContribution = 0.0) then we wrap the page around a cylinder whose side is on the axis
// of the book. If you turn the page from the top right (conicContribution = 1.0) then we wrap
// the page around a cone whose apex is 'south' of the page. If you turn the page from the bottom
// right (conicContribution = -1.0), then we wrap the page around a cone whose apex is 'north' of
// the page.
//
// Anywhere in between, and the info from two sources is combined linearly. Eg, of you turn the
// page from 3/4 of the way up the page (conicContribution = 0.75) then we take half the 'south'
// cone's data and half the cylinder's data to show the page.
void Page::curlPage(coneParameters cone){
    float theta = cone.theta;
    float Ay = cone.ay;
    float angle = cone.angle;
    float conicContribution = cone.conicContribution;
    
    theta = ofDegToRad(theta);
    angle = ofDegToRad(angle);
    
    std::shared_ptr<g::ivertex_buffer> vertexBuffer = pageNode->vertexBuffer();
    g::vertex_lit_colored_textured* buffer = nullptr;
    
    vertexBuffer->lockNoOverwrite(0,
                                  pageNode->vertexCountX() * pageNode->vertexCountY(),
                                  (b::memory**)&buffer);
    
    int iMax = pageNode->vertexCountX();
    int jMax = pageNode->vertexCountY();
    
    // As the user grabs towards the middle-right of the page, we curl the paper by deforming it
    // on to a cylinder. The cylinder radius is taken as the endpoint of the cone parameters:
    // ie: cylRadius = R*sin(theta) where R is the distance to the rightmost point on the page, all the way up.
    float cylR = sqrt(iMax * iMax + (jMax/2 - Ay)*(jMax/2 - Ay));
    float cylRadius = cylR * sin(theta);
    
    // This is a hack to fix the bump at the start of a curl. If theta is greater than 50 degrees,
    // cylRadius needs to start approaching "infinity".
    b::float32 cylRadiusAt50 = cylR * sin(m::degToRad(50));
    if (theta > m::degToRad(50.0f)){
        b::float32 x1 = m::degToRad(90.0f);
        b::float32 x2 = m::degToRad(50.0f);
        cylRadius = STRAIGHT_LINE(x1, 200.0f, x2, cylRadiusAt50, theta);
    }
    
    // Flipping from top corner or bottom corner?
    float posNegOne;
    if (conicContribution > 0) {
        // flipping from top corner
        posNegOne = 1.0f;
    } else {
        // flipping from bottom corner
        posNegOne = -1.0f;
        Ay = -Ay + pageNode->vertexCountY();
    }
    
    conicContribution = abs(conicContribution);
    
    for (int j = 0; j < jMax; j++){
        
        // Vectorization notes:
        //    * i & j need to stay as int32. Converts from uint32 to float aren't supported in the V1 vectorizer.
        //    * we don't scalar expand IV's from outer loops -- just a stupid NYI in V1 vectorizer.
        //    * the member variables need to be hoisted. Again, V1 vectorizer.
        
        int jOffset = j * pageNode->vertexCountX();
        float jFloat = (float)j;
        float pageMaxX = _pageMaxX;
        float pageMaxY = _pageMaxY;
        float pageCoordTransform = _pageCoordTransform;
        
        for (int i = 0; i < iMax; i++) {
            // Be careful with units here. i & j range from [0, iMax] & [0, jMax] which are just the vertex indices.
            // 'ay' is in the coordinate system of the vertex indices. The actual values of the vertices are in
            // page units (pageMin[X,Y], pageMax[X,Y]).
            
            float x = (float)i;
            float y = jFloat;
            float z = 0;
            
            float coneX = x;
            float coneY = y;
            float coneZ = z;
            {
                // Compute conical parameters
                float R = sqrt(x * x + (y - Ay)*(y - Ay));
                float r = R * sin(theta);
                float beta  = asin(x / R) / sin(theta);
                
                // Cone it up
                coneX = r * sin(beta);
                coneY = R + posNegOne * Ay - r * (1 - cos(beta)) * sin(theta);
                coneZ = r * (1 - cos(beta)) * cos(theta);
                
                // Then rotate by angle about the y axis
                coneX = coneX * cos(angle) - coneZ * sin(angle);
                coneZ = coneX * sin(angle) + coneZ * cos(angle);
                
                // Transform coordinates to the page
                coneX = (coneX * pageCoordTransform) - pageMaxX;
                coneY = (-posNegOne * coneY * pageCoordTransform) + pageMaxY;
                coneZ = coneZ * pageCoordTransform;
            }
            
            float cylX = x;
            float cylY = y;
            float cylZ = z;
            {
                float beta = cylX / cylRadius;
                
                // Rotate (0,0,0) by beta around line given by x = 0, z = cylRadius.
                // aka Rotate (0,0,-cylRadius) by beta, then add cylRadius back to z coordinate
                cylZ = -cylRadius;
                cylX = -cylZ * sin(beta);
                cylZ = cylZ * cos(beta);
                cylZ += cylRadius;
                
                // Then rotate by angle about the y axis
                cylX = cylX * cos(angle) - cylZ * sin(angle);
                cylZ = cylX * sin(angle) + cylZ * cos(angle);
                
                // Transform coordinates to the page
                cylX = (cylX * pageCoordTransform) - pageMaxX;
                cylY = (-cylY * pageCoordTransform) + pageMaxY;
                cylZ = cylZ * pageCoordTransform;
            }
            
            // combine cone & cylinder systems
            x = conicContribution * coneX + (1-conicContribution) * cylX;
            y = conicContribution * coneY + (1-conicContribution) * cylY;
            z = conicContribution * coneZ + (1-conicContribution) * cylZ;
            
            buffer[jOffset + i].position.x = x;
            buffer[jOffset + i].position.y = y;
            buffer[jOffset + i].position.z = z;
        }
    }
    
    vertexBuffer->unlock();
    
    pageNode->updateNormals();
}

//--------------------------------------------------------------
void Page::rebuild(){
    path.clear();

    path.moveTo(ofPoint(-pageSize, 0, -pageSize));
    path.lineTo(ofPoint( pageSize, 0, -pageSize));
    path.lineTo(ofPoint( pageSize, 0,  pageSize));
    path.lineTo(ofPoint(-pageSize, 0,  pageSize));

    remesh();
}

//--------------------------------------------------------------
void Page::rebuild(float bendPct){
    path.clear();

    float bendY = pageSize * bendPct;
    float bendZ = ABS(pageSize * bendPct * 2);

    path.moveTo(ofPoint(-pageSize, 0, -pageSize));
    path.lineTo(ofPoint(pageSize, 0, -pageSize));
    path.lineTo(ofPoint(pageSize, 0, pageSize - bendZ));
    path.bezierTo(ofPoint(pageSize, 0, pageSize - bendZ * 0.5), ofPoint(pageSize, bendY * 0.5, pageSize), ofPoint(pageSize, bendY, pageSize));
    path.lineTo(ofPoint(-pageSize, bendY, pageSize));
    path.bezierTo(ofPoint(-pageSize, bendY * 0.5, pageSize), ofPoint(-pageSize, 0, pageSize - bendZ * 0.5), ofPoint(-pageSize, 0, pageSize - bendZ));
    path.lineTo(ofPoint(-pageSize, 0, -pageSize));

    remesh();
}

//--------------------------------------------------------------
void Page::rebuild(float bendTopPct, float bendBottomPct){
    path.clear();

    // try to keep the page area the same as it bends...

    float bendTopY = pageSize * 0.5 * bendTopPct;
    float bendTopZ = -pageSize * (1 - ABS(bendTopPct) * 0.5);
    
    float bendBottomY = pageSize * 0.5 * bendBottomPct;
    float bendBottomZ = pageSize * (1 - ABS(bendBottomPct) * 0.5);

    path.moveTo(ofPoint(-pageSize, bendTopY, bendTopZ));
    path.lineTo(ofPoint(pageSize, bendTopY, bendTopZ));

    path.bezierTo(ofPoint(pageSize, bendTopY * 0.5, bendTopZ),
                  ofPoint(pageSize, 0, bendTopZ * 0.5),
                  ofPoint(pageSize, 0, 0));
    path.bezierTo(ofPoint(pageSize, 0, bendBottomZ * 0.5),
                  ofPoint(pageSize, bendBottomY * 0.5, bendBottomZ),
                  ofPoint(pageSize, bendBottomY, bendBottomZ));

    path.lineTo(ofPoint(-pageSize, bendBottomY, bendBottomZ));

    path.bezierTo(ofPoint(-pageSize, bendBottomY * 0.5, bendBottomZ),
                  ofPoint(-pageSize, 0, bendBottomZ * 0.5),
                  ofPoint(-pageSize, 0, 0));
    path.bezierTo(ofPoint(-pageSize, 0, bendTopZ * 0.5),
                  ofPoint(-pageSize, bendTopY * 0.5, bendTopZ),
                  ofPoint(-pageSize, bendTopY, bendTopZ));

    remesh();
}

//--------------------------------------------------------------
void Page::remesh(){
    mesh = path.getTessellation();

    // add a bunch of dummy normals
    for (int i=0; i < mesh.getNumVertices(); i++) {
        mesh.addNormal(ofVec3f());
    }

    // calculate the real normal for each vertex
    for (int i=0; i < mesh.getNumIndices(); i += 3) {
        ofVec3f a = mesh.getVertex(mesh.getIndex(i + 0));
        ofVec3f b = mesh.getVertex(mesh.getIndex(i + 1));
        ofVec3f c = mesh.getVertex(mesh.getIndex(i + 2));

        ofVec3f ab = b - a;
        ofVec3f ac = c - a;

        ofVec3f n = ab.cross(ac).normalized();  // gotta flip it, guess i'm winding backwards...

        if (i < mesh.getNumVertices() / 2) {
            // overwrite the first half of the normals
            mesh.setNormal(mesh.getIndex(i + 0), n);
            mesh.setNormal(mesh.getIndex(i + 1), n);
            mesh.setNormal(mesh.getIndex(i + 2), n);
        }
        else {
            // don't overwrite the second half
            if (mesh.getNormal(mesh.getIndex(i + 0)).length() == 0) mesh.setNormal(mesh.getIndex(i + 0), n);
            if (mesh.getNormal(mesh.getIndex(i + 1)).length() == 0) mesh.setNormal(mesh.getIndex(i + 1), n);
            if (mesh.getNormal(mesh.getIndex(i + 2)).length() == 0) mesh.setNormal(mesh.getIndex(i + 2), n);
        }
    }
}

//--------------------------------------------------------------
void Page::draw( bool _bDebug ){
    ofPushMatrix();

    ofTranslate(pos);
    
    if (_bDebug) {
        ofSetColor(255);
        mesh.drawWireframe();
        ofSetColor(255, 0, 0);
        for (int i=0; i < mesh.getNumIndices(); i++) {
            ofVec3f coord = mesh.getVertex(mesh.getIndex(i));
            ofVec3f norm = mesh.getNormal(mesh.getIndex(i));

            ofLine(coord, coord + (norm * 10));
        }
    } else {
        mesh.draw();
    }

    ofPopMatrix();
}
