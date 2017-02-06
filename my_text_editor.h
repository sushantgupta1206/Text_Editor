#include<stdio.h>

#define BLOCKSIZE 256

typedef int key_type;
typedef int height_type;
typedef char object_type;

typedef struct text_node{
	key_type key;
	height_type height;
	struct text_node *left;
	struct text_node *right;
	struct text_node *parent;
} text_node;

text_node *currentblock = NULL, *free_list = NULL;
int size_left;
int nodes_taken = 0;
int nodes_returned = 0;

text_node* create_text();
int length_text(text_node *txt);
char* get_line(text_node *txt, int index);
void append_line(text_node *txt, char* new_line);
char* set_line(text_node *txt, int index, char* new_line);
void insert_line(text_node *txt, int index, char* new_line);
char* delete_line(text_node *txt, int index);