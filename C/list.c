#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10

typedef struct Nod{
	char * data;
//	size_t dataSize;
	struct Nod * next;
} Node;

typedef struct Lst{
	Node *head;
	Node *tail;
//	Node array[0];
} List;

List * createList(){
	List * tmp = calloc(1, sizeof(List));
	return tmp;
};

void insertL(List *lst, void * data){
	if( !lst->head){
		lst->head = malloc(sizeof(Node));
		lst->tail = lst->head;
		lst->head->data= data;
		return;
	}
	Node * p = malloc(sizeof(Node));
	p->data = data;
	p->next = 0;
	lst->tail->next = p;
	lst->tail = p;
};

void printList(List *lst){
	printf("print:\n");
	Node *p = lst->head;
	while( p ){
		if(p->next)
			printf("%hhu->", *(p->data));
		else
			printf("%hhu\n", *(p->data));
		p = p->next;
	}
}

void destroyList(List * lst){
	Node *p = lst->head;
	Node * prev = 0;
	while (p) {
		prev = p;
		p = p->next;
		free(prev);
	}
	free(lst);
};

Node * findL(List * lst, char z){
	Node *tmp = 0;
	Node * prev = lst->head;
	if(*(prev->data) == z)
		return prev;
	while (prev != lst->tail)
	{
		if(*(prev->next->data) == z)
			return prev;
		prev = prev->next;
	}
	return 0;
};

void insertBefore(List *lst, Node *target, char *z) {
	Node *before = NULL;
	Node *to_find = lst->head;
	for (; to_find != target; to_find = to_find->next) {
		before = to_find;
	}
	Node *p = malloc(sizeof(Node));
	p->data = z;
	p->next = target;
	if (target == lst->head) {
		lst->head = p;
		return;
	}
	if (before) {
		before->next = p;
	}
}

void insertAfter(List *lst, Node *target, char *z) {
	Node *p = malloc(sizeof(Node));
	Node *tmp = target->next;
	p->data = z;
	p->next = target->next;
	target->next = p;
	if (target == lst->tail) {
		lst->tail = p;
	}
}

void deleteL(List *lst, Node * del){
	if(!del)
		return;
	Node *prev = lst->head;
	if( del == lst->head) {
		lst->head = lst->head->next;
		free(prev);
		return;
	}
	if(del->next == lst->tail) {
		free(lst->tail);
		del->next = 0;
		lst->tail = del;
		return;
	}
	Node * deleted = del->next;
	del->next = deleted->next;
	free(deleted);
};
int main() {
	char array[7] = {2, 4, 7, 8, 12, 44, 3};
	char ardel[7] = { 3, 8, 12, 44, 2, 4, 7};
	List *lst = createList();
	for( int i = 0 ; i < 7; i++)
		insertL(lst, array + i);
	printList(lst);

	//MY_TEST_START
	char test_data[4] = {66, 67, 68, 69};
	insertAfter(lst, lst->head, test_data);
	insertBefore(lst, lst->tail, test_data + 1);
	insertBefore(lst, lst->head, test_data + 2);
	insertAfter(lst, lst->tail, test_data + 3);
	printf("List after my inserts:\n");
	printList(lst);
	//MY_TEST_END

	Node *get = findL(lst, 12);
	if(get)
		printf("get: %hhu\n", *(get->data));
	get = findL(lst, 102);
	if(get)
		printf("get: %hhu\n", *(get->data));
	else
		printf("No\n");
	get = findL(lst, 2);
	deleteL(lst, get);
	printList(lst);
	get = findL(lst, 3);
	printf("get: %hhu\n", *(get->data));
	deleteL(lst, get);
	printList(lst);
	for( int i = 0; i < 9; i++) {  // 7 -> 9
		get = findL(lst, ardel[i]);
		if(get)
			printf("find: %hhu get: %hhu\n", ardel[i], *(get->data));
		else
			printf("find: %hhu No\n", ardel[i]);
		deleteL(lst, get);
		printList(lst);
	}

	destroyList(lst);
}
