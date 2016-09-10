#include <stdio.h>
#include <stdlib.h>

#define TRUE	0
#define FLASE  -1
#define ERROR  -2

#define DEBUG_LINE() \
		printf("\033[33m[%s]\033[0m ------->line=%d\r\n", __func__, __LINE__)
#define DEBUG_INFO(fmt, args...) \
		printf("\033[33m[%s:%d]\033[0m "#fmt"\r\n", __func__, __LINE__, ##args)

#define NODE_EMPTY	(00)

typedef struct _node {
	int data;
	struct _node* next;
} node;

node* init_node(int value)
{
	node* head = (node*)malloc(sizeof(node));
	head->data = value;
	head->next = NULL;

	return head;
}

void create_list(node* head, int value)
{
	node* current_node = head;
	node* new_node = init_node(value);

	while(current_node->next != NULL) {
		current_node = current_node->next;
	}

	current_node->next = new_node;
}

node* init_list(int* arr, int len)
{
	int i;
	node* head = init_node(0);

	for(i=0; i<len; i++) {
		create_list(head, arr[i]);
	}

	return head;
}

int list_traversal(node* head)
{
	node* current_node = head;

	printf("Enter %s\n", __func__);

	if(!head)
		return ERROR;

	while(current_node->next != NULL) {
		current_node = current_node->next;
		printf("node data=%d\n", current_node->data);
	}

	return TRUE;
}

int destroy_list(node *head)
{
	node *L;

	if(!head)
		return ERROR;

	while(head) {
		L = head->next;
		free(head);
		head = L;
	}

	return TRUE;
}

int clear_list(node *head)
{
	node *L;

	if(!head)
		return ERROR;

	while(head) {
		L = head->next;
		head->data = NODE_EMPTY;
		head = L;
	}

	return TRUE;
}

int list_empty(node *head)
{
	int count = 0;

	if(!head)
		return ERROR;

	while(head->next != NULL) {
		if(head->data != NODE_EMPTY)
			count++;
/*		DEBUG_INFO("data=%d, count=%d", head->data, count);*/
		head = head->next;
	}

	return count;
}

int list_length(node *head)
{
	int count = 0;

	if(!head)
		return ERROR;

	while(head->next != NULL) {
		count++;
		head = head->next;
	}

	return count;
}

int get_elem(node *head, int i)
{
	int count = 0;

	if(!head)
		return ERROR;

	while(head->next != NULL) {
		++count;
		if(count == i)
			return head->data;
		head = head->next;
	}

	return TRUE;
}

int list_insert(node *head, int index, int value)
{
	DEBUG_LINE();
	int count = 0;
	node *L, *new_node;

	new_node = init_node(value);

	DEBUG_LINE();
	if(!head)
		return ERROR;

	DEBUG_LINE();
	if(list_length(head) < index - 1)
		return ERROR;

	DEBUG_LINE();
	while(head->next != NULL) {
		++count;
		if(count == index - 1) {
			L = head->next;
			head->next = new_node;
			new_node->next = L;
		}
		head = head->next;
	}

	return 0;
}

int list_delete(node *head, int index)
{
	int count = 0;
	node *L;

	if(!head)
		return ERROR;

	if(list_length(head) < index - 1)
		return ERROR;

	while(head->next != NULL) {
		++count;
		head = head->next;
		if(count == index - 1) {
			L = head->next;
			free(head)

		}
	}
	return 0;
}

int main(int argc, char const* argv[])
{
	int array[] = {1, 2, 3, 4, 5, 6, 7, 8 ,9, 0};

	node* list = init_list(array, sizeof(array)/sizeof(int));

	list_traversal(list);

	if(!list_empty(list))
		printf("List is empty!!!\n");

	DEBUG_INFO("list length=%d", list_length(list));
	DEBUG_INFO("list %d elem data=%d", 5, get_elem(list, 5));

/*	if(clear_list(list))*/
/*		printf("clear list node=0\n");*/

	list_insert(list, 9, 12);


	DEBUG_INFO("list length=%d", list_length(list));
	DEBUG_INFO("list %d elem data=%d", 4, get_elem(list, 4));
	DEBUG_INFO("list %d elem data=%d", 10, get_elem(list, 10));
	list_traversal(list);
	destroy_list(list);

	return 0;
}
