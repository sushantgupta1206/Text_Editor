#include<stdlib.h>

#include"my_text_editor.h"

text_node *get_node()
{
	text_node *tmp;
	nodes_taken += 1;
	if (free_list != NULL)
	{
		tmp = free_list;
		free_list = free_list->right;
	}
	else
	{
		if (currentblock == NULL || size_left == 0)
		{
			currentblock = (text_node *)malloc(BLOCKSIZE * sizeof(text_node));
			size_left = BLOCKSIZE;
		}
		tmp = currentblock++;
		size_left -= 1;
	}
	return (tmp);
}

/* This function assigns the default values for newly created node */
text_node *create_node(void)
{
	text_node *new_node;
	new_node = get_node();
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;
	new_node->key = 0;
	new_node->height = 0;
	return (new_node);
}

/* New instance of a text file*/
text_node *create_text_file() {
	text_node *new_node;
	new_node = create_node();
	return (new_node);
}

/* This function assigns the given line to a leaf node */
char *create_object(char *line){
	char *new_object = (char *)malloc(strlen(line) + 1);
	if (new_object == NULL)
	{
		perror("Malloc failed in create object");
		return NULL;
	}
	strcpy(new_object, line);
	return (new_object);
}

void left_rotation(text_node *node){
	text_node *tmp_node;
	tmp_node = node->left;
	node->left = node->right;
	node->right = node->left->right;
	node->right->parent = node;
	node->left->right = node->left->left;
	node->left->left = tmp_node;
	tmp_node->parent = node->left;
}

/* This function performs the right rotation of a tree */
void right_rotation(text_node *node){
	text_node *tmp_node;
	tmp_node = node->right;
	node->right = node->left;
	node->left = node->right->left;
	node->left->parent = node;
	node->right->left = node->right->right;
	node->right->right = tmp_node;
	tmp_node->parent = node->right;
}

/* This function appends new line at the end*/
void append_line(text_node *text, char *new_line){
	text_node *tmp_node = text;
	int traversal_complete  = 0;
	if (text->left == NULL){
		text->left = (text_node *)create_object(new_line);//ask barve
		text->right = NULL;
		text->parent = NULL;
		text->key = 1;
		text->height = 0;
	}
	else{

		while (tmp_node->right != NULL){
			(tmp_node->key)++;
			tmp_node = tmp_node->right;
		}

		text_node *prev_leaf, *next_leaf;
		prev_leaf = get_node();
		next_leaf = get_node();
		prev_leaf->left = tmp_node->left;
		prev_leaf->key = tmp_node->key;
		prev_leaf->right = NULL;
		prev_leaf->parent = tmp_node;
		prev_leaf->height = 0;

		next_leaf->left = (text_node *)create_object(new_line);
		next_leaf->key = tmp_node->key;
		next_leaf->right = NULL;
		next_leaf->parent = tmp_node;
		next_leaf->height = 0;
		tmp_node->left = prev_leaf;
		tmp_node->right = next_leaf;
		(tmp_node->key)++;
		tmp_node->height = 0;

		while (!traversal_complete && tmp_node != NULL){
			
			int tmp_height, prev_height;
			prev_height = tmp_node->height;
			
			if (tmp_node->left->height - tmp_node->right->height == 2){ //left subtree is greater than right. Needs rebalancing.
			
				if (tmp_node->left->left->height - tmp_node->right->height == 1){// RR rotation
					right_rotation(tmp_node);
					tmp_node->right->height = tmp_node->right->left->height + 1;
					tmp_node->height = tmp_node->right->height + 1;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right  ->key;
					tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
				else {//RL rotation
					left_rotation(tmp_node->left);
					right_rotation(tmp_node);
					tmp_height = tmp_node->left->left->height;
					tmp_node->left->height = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
					tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
			}
			else if (tmp_node->right->height - tmp_node->left->height == 2){//right subtree is longer than left subtree. Needs balancing 
			
				if (tmp_node->right->right->height - tmp_node->left->height == 1){//LL rotation
					left_rotation(tmp_node);
					tmp_node->left->height = tmp_node->left->right->height + 1;
					tmp_node->height = tmp_node->left->height + 1;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
					tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
				else{
					right_rotation(tmp_node->right); //LR rotation
					left_rotation(tmp_node);
					tmp_height = tmp_node->right->right->height;
					tmp_node->left->height = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
					tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
			}
			
			else{
			
				if (tmp_node->left->height > tmp_node->right->height){
					tmp_node->height = tmp_node->left->height + 1;
				}
				else{
					tmp_node->height = tmp_node->right->height + 1;
				}
			}
			
			if (tmp_node->height == prev_height){
				traversal_complete = 1;
			}

			tmp_node = tmp_node->parent;
		}
	}
}
/* This function returns the deleted node to the free list */
void return_node(text_node *node)
{  
	node->right = free_list;
   	free_list = node;
   	total_nodes_returned +=1;
}


/* This function is used to find the height of a tree or sub-tree */
int max_height(text_node *text) 
{
	int left_depth, right_depth;
	if (text->right == NULL) 
	{
		return 0;
	}
   	else
   	{
   		left_depth = max_height(text->left);
       		right_depth = max_height(text->right);
       		if (left_depth < right_depth) 
		{
           		return (right_depth+1);
       		}
		else
		{ 
			return (left_depth+1);
   		}
	}
} 

/* This function performs the inorder traversal of a given tree */
void inorder(text_node *text){
	if (text->right == NULL)	{
/*			printf("************************\n");
			printf("The key is %d\n", txt->key);
			printf("The height is %d\n", txt->height);*/
			printf("%s\n", (char *)txt->left);
			//printf("The parent address is %d\n", txt->parent);
			

		return;
	}
	else if (text != NULL){

		inorder(text->left);
		
			
		inorder(text->right);
	}
}

/* This function gets the line of number index, if such a line exists, and returns NULL else */
char *get_line(text_node *text, int index) 
{
	int count = index;
	text_node *tmp_node = text;
	if (text->key < index || index == 0 || text->key == -1)
	{
		return NULL;
	}
	else
	{
		while(tmp_node->right != NULL)
                {
                        if (count <= tmp_node->left->key)
                        {
                                tmp_node = tmp_node->left;
                        }
                        else
                        {
                                count -= tmp_node->left->key;
                                tmp_node = tmp_node->right;
                        }
                }
		return (char *)(tmp_node->left);
	}
} 

int main() {
	text_node *text1;
	printf("Create a new text file\n");
	text1 = create_text_file();
	append_line(text1, "Hello");
	append_line(text1, "How are you");
	append_line(text1, "Mr.  Gupta");
	printf("After appending.\n");
	printf("Going on an inorder traversal\n");
	inorder(text1);
	getchar();
	return 0;
}
