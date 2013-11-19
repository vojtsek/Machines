/*
 * Mask.h
 *
 *  Created on: Aug 6, 2013
 *      Author: vojcek
 */
#include <stdlib.h>
#include <allegro.h>
#include "MyVector.h"
#ifndef MASK_H_
#define MASK_H_

class Mask {
private:
	bool **rows;
	int width,height,x,y,color;
	BITMAP *bmp;
	LinkedList *detected_points;
public:
	MyVector **sides;
	int *collision_pixel;
	Mask(int w, int h, int x, int y,BITMAP *bmp, int c);
	void setBmp(BITMAP *bmp);
	void createMask();
	void setColPix(int x, int y);
	void setSideVector(int out_bound, int in_bound,int delta_i, int delta_j,LinkedList *points);
	void fill();
	void init();
	bool collideWith(Mask *m);
	void show(BITMAP *scr);
	void setX(int x);
	void setY(int y);
	int containsPoint(LinkedList *list, int x, int y);
	void addPoint(LinkedList *list, int x, int y);
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	void setPix(int x, int y);
	bool getPix(int x, int y);
	MyVector* getDirection();
	virtual ~Mask();
};

#endif /* MASK_H_ */
