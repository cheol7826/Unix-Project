#include "linkedlist.h"

List* initList(char * dirname){
	List * h;

	h = (List*)malloc(sizeof(List));
	h->predir = h;
	h->nextdir = h;
	strcpy(h->dirname, dirname);
	return h;
}


void addList(List* head, char* dirname){
	List * li;

	li = (List*)malloc(sizeof(List));
	if(head->predir == head){
		head->predir = li;
		head->nextdir = li;
		li->predir = head;
		li->nextdir = head;
	} else{
		li->predir = head->predir;
		li->nextdir = head;
		head->predir->nextdir = li;
		head->predir = li;
	}
	strcpy(li->dirname, dirname);

}

void removeList(List* head){
	List * temp;
	if(strcmp(head->dirname, head->predir->dirname) != 0){
		temp = head->predir;
		temp->predir->nextdir = head;
		temp->nextdir->predir = temp->predir;
		free(temp);
	}
}


