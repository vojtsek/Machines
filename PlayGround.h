/*

 * PlayGround.h
 *
 *  Created on: Aug 4, 2013
 *      Author: vojcek
 */

#ifndef PLAYGROUND_H_
#define PLAYGROUND_H_
#include "Listable.h"
#include "Object.h"
#include <allegro.h>
class PlayGround {
protected:
	LinkedList object_list;
	BITMAP *bmp;
	int width;
	int height;
	int steps;
public:
	int getWidth();
	int getHeight();
	void drawFrame();
	void step();
	BITMAP* getBmp();
	void reset();
	void addObject(Object *new_object);
	void reDraw();
	Object* getPickedObject(int x,int y,bool erase,int range_x, int range_y);
	PlayGround(BITMAP *bmp);
	virtual ~PlayGround();
};

class Container{
private:
	LinkedList object_list;
	int x,y,height,width,id;
public:
	Container(int height);
	void addObject(Object *new_object);
	void draw(BITMAP *scr);
	int getWidth();
	int getX();
	int getY();
	int getId();
	int getCount();
	Object* pull(bool erase);
	void setXY(int x,int y);
};

class ObjectBar : PlayGround{
private:
	int current_x;
public:
	ObjectBar(BITMAP *bmp) : PlayGround(bmp) {
		current_x = 10;
		drawFrame();
	}
	void addObject(Container *new_object);
	int getHeight();
	void reDraw();
	Object* getPickedObject(int x,int y);
	void reCompute(int width,int q,int ord);
	void put(Object *obj);
};

#endif /* PLAYGROUND_H_ */
