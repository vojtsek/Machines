/*
 * Listable.h
 *
 *  Created on: Aug 1, 2013
 *      Author: vojcek
 */
#include<stdio.h>

#ifndef LISTABLE_H_
#define LISTABLE_H_
class Node{
public:
	double value;
	Node *next,*prev;
	void *data;
	Node(double v){
		value = v;
		next = NULL;
		prev = NULL;
		data = NULL;
	}
};

class LinkedList{
private:
	Node *first,*last;
	int count;
public:
	LinkedList();
	~LinkedList();
	void addNode(void *new_node,bool end);
	bool deleteNode(bool end);
	bool deleteNode(void *deleted_node);
	bool deleteNode(int ord);
	int getCount();
	void* getFirst();
	void clear();
};
#endif /* LISTABLE_H_ */
