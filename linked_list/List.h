#ifndef LIST_H
#define LIST_H

class List{
private:
	typedef struct node{
		int data;
		node* next;
		}* nodePtr;
	nodePtr head;
	nodePtr curr;
	nodePtr temp;
	//int current_woman_partner [input_size_n];
public:
	List();
	void AddNode(int addData);
	void DeleteNode(int delData);
	void PrintList();
	//int current_woman_partner [input_size_n];
	int *GetCurrentWomanPartner(int input_size_n);
};

#endif
