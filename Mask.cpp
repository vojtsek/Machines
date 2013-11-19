/*
 * Mask.cpp
 *
 *  Created on: Aug 6, 2013
 *      Author: vojcek
 */

#include "Mask.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

Mask::Mask(int w, int h, int x, int y,BITMAP *bmp,int c) {
	width = w;
	height = h;
	this->x = x;
	this->y = y;
	rows = new bool*[w];
	collision_pixel = new int[2];
	sides = new MyVector*[4];
	init();
	setBmp(bmp);
	if(bmp)
		createMask();
	color = c;
	collision_pixel[1] = collision_pixel[0] = 0;
}

void Mask::setBmp(BITMAP *bmp){
	this->bmp = bmp;
}

typedef struct Point{
	int x;
	int y;
	int connected_x;
	int connected_y;
	int count;
	bool marked;
}POINT;

void Mask::createMask(){
	for(int i = 0; i < width;i++){
		for(int j = 0; j < height;j++){
			if(geta(getpixel(bmp,i,j))){
				rows[i][j] = true;
			}
		}
	}
	detected_points = new LinkedList();

	setSideVector(width,height,1,1,detected_points);
	setSideVector(height,width,1,-1,detected_points);
	setSideVector(width,height,-1,-1,detected_points);
	setSideVector(height,width,-1,1,detected_points);

	POINT *p1,*p2,*p3,*p4,*p;
		p1 = p2 = p3 = p4 = NULL;
		int max = 0;
		Node *n = (Node*)detected_points->getFirst();
		while(n){
			p = (POINT*) n->data;
			if(!p) break;
			if (p->x > max){
				max = p->x;
				p1 = p;
			}
			n = n->next;
		}
		p1->marked = true;
		max = 0;
		n = (Node*)detected_points->getFirst();
		while(n){
			p = (POINT*) n->data;
			if(!p) break;
			if(p->marked){
				n = n->next;
				continue;
			}
			if (p->x > max){
				max = p->x;
				p2 = p;
			}
			n = n->next;
		}
		p2->marked = true;
		n = (Node*)detected_points->getFirst();
		while(n){
			p = (POINT*) n->data;
			if(!p) break;
			if(p->marked){
				n = n->next;
				continue;
			}
			if (!p3)
				p3 = p;
			else
				p4 = p;
			n = n->next;
		}
		POINT *a, *b, *c, *d;
		sides[0] = new MyVector(p1->x - p2->x, p1->y - p2->y);
		sides[0]->setStart(p2->x,p2->y);
		sides[1] = new MyVector(p3->x - p4->x, p3->y - p4->y);
		sides[1]->setStart(p4->x,p4->y);
		if(p1->x == p2->x){
			a = (p1->y > p2->y) ? p1 : p2;
		}else
			a = (p1->x > p2->x) ? p1 : p2;
		if(p3->x == p4->x){
			b = (p3->y > p4->y) ? p3 : p4;
		}else
			b = (p3->x > p4->x) ? p3 : p4;
		sides[2] = new MyVector(a->x - b->x, a->y - b->y);
		sides[2]->setStart(b->x,b->y);

		c = (a == p1) ? p2 : p1;
		d = (b == p3) ? p4 : p3;
		sides[3] = new MyVector(c->x - d->x, c->y - d->y);
		sides[3]->setStart(d->x,d->y);

	//	sides[0]->viz(screen,x + b->x,y + b->y);
	//	sides[1]->viz(screen,x + p4->x,y + p4->y);
	//	sides[2]->viz(screen,x + d->x,y + d->y);
	//	sides[3]->viz(screen,x +p2->x,y+p2->y);

		circlefill(screen,x+p1->x,y+p1->y,2,makecol(255,255,0));
		circlefill(screen,x+p2->x,y+p2->y,2,makecol(255,255,0));
		circlefill(screen,x+p3->x,y+p3->y,2,makecol(255,255,0));
		circlefill(screen,x+p4->x,y+p4->y,2,makecol(255,255,0));
}


int Mask::containsPoint(LinkedList *list, int x, int y){
	Node *node;
	node = (Node*) list->getFirst();
	POINT *p;
	while(node){
		p = (POINT*) node->data;
		if (p){
			if((p->x == x) && (p->y == y))
				return p->count;
		}
		node = node->next;
	}
	return -1;
}

void Mask::addPoint(LinkedList *list, int x, int y){
	if(containsPoint(list,x,y) != -1)
		return;
	POINT *p = (POINT*) malloc(sizeof(Point));
	p->x = x;
	p->y = y;
	p->count = 0;
	p->marked = false;

	list->addNode(p,true);
}


void Mask::setSideVector(int out_bound, int in_bound,int delta_i,int delta_j,LinkedList *detected_points){
	int x1,y1,x2,y2;
	bool end = false;

	int i_bound = (delta_i == 1) ? 0 : out_bound;
	int j_bound = (delta_j == 1) ? 0 : in_bound;
	int q = (i_bound) ? 1 : 0;
	int r = (j_bound) ? 1 : 0;

	for(int i = i_bound - q; i != out_bound - i_bound - q;i+=delta_i){
		for(int j = j_bound - r; j != in_bound - j_bound - r ;j+=delta_j){
			//if(((width > height) && (out_bound > in_bound)) or ((width < height) && (out_bound < in_bound)) or ((width == height) && (delta_i == delta_j))){
			if(((width == out_bound) && (width != height)) or ((width == height) && (delta_i == delta_j))){
				if(rows[i][j]){
					addPoint(detected_points,i,j);
					end = true;
					break;
				}
			}else{
				if(rows[j][i]){
					addPoint(detected_points,j,i);
					end = true;
					break;
				}
			}
		}
		if(end) break;
	}
	q = (i_bound) ? 0 : 1;
	end = false;
	for(int i = out_bound - i_bound - q; i != i_bound -q  ;i-=delta_i){
		for(int j = j_bound - r; j != in_bound - j_bound - r ;j+=delta_j){
			//if(((width > height) && (out_bound > in_bound)) or ((width < height) && (out_bound < in_bound)) or ((width == height) && (delta_i == delta_j))){
			if(((width == out_bound) && (width != height)) or ((width == height) && (delta_i == delta_j))){
							if(rows[i][j]){
								addPoint(detected_points,i,j);
								end = true;
								break;
							}
						}else{
							if(rows[j][i]){
								addPoint(detected_points,j,i);
								end = true;
								break;
							}
						}
		}
		if(end) break;
	}//MyVector *v = new MyVector(x1 - x2, y1 - y2);
}

void Mask::fill(){
	for(int i = 0; i < width;i++){
		for(int j = 0; j < height;j++){
			rows[i][j] = true;
		}
	}
}

bool Mask::collideWith(Mask *m){
	int minX,minY;
	int reps = 0;
	if((abs(x + width / 2 - m->getX() - m->getWidth() / 2) < (floor((width + m->getWidth()) / 2))) && (abs(y + height / 2 - m->getY() - m->getHeight() / 2) < (floor((height  + m->getHeight()) / 2)))){
		if (x < m->getX())
			minX = x;
		else
			minX = m->getX();
		if (y < m->getY())
			minY = y;
		else
			minY = m->getY();
		Mask *new_m = new Mask((width + m->getWidth()),(height + m->getHeight()),minX,minY,NULL,makecol(255,255,0));
		for(int i = 0; i < width;i++){
			for(int j = 0; j < height;j++){
				if(rows[i][j]){
					reps++;
					new_m->setPix(i + x - minX,j + y - minY);
				}
			}
		}
		for(int i = 0; i < m->getWidth();i++){
			for(int j = 0; j < m->getHeight();j++){
				if((m->getPix(i,j)) && (new_m->getPix(i + m->getX() - minX,j + m->getY() - minY))){
					m->setColPix(i,j);
					return true;
				}
			}
		}
	}
	return false;
}

void Mask::setColPix(int i, int j){
	collision_pixel[0] = i;
	collision_pixel[1] = j;
}

MyVector* Mask::getDirection(){

}

void Mask::setPix(int i,int j){
	rows[i][j] = true;
}

bool Mask::getPix(int i,int j){
	return rows[i][j];
}

void Mask::setX(int x){
	this->x = x;
}

void Mask::setY(int y){
	this->y = y;
}

int Mask::getX(){
	return x;
}

int Mask::getY(){
	return y;
}

int Mask::getWidth(){
	return width;
}

int Mask::getHeight(){
	return height;
}

void Mask::show(BITMAP *scr){
	/*for(int i = 0; i < width;i++){
		for(int j = 0; j < height;j++){
			if(rows[i][j])
				putpixel(scr,x+i,y+j,color);
		}
	}*/
	for(int i = 0;i < 4;i++)
		sides[i]->viz(scr,x + sides[i]->getStartX(),y + sides[i]->getStartY());
}

void Mask::init(){
	for(int i = 0; i < width;i++){
		rows[i] = (bool*) malloc(height * sizeof(bool));
		for(int j = 0; j < height;j++){
			rows[i][j] = false;
		}
	}
}

Mask::~Mask() {
	// TODO Auto-generated destructor stub
}

