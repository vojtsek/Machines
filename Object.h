/*
 * Object.h
 *
 *  Created on: Aug 1, 2013
 *      Author: vojcek
 */

#include<allegro.h>
#include <iostream>
#include <stdlib.h>
#include "Listable.h"
#include "Mask.h"
#include "MyVector.h"
#define ACC_CONST 0.03
#ifndef OBJECT_H_
#define OBJECT_H_
#define BG_COL 0x008888
#define GRAVITY 1

class Object{
protected:
	int width,height;
	int start_x,start_y;
	int id;
	double x,y;
	bool laying;
	BITMAP *bmp, *big_bmp, *small_bmp;
	Mask *mask;
public:
	bool movable, compute_hit;
	double mass,k;
	Object();
	Object(double x,double y,BITMAP *bmp,int id);
	void setX(double x);
	void setY(double y);
	int getWidth();
	int getHeight();
	Mask* getMask();
	bool isScaled();
	void setSmall();
	void setBig();
	double getX();
	double getY();
	int getId();
	bool isLaying();
	void setLaying(bool l);
	bool isSmall();
	bool operator==(Object *o);
	int setDimH(int height);
	virtual void init();
	virtual void reset();
	virtual ~Object();
	void draw(BITMAP *screen,bool pic);
	void drawFrame(BITMAP *scr, int col);
	//virtual void step(BITMAP *scr);
	Object* detectColision(Node *first);
	virtual void processColision(Object *o);
};


class MovableObject : public Object{
private:
	LinkedList acceleration_list,delta_accelerations;
	int steps;
	MyVector result_vector;
	//virtual void detectColission();
public:
	MovableObject(int x,int y,BITMAP *bmp,int id) : Object(x,y,bmp,id) {
		init();
		movable = true;
	}
	void move(LinkedList *ol);
	void changeAcceleration(int order,MyVector *new_v);
	void processAcc();
	MyVector* getVector();
	virtual void init();
	virtual void reset();
	//virtual void step(BITMAP *scr);
	virtual void processColision(Object *o);
	~MovableObject();
};


class BowlingBall : public MovableObject{
public:
	BowlingBall(int x,int y) : MovableObject(x,y,load_tga("bowling_ball.tga",NULL),1) {mass = 30;k = 0.9;}
	BowlingBall() : MovableObject(0,0,load_tga("bowling_ball.tga",NULL),1) {mass = 30; k = 0.9;}
};

class TennisBall : public MovableObject{
public:
	TennisBall(int x,int y) : MovableObject(x,y,load_tga("tennis_ball.tga",NULL),2) {mass = 5; k = 0.3;}
	TennisBall() : MovableObject(0,0,load_tga("tennis_ball.tga",NULL),2) {mass = 5; k = 0.3;}
};

class BaseballBall : public MovableObject{
public:
	BaseballBall(int x,int y) : MovableObject(x,y,load_tga("baseball_ball.tga",NULL),3) {mass = 6; k = 0.4;}
	BaseballBall() : MovableObject(0,0,load_tga("baseball_ball.tga",NULL),3) {mass = 6; k = 0.4;}
};

class BasketballBall : public MovableObject{
public:
	BasketballBall(int x,int y) : MovableObject(x,y,load_tga("basketball_ball.tga",NULL),4) {mass = 12; k = 0.3;}
	BasketballBall() : MovableObject(0,0,load_tga("basketball_ball.tga",NULL),4) {mass = 12; k = 0.3;}
};

class SoccerBall : public MovableObject{
public:
	SoccerBall(int x,int y) : MovableObject(x,y,load_tga("soccer_ball.tga",NULL),5) {mass = 12; k = 0.4;}
	SoccerBall() : MovableObject(0,0,load_tga("soccer_ball.tga",NULL),5) {mass = 12; k = 0.4;}
};

class Star : public Object{
public:
	Star(int x,int y) : Object(x,y,load_tga("star.tga",NULL),6) {mass = 30; k = -2.0;}
	Star() : Object(0,0,load_tga("star.tga",NULL),6) {mass = 30; k = -2.0;}
};

class InclinedPlane : public Object{
public:
	InclinedPlane(int x,int y) : Object(x,y,load_tga("inclined_plane.tga",NULL),7) {mass = 10; k = 1;compute_hit = false;}
	InclinedPlane() : Object(0,0,load_tga("pine_hor.tga",NULL),7) {mass = 10; k = 1;compute_hit = false;}
};

class Slant : public Object{
public:
	Slant(int x,int y) : Object(x,y,load_tga("slant.tga",NULL),8) {mass = 10; k = 1;compute_hit = false;}
	Slant() : Object(200,200,load_tga("pine_ver.tga",NULL),8) {mass = 10; k = 1;compute_hit = false;}
};

class RotateMini : public Object{
public:
	Object *whose;
	RotateMini(int x,int y) : Object(x,y,load_tga("mini_rotate.tga",NULL),8) {mass = 0; k = 0;compute_hit = false; whose = NULL;}
	RotateMini() : Object(0,0,load_tga("mini_rotate.tga",NULL),8) {mass = 0; k = 0;compute_hit = false; whose = NULL;}
};

#endif /* OBJECT_H_ */
