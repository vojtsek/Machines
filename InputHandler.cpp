/*
 * InputHandler.cpp
 *
 *  Created on: Aug 5, 2013
 *      Author: vojcek
 */

#include "InputHandler.h"
#include <allegro.h>
#include <stdlib.h>
#include "PlayGround.h"
using namespace std;

InputHandler::InputHandler(int WB,int HP,int HB,int WP) {
	this->WB = WB;
	this->HB = HB;
	this->WP = WP;
	this->HP = HP;
	this->obj = NULL;
	this->last = NULL;
	this->rotate = NULL;
	steps = 0;
}

void InputHandler::processKeyPress(bool *exit, bool *stop, bool *pause){
	int key;
	key = readkey() >> 8;
	switch(key){
		case(KEY_ESC):
			*exit = true;
			break;
		case(KEY_ENTER):
			if(!obj)
				*stop = true;
			break;
		case(KEY_P):
			*pause = !(*pause);
			break;
	}
	clear_keybuf();
}

bool InputHandler::inBar(int x, int y){
	if((x > 10) && (x < WB + 10) && (y > HP + 20) && (y < HP + 20 + HB))
		return true;
	return false;
}

bool InputHandler::inGround(int x, int y){
	if((x > 10) && (x < WP + 10) && (y > 10) && (y < HP+10))
		return true;
	return false;
}

void InputHandler::processMouse(int button, ObjectBar *object_bar, PlayGround* play_ground){
	int x;
	int y;
	/*if(!steps){
		clear_to_color(play_ground->getBmp(),BG_COL);
		play_ground->reDraw();   prober s bratrem
	}
	steps = (steps+1) % 10;*/
	Object *o;
	if(button & 1){
		mouse_b &=  ~1;
		x = mouse_pos >> 16;
		y =	mouse_pos & 0x0000ffff;
		if(!obj){
			if (inBar(x,y)){
				obj = object_bar->getPickedObject(x - 10,y-HP-20);
			}else if (inGround(x,y)){
				obj = play_ground->getPickedObject(x - 10,y-10,true,0,0);
				if(obj) obj->draw(play_ground->getBmp(),false);
			}
		}else{
			if (inBar(x,y)){
				object_bar->put(obj);
				obj = NULL;
				show_mouse(screen);
			}else if(inGround(x,y)){
				o = play_ground->getPickedObject(x - 10 ,y-10,false,obj->getWidth()/2,obj->getHeight() / 2);
				if((o == NULL) or (!(o->getMask()->collideWith(obj->getMask())))){
					play_ground->addObject(obj);
					obj->setX(x - 10);
					obj->setY(y - 10);
					obj->init();
					obj->reset();
					obj->draw(play_ground->getBmp(),true);
					play_ground->reDraw();
					obj = NULL;
					show_mouse(screen);
				}
			}
		}
	}
}

void InputHandler::drawCursor(ObjectBar *object_bar, PlayGround *play_ground){
	int x;
	int y;
	Object *o;
	if(obj){
		if(rotate){
			rotate->draw(play_ground->getBmp(),false);
			delete rotate;
			rotate = NULL;
			play_ground->reDraw();
		}
		if(last)
			last = NULL;
		show_mouse(NULL);
		x = mouse_pos >> 16;
		y =	mouse_pos & 0x0000ffff;
		obj->draw(screen,false);
		if((inBar(x,y)) && (!obj->isSmall()))
		obj->setSmall();
		else if((inGround(x,y)) && (obj->isSmall()))
			obj->setBig();
			//obj->drawFrame(screen,makecol(0,0,0),false);
		object_bar->reDraw();
		obj->setX(x-10);
		obj->setY(y-10);
		play_ground->reDraw();
		obj->draw(play_ground->getBmp(),true);
		if((o = play_ground->getPickedObject(x - 10,y-10,false,obj->getWidth()/2,obj->getHeight()/2)) && (o->getMask()->collideWith(obj->getMask()))){
			obj->drawFrame(screen,makecol(255,0,0));
			o->drawFrame(play_ground->getBmp(),makecol(255,0,0));
		}
	}else{
		x = mouse_pos >> 16;
		y =	mouse_pos & 0x0000ffff;
		if(inGround(x,y)){
			if (!rotate || !((rotate->getX() - rotate->getWidth() / 2 < x - 10) && (rotate->getX() + rotate->getWidth() / 2 > x - 10) && (rotate->getY() - rotate->getHeight() / 2 < y - 10) && (rotate->getY() + rotate->getHeight() / 2 > y - 10)))
				o = play_ground->getPickedObject(x - 10 ,y-10,false,0,0);
			else
				o = rotate->whose;;
			if(o && (!rotate || rotate->whose != o)){
				if (rotate){
				 	rotate->draw(play_ground->getBmp(),false);
				 	delete rotate;
				 	rotate = NULL;
				 	play_ground->reDraw();
				}
				rotate = new RotateMini();
				rotate->setX(o->getX() + o->getWidth()/2 + rotate->getWidth()/2);
				rotate->setY(o->getY() - o->getHeight()/2 + rotate->getHeight()/2);
				rotate->whose = o;
				rotate->draw(play_ground->getBmp(),true);
			}
			if(o){
				o->drawFrame(play_ground->getBmp(),makecol(0,200,0));
				if (rotate->whose != o){
				 	rotate->draw(play_ground->getBmp(),false);
				 	play_ground->reDraw();
					delete rotate;
					rotate = new RotateMini();
					rotate->setX(o->getX() + o->getWidth()/2 + rotate->getWidth()/2);
					rotate->setY(o->getY() - o->getHeight()/2 + rotate->getHeight()/2);
					rotate->whose = o;
					rotate->draw(play_ground->getBmp(),true);
				}
				rotate->draw(play_ground->getBmp(),true);
				if ((last != o) && last){
					rotate->draw(play_ground->getBmp(),false);
					play_ground->reDraw();

				}
				last = o;
			}else if(last){
				rotate->draw(play_ground->getBmp(),false);
				delete rotate;
				rotate = NULL;
				play_ground->reDraw();
				//last->draw(play_ground->getBmp(),false);
				//last->draw(play_ground->getBmp(),true);
				last = NULL;
			}else{
				if(!steps){
						play_ground->reDraw();
					}
					steps = (steps+1) % 10;
			}
		}
	}

}

Object* InputHandler::getObj(){
	return obj;
}

InputHandler::~InputHandler() {
	// TODO Auto-generated destructor stub
}

