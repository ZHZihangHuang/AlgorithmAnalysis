#include <cstdlib>
#include <iostream>

#include "List.h"

using namespace std;

List::List(){
	head = NULL;
	curr = NULL;
	temp = NULL;
}

void List::AddNode(int addData){
	nodePtr n = new node;
	n->next = NULL;
	n->data = addData;

	if (head != NULL){
		curr = head;
		while(curr->next != NULL){
			curr = curr->next;
		}
		curr->next = n;
	}
	else
	{
		head = n;
	}
}

void List::DeleteNode(int delData){
	nodePtr delPtr = NULL;
	temp = head;
	curr = head;
	while (curr != NULL && curr->data != delData){
		temp = curr;
		curr = curr->next;
	}
	if (curr == NULL){
		cout << delData << " was not in the list" << endl;
		delete delPtr;
	}
	else {
		delPtr = curr;
		curr = curr->next;
		temp->next = curr;
		delete delPtr;
	}

}

void List::PrintList(){
	curr = head;
	while (curr != NULL){
		cout << curr->data << endl;
		curr = curr->next;
	}
}

//int *GetCurrentWomanPartner(int input_size_n){
//	int *current_woman_partner [input_size_n];
//	// use -1 as null since it is not easy to store different data type in c++
//	for (int i = 0; i < input_size_n; i++){
//		&current_woman_partner[i] = -1;
//	}
//	return *current_woman_partner;
//}
