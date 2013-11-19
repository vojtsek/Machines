/*
 * Object.cpp
 *
 *  Created on: Aug 1, 2013
 *      Author: vojcek
 */

#include <allegro.h>
#include "Object.h"
#include "Listable.h"
#include <cmath>
#include <iostream>
using namespace std;
bool show_mask = 0;


Object::Object(double x,double y,BITMAP *bmp,int id){
	start_x = x;
	start_y = y;
	this->id = id;
	this->x =x;
	this->y = y;
	this->bmp = bmp;
	this->big_bmp = bmp;
	this->small_bmp = bmp;
	if(bmp){
		this->width = bmp->w;
		this->height = bmp->h;
		mask = new Mask(bmp->w,bmp->h,x,y,bmp,makecol(255,0,0));
	}
	movable = false;
	mass = 10;
	k = 0;
	compute_hit = true;
}

bool Object::operator ==(Object* o){
	if(!o)
		return false;
	if (id == o->id)
		return true;
	return false;
}

int Object::getId(){
	return id;
}

Object::Object(){
	id = 0;
	x = 0;
	y = 0;
	start_x = start_y = 0;
	width = height = 0;
	small_bmp = big_bmp = bmp = NULL;
	mask = NULL;
	movable = false;
	mass = 10;
	k = 0;
	compute_hit = false;
}

void Object::init(){
	setBig();
	start_x = x;
	start_y = y;
}

bool Object::isScaled(){
	if(small_bmp == big_bmp)
		return false;
	return true;
}

void Object::reset(){
	x = start_x;
	y = start_y;
}

int Object::getWidth(){
	return width;
}

int Object::getHeight(){
	return height;
}

double Object::getX(){
	return x;
}

double Object::getY(){
	return y;
}

int Object::setDimH(int new_height){
	if (new_height >= height)
		return height;
	BITMAP *new_b = create_bitmap((int)(width * ((double)new_height / height)),new_height);
	stretch_blit(bmp, new_b, 0, 0, bmp->w,bmp->h, 0, 0, new_b->w, new_b->h);
	small_bmp = new_b;
	return small_bmp->w;
}

void Object::setSmall(){
	bmp = small_bmp;
	width = bmp->w;
	height = bmp->h;
}

bool Object::isSmall(){
	if(bmp == small_bmp)
		return true;
	return false;
}

void Object::setBig(){
	bmp = big_bmp;
	width = bmp->w;
	height = bmp->h;
}

Object::~Object() {
	// TODO Auto-generated destructor stub
}

void Object::draw(BITMAP *scr,bool pic){
	if(pic){
		draw_trans_sprite(scr,bmp,x-(width/2),y-(height/2));
		//rectfill(scr,x-2,y-2,x+2,y+2,makecol(200,0,200));
	}else
		rectfill(scr,x-(width/2),y-(height/2),x+(width/2),y+(height/2),BG_COL);
	if(show_mask)
		mask->show(scr);
}

void Object::drawFrame(BITMAP *scr,int col){
	rect(scr,x-(width/2),y-(height/2),x+(width/2),y+(height/2),col);
	line(scr,x-(width / 4),y-(height/2),x+(width/4),y-(height/2),BG_COL);
	line(scr,x-(width / 4),y+(height/2),x+(width/4),y+(height/2),BG_COL);
	line(scr,x-(width / 2),y-(height/4),x-(width/2),y+(height/4),BG_COL);
	line(scr,x+(width / 2),y-(height/4),x+(width/2),y+(height/4),BG_COL);
}

void Object::processColision(Object *o){

}

void Object::setLaying(bool l){
	laying = l;
}

bool Object::isLaying(){
	return laying;
}

void Object::setX(double x){
	this->x = x;
	if(mask)
		mask->setX(x - width / 2);
}

void Object::setY(double y){
	this->y = y;
	if(mask)
		mask->setY(y - height / 2);
}

/*void Object::step(BITMAP *scr){
	this->draw(scr,true);
}
*/
Mask* Object::getMask(){
	return mask;
}


Object* Object::detectColision(Node *node){
	Object *obj;
	while(node){
		obj = (Object*) node->data;
		if(!obj)
			return NULL;
		if(obj == this){
			node = node->next;
			continue;
		}
		if(mask->collideWith(obj->getMask())){
			return obj;
		}
		node = node->next;
	}
	return NULL;
}

/*void MovableObject::step(BITMAP *scr){
	this->draw(scr,false);
	this->move();
	this->draw(scr,true);
}*/

void MovableObject::init(){
	steps = 0;
	start_x = x;
	start_y = y;
	result_vector.init();
	laying = false;
	double *delta = (double*) malloc(sizeof(double));
	(*delta) = 0.0;
	acceleration_list.addNode(new MyVector(0,3  * ACC_CONST),1);
	delta_accelerations.addNode(delta,1);
}

void MovableObject::reset(){
	x = start_x;
	y = start_y;
	acceleration_list.clear();
	delta_accelerations.clear();
	init();
}

MyVector* MovableObject::getVector(){
	return &result_vector;
}

void MovableObject::processColision(Object *o){
	MovableObject* mo;
	MyVector *u1, *u2, *axis;
	MyVector *u1n, *u1t, *u2n;
	double alfa1,alfa2,beta1;
	double m1,m2;
	double v1n_s,v1_s,k,fi,new_x,new_y,diff;
	if(o->compute_hit){
	m1 = mass;
	m2 = o->mass;
	if((this->k >= 0) && (o->k >= 0))
		k = 1 - abs(o->k - this->k);
	else
		k = (this->k < o->k) ? this->k + 1 - o->k : o->k + 1 - this->k ;
	if (o->movable){
		mo = (MovableObject*) o;
		u2 = mo->getVector();
	}else{
		mo = NULL;
		u2 = new MyVector(0,0.01);
	}
	u1 = (MyVector*) malloc(sizeof(MyVector));
	(*u1) = result_vector;
	axis = new MyVector(o->getX() - x, o->getY() - y);
	alfa1 = u1->getAngle((*axis));
	alfa2 = u2->getAngle((*axis));
	u1n = new MyVector((*u1)[0],(*u1)[1]);
	u1t = new MyVector((*u1)[0],(*u1)[1]);
	u2n = new MyVector((*u2)[0],(*u2)[1]);

	(*u1n) *= cos(alfa1);
	(*u1t) *= sin(alfa1);
	(*u2n) *= cos(alfa2);

	v1n_s = (m1 * u1n->getSize() + m2 * u2n->getSize() - k * m2 * (u1n->getSize() - u2n->getSize())) / (m1 + m2);
	v1_s = sqrt(pow(v1n_s,2) + pow(u1t->getSize(),2)); // u1t == v1t
	beta1 = atan(u1t->getSize() / v1n_s);

	MyVector y_axis(0,1);
	fi = axis->getAngle(y_axis);
	diff = x - o->getX();

	if((cos(fi) >= 0) && (diff > 0)){
		new_y = y - cos(beta1) * v1_s;
		new_x = x - sin(beta1) * v1_s;
	}else if((cos(fi) >= 0) && (diff < 0)){
		new_y = y - cos(beta1) * v1_s;
		new_x = x + sin(beta1) * v1_s;
	}else if((cos(fi) <= 0) && (diff < 0)){
		new_y = y + cos(beta1) * v1_s;
		new_x = x + sin(beta1) * v1_s;
	}else if((cos(fi) <= 0) && (diff > 0)){
		new_y = y + cos(beta1) * v1_s;
		new_x = x - sin(beta1) * v1_s;
	}

	if((o->getX() - x >= 0) && (o->getY() - y >= 0))
		fi *= -1;
	else if((o->getX() - x < 0) && (o->getY() - y < 0))
		fi *= -1;

	if(!fi) fi = 0.001;

	MyVector w(new_x - x,new_y - y);
	w.rotate(fi);

	result_vector = w;
	}else{
		Mask *m = o->getMask();
		int *col = m->collision_pixel;
		MyVector *v, *w;
		double min_dist = 2000;
		for(int i = 0; i < 4; i++){
			v = m->sides[i];
			MyVector norm((*v)[1],(*v)[0] * -1);
			double c = -norm[0] * (v->getStartX()) - norm[1] * v->getStartY();
			double dist = abs(norm[0] * col[0] + norm[1] * col[1] + c) / norm.getSize();
			if(dist < min_dist){
				min_dist = dist;
				w = v;
			}
		}
		MyVector x_axis(1,0);
		double alfa = x_axis.getAngle(*w);
		MyVector *u = new MyVector(0,1);
		u->rotate(3.141592 / 2 - alfa);
		double f = 4;
		u->changeSize(3 * ACC_CONST * sin(alfa) * f);
		MyVector *u_n = new MyVector((*u)[1],(*u)[0]);
		u_n->changeSize(3 * ACC_CONST * cos(alfa));
		Node *first = (Node*)acceleration_list.getFirst();
		if(acceleration_list.getCount() == 1){
			acceleration_list.addNode(u,1);
			acceleration_list.addNode(u_n,1);
		}else{
			first = first->next;
			if(first)
				(*(MyVector*)(first->data)) = (*u_n);
			first = first->next;
			if(first)
				(*(MyVector*)(first->data)) = (*u);
		}

		if(!laying){
			Node *first = (Node*)acceleration_list.getFirst();
			if((this->k >= 0) && (o->k >= 0))
				k = 1 - abs(o->k - this->k);
			else
				k = (this->k < o->k) ? this->k + 1 - o->k : o->k + 1 - this->k ;
			MyVector y_axis((*w)[1],(*w)[0] * -1);
			fi = result_vector.getAngle(y_axis);
			int q = -1;
			y_axis.rotate(fi);
			if(abs(result_vector.getAngle(y_axis)) >= abs(fi)){
				q = 1;
			}
			result_vector.rotate(q * 2 * (3.1415926535897 / 2 - fi));
			result_vector.changeSize(result_vector.getSize() - k * result_vector.getSize());
		}
		if(result_vector.getSize() < 0.5){
			setLaying(true);
			steps = 0;
		}
	}
}

void MovableObject::changeAcceleration(int order, MyVector *new_v){
	Node *node = static_cast<Node*>(acceleration_list.getFirst());
	int i = 0;
	MyVector* acc;
	while(node){
		i++;
		if(i == order){
			acc = (MyVector*)node->data;
			(*acc) = (*new_v);
		}
		node = (Node*) node->next;
	}
}

void MovableObject::move(LinkedList *object_list){

//	setX(x + result_vector[0]);
//	setY(y + result_vector[1]);
	Object *obj;
	obj = NULL;
	Node *node = static_cast<Node*> (object_list->getFirst());
	double max_it = result_vector.getSize();
	/*MyVector back_vector = result_vector;
	if(detectColision(node)){
		if(!laying)
			back_vector.rotate(3.1415926);
		else
			back_vector.rotate(3.1415926 / 2);
		back_vector.changeSize(1);
		while((obj = detectColision(node))){
			old_obj = obj;
			setX(x + back_vector[0]);
			setY(y + back_vector[1]);
		}
		setX(x - back_vector[0]);
		setY(y - back_vector[1]);*/
		if(!result_vector.getSize())
			processAcc();
		MyVector plus_vector = result_vector;
		plus_vector.changeSize(1);
		for(int i = 0; i < max_it;i++){
			setX(x + plus_vector[0]);
			setY(y + plus_vector[1]);
			if((obj = detectColision(node)))
				break;
		}
		if(obj){
			//setX(x - plus_vector[0]);
			//setY(y - plus_vector[1]);
			processColision(obj);
		}else
			processAcc();
	/*}else{
		Node *acceleration, *delta;
		delta = static_cast<Node*>(delta_accelerations.getFirst());
		acceleration = static_cast<Node*>(acceleration_list.getFirst());
		int i = 0;
		MyVector acc;
		while(1){
			if(!acceleration) break;
			i++;
			acc = ((MyVector)(*(MyVector*)acceleration->data));
			acc.changeSize(acc.getSize() + (*(double*) delta->data));
			result_vector += acc;
			if (!acceleration->next)
				break;
			acceleration = (Node*) acceleration->next;
			delta = (Node*)delta->next;
		}*/
	//}
}

void MovableObject::processAcc(){
	Node *acceleration, *delta;
	//delta = static_cast<Node*>(delta_accelerations.getFirst());
	acceleration = static_cast<Node*>(acceleration_list.getFirst());
	int i = 0;
	MyVector acc;
	while(1){
		if(!acceleration) break;
		i++;
		acc = ((MyVector)(*(MyVector*)acceleration->data));
		//acc.changeSize(acc.getSize() + (*(double*) delta->data));
		result_vector += acc;
		if (!acceleration->next)
			break;
		acceleration = (Node*) acceleration->next;
	//	delta = (Node*)delta->next;
	}
}

MovableObject::~MovableObject(){
	//delete &acceleration_list;
	//delete &result_vector;
}



