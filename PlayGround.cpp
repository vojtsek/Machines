/*
 * PlayGround.cpp
 *
 *  Created on: Aug 4, 2013
 *      Author: vojcek
 */

#include "PlayGround.h"
#include "Object.h"
#include <allegro.h>
#include <math.h>
using namespace std;
PlayGround::PlayGround(BITMAP *bmp) {
	clear_to_color(bmp,BG_COL);
	this->bmp= bmp;
	this->width = bmp->w;
	this->height = bmp->h;
	drawFrame();
	steps = 0;
}

void PlayGround::drawFrame(){
	line(bmp,0,0,0,bmp->h-1,makecol(255,255,255));
	line(bmp,0,0,bmp->w-1,0,makecol(255,255,255));
	line(bmp,bmp->w-1,0,bmp->w-1,bmp->h-1,makecol(255,255,255));
	line(bmp,0,bmp->h-1,bmp->w-1,bmp->h-1,makecol(255,255,255));
}

int PlayGround::getWidth(){
	return width;
}

int PlayGround::getHeight(){
	return height;
}

BITMAP* PlayGround::getBmp(){
	return bmp;
}

void PlayGround::step(){
	if(!steps)
		clear_to_color(bmp,BG_COL);
	steps = (steps+1) % 5;
	Node *node;
	node = static_cast<Node*> (object_list.getFirst());
	Object *obj;
	while (node){
		obj = (Object*)node->data;
		obj->draw(bmp,false);
		node = node->next;
	}
	node = static_cast<Node*> (object_list.getFirst());
	while (node){
		obj = (Object*)node->data;
		if(obj->movable)
			((MovableObject*)obj)->move(&object_list);
		node = node->next;
	}
	/*node = static_cast<Node*> (object_list.getFirst());
	while (node){
		obj = (Object*)node->data;
		obj->detectColision((Node*)object_list.getFirst());
		node = node->next;
	}*/
	node = static_cast<Node*> (object_list.getFirst());
	while (node){
		obj = (Object*)node->data;
		obj->draw(bmp,true);
		node = node->next;
	}
	drawFrame();
}

void PlayGround::reset(){
	Node *object;
	clear_to_color(bmp,BG_COL);
	object = static_cast<Node*> (object_list.getFirst());
	Object *obj;
	while (object){
		obj = (Object*)object->data;
		obj->reset();
		obj->draw(bmp,true);
		object = object->next;
	}
	drawFrame();
}

void PlayGround::addObject(Object *new_object){
	object_list.addNode(new_object,true);
	new_object->init();
}

void PlayGround::reDraw(){
	clear_to_color(bmp,BG_COL);
	Node *node;
	node = static_cast<Node*> (object_list.getFirst());
		Object *obj;
		while (node){
			obj = (Object*)node->data;
			obj->draw(bmp,false);
			node = node->next;
		}
		node = static_cast<Node*> (object_list.getFirst());
		while (node){
			obj = (Object*)node->data;
			obj->draw(bmp,true);
			node = node->next;
		}
		drawFrame();
}

PlayGround::~PlayGround() {
	// TODO Auto-generated destructor stub
}

Object* PlayGround::getPickedObject(int x,int y,bool erase, int range_x, int range_y){

	Node *node;
	int ord = 0;
	node = static_cast<Node*> (object_list.getFirst());
	if(!node)
		return NULL;
	Object *obj;
	while(node){
		ord++;
		obj = (Object*)node->data;
		if(!obj)
			return NULL;
		if((abs(obj->getX() - x) <= (obj->getWidth() / 2) + range_x) && (abs(obj->getY() - y) <= (obj->getHeight() / 2) + range_y)){
			if(erase)
				object_list.deleteNode(node);
			return obj;
		}
		node = node->next;
	}
	return NULL;
}

void ObjectBar::addObject(Container *new_object){
	int w = new_object->getWidth();
	new_object->setXY(current_x + w / 2,(height-20) / 2 + 10);
	object_list.addNode(new_object,true);
	current_x += 20 + w;
}

void ObjectBar::reCompute(int x,int q,int ord){
	Node *object;
	int o = 0;
	object = static_cast<Node*> (object_list.getFirst());
	if(!object)
		return;
	Container *cont;
	bool change = false;
	while(object){
		o++;
		cont = (Container*)object->data;
		if(o == ord){
			object = object->next;
			change = true;
			continue;
		}
		if(change){
			cont->setXY(cont->getX() +((x + 10) * q),cont->getY());
		}
		object = object->next;
	}
}

Object* ObjectBar::getPickedObject(int x,int y){
	Node *object;
	int ord = 0;
	object = static_cast<Node*> (object_list.getFirst());
	if(!object)
		return NULL;
	Container *cont;
	Object *obj;
	while(object){
		ord++;
		cont = (Container*)object->data;
		obj = cont->pull(false);
		if(!obj){
			object = object->next;
			continue;
		}
		if((abs(obj->getX() - x) <= obj->getWidth()) && (abs(obj->getY() - y) <= obj->getHeight())){
			obj = cont->pull(true);
			if(!cont->pull(false))
				reCompute(obj->getWidth(),-1,ord);
			reDraw();
			return obj;
		}
		object = object->next;
	}
	return NULL;
}

void ObjectBar::put(Object *o){
	Node *object;
	int ord = 0;
	object = static_cast<Node*> (object_list.getFirst());
	if(!object)
		return;
	Container *cont;
	while(object){
		ord++;
		cont = (Container*) object->data;
		if (o->getId() == cont->getId()){
			cont->addObject(o);
			break;
		}
		object = object->next;
	}
	if(cont->getCount() == 1)
		reCompute(o->getWidth(),1,ord);
	reDraw();
}

int ObjectBar::getHeight(){
	return height;
}

void ObjectBar::reDraw(){
	clear_to_color(bmp,BG_COL);
	Node *object;
	object = static_cast<Node*> (object_list.getFirst());
	Container *obj;
	while (object){
		obj = (Container*)object->data;
		obj->draw(bmp);
		object = object->next;
	}
	drawFrame();
}

Container::Container(int height){
	this->height = height;
	width = 0;
	x = 0;
	y = 0;
	id = 0;
}

int Container::getId(){
	return id;
}

int Container::getY(){
	return y;
}

int Container::getCount(){
	return object_list.getCount();
}

int Container::getX(){
	return x;
}

Object* Container::pull(bool erase){
	Node *n = (Node*) object_list.getFirst();
	if(!n)
		return NULL;
	if (erase)
		object_list.deleteNode(n);
	return (Object*)n->data;
}

void Container::addObject(Object *new_object){
	if(!new_object->isScaled())
			this->width = new_object->setDimH(height - 20);
	id = new_object->getId();

	object_list.addNode(new_object,true);
}

void Container::draw(BITMAP *scr){

	Node *object = static_cast<Node*> (object_list.getFirst());
	if(!object)
			return;
	Object *obj = (Object *)object->data;
	obj->setX(x);
	obj->setY(y);
	obj->setSmall();
	obj->draw(scr,true);
	textprintf_ex(scr, font, x + obj->getWidth() / 2, y + obj->getHeight() / 2, makecol(255, 255, 255),-1, "%d", object_list.getCount());
}

int Container::getWidth(){
	return width;
}

void Container::setXY(int x,int y){
	this->x = x;
	this->y = y;
}
