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

#ifndef TUIOCONTAINER
#define TUIOCONTAINER

#include "TuioPoint.h"

//--------------------------------------------------------
class TuioContainer : public TuioPoint{
	
public:
	
	TuioContainer(long _sid, float _xpos, float _ypos) : TuioPoint(xpos,  ypos){
		sessionId = _sid;
		xspeed = 0.0f;
		yspeed = 0.0f;
		motion_speed = 0.0f;
		motion_accel = 0.0f;
		xpos = _xpos;
		ypos = _ypos;
		
		width = 0.0f;
		height = 0.0f;
	};
	
	TuioContainer(TuioContainer * _tuioContainer) : TuioPoint(_tuioContainer){
		sessionId = _tuioContainer->getSessionId();
		xspeed = _tuioContainer->getXSpeed();
		yspeed = _tuioContainer->getYSpeed();
		motion_speed = _tuioContainer->getMotionSpeed();
		motion_accel = _tuioContainer->getMotionAccel();
		xpos = _tuioContainer->getX();
		ypos = _tuioContainer->getY();
		
		width = _tuioContainer->getWidth();
		height = _tuioContainer->getHeight();
	};
	
	virtual ~TuioContainer(){};
	
	virtual void update (float _xpos, float _ypos, float _xspeed, float _yspeed, float _maccel) {
		TuioPoint::update(_xpos, _ypos);
		xspeed = _xspeed;
		yspeed = _yspeed;
		motion_speed = (float)sqrt(_xspeed*_xspeed+_yspeed*_yspeed);
		motion_accel = _maccel;
	};
	
	virtual void update (float _xpos, float _ypos, float _xspeed, float _yspeed, float _maccel, float _width, float _height) {
		TuioPoint::update(_xpos, _ypos);
		xspeed = _xspeed;
		yspeed = _yspeed;
		motion_speed = (float)sqrt(_xspeed*_xspeed+_yspeed*_yspeed);
		motion_accel = _maccel;
		
		width = _width;
		height = _height;
	};
	
	virtual void update (TuioContainer * _tuioContainer) {
		TuioPoint::update(_tuioContainer);
		xspeed = _tuioContainer->getXSpeed();
		yspeed = _tuioContainer->getXSpeed();
		motion_speed = _tuioContainer->getMotionSpeed();
		motion_accel = _tuioContainer->getMotionAccel();
	};
	
	virtual long getSessionId() { 
		return sessionId; 
	};
	
    virtual TuioPoint getPosition() {
		TuioPoint p(xpos,ypos);
		return p;
	};
	
	virtual float getXSpeed(){
		return xspeed;
	};
	
	virtual float getYSpeed(){
		return yspeed;
	};
	
	virtual float getMotionSpeed(){
		return motion_speed;
	};
	
	virtual float getMotionAccel(){
		return motion_accel;
	};
	
	virtual float getWidth(){
		return width;
	};
	
	virtual float getHeight(){
		return height;
	};
	
protected:
	long sessionId;
    float xspeed, yspeed;
	float motion_speed, motion_accel;
	
	float width, height;
};

#endif	
