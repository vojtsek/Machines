/*
 * MyVector.h
 *
 *  Created on: Jul 31, 2013
 *      Author: vojcek
 */

#include "Listable.h"
#include <allegro.h>
#ifndef MYVECTOR_H_
#define MYVECTOR_H_

class MyVector {
private:
	double* components;
	double size;
	double start_x, start_y;
	void computeSize();
public:
	MyVector();
	MyVector(double x, double y);
	MyVector(MyVector& second);
	double getSize();
	void init();
	void setStart(int x, int y);
	double getStartX();
	double getStartY();
	void rotate(double angle);
	void setComponents(double x,double y);
	bool changeSize(double new_size);
	void  viz(BITMAP *scr,int x, int y);
	double getAngle(MyVector& v);
	MyVector operator+=(MyVector& v);
	MyVector operator*=(double s);
	double& operator[](int idx);
	virtual ~MyVector();
private:
	double scalarMul(MyVector& v);
};

#endif /* MYVECTOR_H_ */
