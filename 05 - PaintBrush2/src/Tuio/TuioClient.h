/*
 TUIO C++ Library for OpenFrameworks
 http://www.openframeworks.cc

 Copyright (c) 2008 by Matthias Dörfelt based on the Classes by Martin Kaltenbrunner
 which can be found at http://reactivision.sourceforge.net/

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef TUIOCLIENT
#define TUIOCLIENT


#include "ofMain.h"

#include "ofxOsc.h"

#include "TuioCursor.h"
#include "TuioObject.h"

// default port 12345
#define PORT 12345
//#define PORT 3333

//--------------------------------------------------------
class TuioClient{

public:
	TuioClient(){
		currentFrame = lastFrame = maxFingerID = -1;
	};

	void start(int _port){
		receiver.setup( _port );
	};

	void start(){
		receiver.setup( PORT );
	};

	void getMessage(){

		while( receiver.hasWaitingMessages() )
		{
			ofxOscMessage m;
			receiver.getNextMessage( &m );

			if ( m.getAddress() == "/tuio/2Dobj" )
			{

				if( m.getArgAsString( 0 ) == "set" ){
					if ((currentFrame<lastFrame) && (currentFrame>0)) return;

					long sid  = (long)(m.getArgAsInt32(1));
					int fiducial = m.getArgAsInt32(2);
					float xpos =  m.getArgAsFloat(3);
					float ypos =  m.getArgAsFloat(4);
					float angle =  m.getArgAsFloat(5);
					float xspeed =  m.getArgAsFloat(6);
					float yspeed =  m.getArgAsFloat(7);
					float rspeed =  m.getArgAsFloat(8);
					float maccel =  m.getArgAsFloat(9);
					float raccel =  m.getArgAsFloat(10);

					list<TuioObject*>::iterator tobj;
					for (tobj=objectList.begin(); tobj != objectList.end(); tobj++)
						if((*tobj)->getSessionId()==sid) break;

					if (tobj == objectList.end()) {
						TuioObject *addObject = new TuioObject(sid, fiducial, xpos, ypos, angle);
						objectList.push_back(addObject);


						ofNotifyEvent(objectAdded, *(objectList.back()), this);

					}else if ( ((*tobj)->getX()!=xpos) || ((*tobj)->getY()!=ypos) || ((*tobj)->getAngle()!=angle) || ((*tobj)->getXSpeed()!=xspeed) || ((*tobj)->getYSpeed()!=yspeed) || ((*tobj)->getRotationSpeed()!=rspeed) || ((*tobj)->getMotionAccel()!=maccel) || ((*tobj)->getRotationAccel()!=raccel) ) {
						(*tobj)->update(xpos,ypos,angle,xspeed,yspeed,rspeed,maccel,raccel);

						ofNotifyEvent(objectUpdated, **tobj, this);
					}
				} else if( m.getArgAsString( 0 ) == "alive"  ){
					if ((currentFrame<lastFrame) && (currentFrame>0)) return;
					for (int i=1;i<m.getNumArgs();i++) {

						long sid  = (long)(m.getArgAsInt32(i));;
						newObjectList.push_back((long)(sid));

						list<long>::iterator iter;
						iter = find(aliveObjectList.begin(), aliveObjectList.end(), (long)(sid));
						if (iter != aliveObjectList.end()) aliveObjectList.erase(iter);
					}
					list<long>::iterator alive_iter;
					for (alive_iter=aliveObjectList.begin(); alive_iter != aliveObjectList.end(); alive_iter++) {
						list<TuioObject*>::iterator tobj;
						for (tobj=objectList.begin(); tobj!=objectList.end(); tobj++) {
							TuioObject *deleteObject = (*tobj);
							if(deleteObject->getSessionId()==*alive_iter) {

								ofNotifyEvent(objectRemoved, **tobj, this);

								objectList.erase(tobj);

								delete deleteObject;
								break;
							}
						}
					}
					aliveObjectList = newObjectList;
					newObjectList.clear();
				} else if( m.getArgAsString( 0 ) == "fseq" ){

					if(currentFrame>0) lastFrame = currentFrame;
					currentFrame  = (int)(m.getArgAsInt32(1));
				}
			} else if ( m.getAddress() == "/tuio/2Dcur" )
			{
				if( m.getArgAsString( 0 ) == "set" ){
					if ((currentFrame<lastFrame) && (currentFrame>0)) return;

					long sid  = (long)(m.getArgAsInt32(1));
					float xpos	 =	m.getArgAsFloat(2);
					float ypos	 =  m.getArgAsFloat(3);
					float xspeed =  m.getArgAsFloat(4);
					float yspeed =  m.getArgAsFloat(5);
					float maccel =  m.getArgAsFloat(6);
					// WIDTH & HEIGHT
					float wd	 =	m.getArgAsFloat(7);
					float ht	 =  m.getArgAsFloat(8);

					list<TuioCursor*>::iterator tit;
					for (tit=cursorList.begin(); tit != cursorList.end(); tit++)
						if((*tit)->getSessionId()==sid) break;

					if (tit == cursorList.end()) {
						int fid = (int)(cursorList.size());

						if ((int)(cursorList.size())<=maxFingerID) {
							list<TuioCursor*>::iterator closestCursor = freeCursorList.begin();

							for(list<TuioCursor*>::iterator testCursor = freeCursorList.begin();testCursor!= freeCursorList.end(); testCursor++) {
								if((*testCursor)->getDistance(xpos,ypos)<(*closestCursor)->getDistance(xpos,ypos)) 
									closestCursor = testCursor;
							}

							fid = (*closestCursor)->getFingerId();
							freeCursorList.erase(closestCursor);
							delete *closestCursor;
						} else maxFingerID = fid;

						TuioCursor *addCursor = new TuioCursor(sid,fid,xpos,ypos,wd,ht);
						cursorList.push_back(addCursor);

						ofNotifyEvent(cursorAdded, *(cursorList.back()), this);

					} else if ( ((*tit)->getX()!=xpos) || ((*tit)->getY()!=ypos) || ((*tit)->getXSpeed()!=xspeed) || ((*tit)->getYSpeed()!=yspeed) || ((*tit)->getMotionAccel()!=maccel) ) {
						(*tit)->update(xpos,ypos,xspeed,yspeed,maccel,wd,ht);

						ofNotifyEvent(cursorUpdated, **tit, this);
					}
				} else if( m.getArgAsString( 0 ) == "alive" ){
					if ((currentFrame<lastFrame) && (currentFrame>0)) return;
					for (int i=1;i<m.getNumArgs();i++) {

						long sid  = (long)(m.getArgAsInt32(i));;
						newCursorList.push_back((long)(sid));

						list<long>::iterator iter;
						iter = find(aliveCursorList.begin(), aliveCursorList.end(), (long)(sid));
						if (iter != aliveCursorList.end()) aliveCursorList.erase(iter);
					}
					list<long>::iterator alive_iter;
					for (alive_iter=aliveCursorList.begin(); alive_iter != aliveCursorList.end(); alive_iter++) {
						list<TuioCursor*>::iterator tit;
						for (tit=cursorList.begin(); tit != cursorList.end(); tit++) {
							TuioCursor *deleteCursor = (*tit);
							if(deleteCursor->getSessionId()==*alive_iter) {

								ofNotifyEvent(cursorRemoved, **tit, this);

								cursorList.erase(tit);

								if (deleteCursor->getFingerId()==maxFingerID) {
									maxFingerID = -1;
									delete deleteCursor;

									if (cursorList.size()>0) {
										list<TuioCursor*>::iterator clist;
										for (clist=cursorList.begin(); clist != cursorList.end(); clist++) {
											int fid = (*clist)->getFingerId();
											if (fid>maxFingerID) maxFingerID=fid;
										}

										list<TuioCursor*>::iterator flist;
										for (flist=freeCursorList.begin(); flist != freeCursorList.end(); flist++) {
											TuioCursor *freeCursor = (*flist);
											if (freeCursor->getFingerId()>maxFingerID) delete freeCursor;
											else freeCursorBuffer.push_back(freeCursor);
										}

										freeCursorList = freeCursorBuffer;
										freeCursorBuffer.clear();
									}
								} else if (deleteCursor->getFingerId()<maxFingerID) freeCursorList.push_back(deleteCursor);

								break;
							}
						}

					}
					aliveCursorList = newCursorList;
					newCursorList.clear();
				}
			} else if( m.getArgAsString( 0 ) == "fseq" ){

				if(currentFrame>0) lastFrame = currentFrame;
				currentFrame  = (int)(m.getArgAsInt32(1));
			}
		}
	};

	void drawCursors(){
		list<TuioCursor*>::iterator tit;
		for (tit=cursorList.begin(); tit != cursorList.end(); tit++) {
			TuioCursor *blob = (*tit);

			glColor3f(1.0,1.0,1.0);
			ofEllipse(blob->getX()*ofGetWidth(), blob->getY()*ofGetHeight(), 10.0, 10.0);
			string str = "SessionId: "+ofToString((int)(blob->getSessionId()));
			ofDrawBitmapString(str, blob->getX()*ofGetWidth()-10.0, blob->getY()*ofGetHeight()+25.0);
		}
	};

	void drawObjects(){

		list<TuioObject*>::iterator tobj;
		for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
			TuioObject *blob = (*tobj);
			glColor3f(1.0,0.0,0.0);
			glPushMatrix();
		    glTranslatef(blob->getX()*ofGetWidth(), blob->getY()*ofGetHeight(), 0.0);
			glRotatef(blob->getAngleDegrees(), 0.0, 0.0, 1.0);
			ofRect(-10.0, -10.0, 20.0, 20.0);
			glColor3f(1.0,1.0,1.0);
			ofLine(0, 0, 0, 10);
			glPopMatrix();
			string str = "FiducialId: "+ofToString((int)(blob->getFiducialId()));
			ofDrawBitmapString(str, blob->getX()*ofGetWidth()-10.0, blob->getY()*ofGetHeight()+25.0);
			str = "SessionId: "+ofToString((int)(blob->getSessionId()));
			ofDrawBitmapString(str, blob->getX()*ofGetWidth()-10.0, blob->getY()*ofGetHeight()+40.0);
		}
	};

	list<TuioObject*> getTuioObjects() {
		return objectList;
	};

	list<TuioCursor*> getTuioCursors() {
		return cursorList;
	};

	ofEvent<TuioCursor> cursorRemoved;
	ofEvent<TuioCursor> cursorAdded;
	ofEvent<TuioCursor> cursorUpdated;

	ofEvent<TuioObject> objectAdded;
	ofEvent<TuioObject> objectRemoved;
	ofEvent<TuioObject> objectUpdated;

private:
	ofxOscReceiver	receiver;
    
	list<TuioCursor*> cursorList;
	list<TuioCursor*> freeCursorList;
	list<TuioCursor*> freeCursorBuffer;
	list<TuioObject*> objectList;
	list<long> aliveObjectList;
	list<long> newObjectList;
	list<long> aliveCursorList;
	list<long> newCursorList;

	int currentFrame, lastFrame, maxFingerID;
};

#endif
