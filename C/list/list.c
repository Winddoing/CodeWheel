#include <stdio.h>
#include <stdlib.h>

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

void traversal_list(node* head)
{
	node* current_node = head;

	while(current_node->next != NULL) {
		current_node = current_node->next;
		printf("node data=%d\n", current_node->data);
	}
}

int main(int argc, char const* argv[])
{
	int array[] = {1, 2, 3, 4, 5, 6, 7, 8 ,9, 0};

	node* head = init_list(array, sizeof(array)/sizeof(int));

	traversal_list(head);

	free(head);

	return 0;
}
