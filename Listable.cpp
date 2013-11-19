/*
 * Listable.cpp
 *
 *  Created on: Aug 1, 2013
 *      Author: vojcek
 */
#include "Listable.h"

LinkedList::LinkedList(){
	last = NULL;
	first = NULL;
	count = 0;
}


void LinkedList::addNode(void *new_data,bool end){
	Node *new_node = new Node(0);
	new_node->data = new_data;
	if(count){
		if (end){
			new_node->prev = last;
			last->next = new_node;
			last = new_node;
		}else{
			new_node->next = first;
			first->prev = new_node;
			first = new_node;
		}
	}else{
		last = new_node;
		first = new_node;
		new_node->next = NULL;
		new_node->prev = NULL;
	}
	count++;
}

void LinkedList::clear(){
	Node *p_node, *old;
	if (count){
		p_node=first;
		while(p_node){
			old = p_node;
			p_node = p_node->next;
			delete old;
		}
		last = first = NULL;
		count = 0;
	}
}

bool LinkedList::deleteNode(bool end){
	Node *old;
	if(count){
		if(end){

			old = last;
			old->prev->next = NULL;
			last = old->prev;
		}else{
			old = first;
			old->next->prev = NULL;
			first = old->next;
		}
		delete(old);
		count--;
		return 1;
	}
	return 0;
}

bool LinkedList::deleteNode(void *deleted_n){
	Node *deleted_node = (Node*) deleted_n;
	Node *p_node;
	if (count){
		p_node=first;
		while(p_node!=deleted_node){
			if(p_node == last) return 0;
			p_node=p_node->next;
		}
		if(p_node != first)
			p_node->prev->next=p_node->next;
		else
			first = p_node->next;
		if(p_node != last)
			p_node->next->prev=p_node->prev;
		else
			last = p_node->prev;
		count--;
		return 1;
	}else return 0;
}

bool LinkedList::deleteNode(int ord){
	int current = 1;
	Node *p_node;
	p_node = first;
	if(count < ord) return 0;
	while(current < ord){
		p_node = p_node->next;
		current++;
	}
	p_node->prev->next=p_node->next;
	p_node->next->prev=p_node->prev;
	delete p_node;
	count--;
	return 1;
}

int LinkedList::getCount(){
	return count;
}

void* LinkedList::getFirst(){
	return first;
}

LinkedList::~LinkedList(){
	Node *p_node, *old;
	p_node = first;
	while(p_node){
		old = p_node;
		p_node = p_node->next;
		delete old;
	}
}



