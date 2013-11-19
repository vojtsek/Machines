/*
 * MyVector.cpp
 *
 *  Created on: Jul 31, 2013
 *      Author: vojcek
 */

#include "MyVector.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

void MyVector::computeSize(){
	size = sqrt(pow(components[0],2) + pow(components[1],2));
}

MyVector::MyVector(){
	components = new double[2];
	start_x = start_y = 0;
	init();
}

MyVector::MyVector(double x,double y){
	components = new double[2];
	components[0] = x;
	components[1] = y;
	start_x = start_y = 0;
	computeSize();
}

void MyVector::setStart(int x, int y){
	start_x = x;
	start_y = y;
}

double MyVector::getStartX(){
	return start_x;
}

double MyVector::getStartY(){
	return start_y;
}

MyVector::MyVector(MyVector& second){
	components = new double[2];
	components[0] = second[0];
	components[1] = second[1];
	computeSize();
}

void MyVector::init(){
	components[0] = 0;
	components[1] = 0;
	size = 0;
}

MyVector::~MyVector() {
	//	delete[] components;
}

double MyVector::getSize(){
	return this->size;
}

double& MyVector::operator [](int idx){
	return components[idx];
}

void MyVector::setComponents(double x, double y){
	components[0] = x;
	components[1] = y;
	computeSize();
}

MyVector MyVector::operator +=(MyVector &v){
	components[0] += v[0];
	components[1] += v[1];
	computeSize();
	return *this;
}



MyVector MyVector::operator *=(double s){
	components[0] *= s;
	components[1] *= s;
	computeSize();
	return *this;
}

double MyVector::scalarMul(MyVector &v){
	return (double)((*this)[0] * v[0]) + ((*this)[1] * v[1]);
}

double MyVector::getAngle(MyVector &v){
	if((!size) or (!v.getSize()))
		return 0;
	double cos = scalarMul(v) / (getSize() * v.getSize());
	return acos(cos);
}

void  MyVector::viz(BITMAP *scr,int x, int y){
	int o_size = size;
	//changeSize(size * 10);
	line(scr,x,y,x+components[0],y+components[1],makecol(222,222,0));
	changeSize(o_size);
}

void MyVector::rotate(double angle){
	double rx = cos(angle) * components[0] - sin(angle) * components[1];
	double ry = sin(angle) * components[0] + cos(angle) * components[1];
	components[0] = rx;
	components[1] = ry;
}

bool MyVector::changeSize(double new_size){
	/*int x_sign = (components[0]) ? components[0] / abs(components[0]) : 0;
	int y_sign = (components[1]) ? components[1] / abs(components[1]) : 0;
	if(!new_size) return false;
	double ratio;*/
	if (new_size <= 0)
	{
		components[0] = 0;
		components[1] = 0;
		computeSize();
		return false;
	}
	/*
	ratio = pow(new_size,2) / pow(size,2);
	components[0] = x_sign * sqrt(pow(components[0],2)*ratio);
	components[1] = y_sign * sqrt(pow(components[1],2)*ratio);
	computeSize();
	return 1;*/
	(*this) *= new_size / size;
	return true;
}


