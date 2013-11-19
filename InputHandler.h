/*
 * InputHandler.h
 *
 *  Created on: Aug 5, 2013
 *      Author: vojcek
 */

#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_
#include "Object.h"
#include "PlayGround.h"

class InputHandler {
private:
	Object *obj;
	Object *last;
	RotateMini *rotate;
	bool inBar(int x,int y);
	bool inGround(int x,int y);
	int WB,HB,HP,WP,steps;
public:
	InputHandler(int WB,int HP,int HB,int WP);
	void processKeyPress(bool *exit, bool *stop, bool *pause);
	void processMouse(int button, ObjectBar *object_bar,PlayGround *play_ground);
	void drawCursor(ObjectBar *object_bar,PlayGround *play_ground);
	Object* getObj();
	virtual ~InputHandler();
};

#endif /* INPUTHANDLER_H_ */
