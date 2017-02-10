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
	printf("Line in create object %s\n",new_object);
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


void preorder(text_node *text) {
	if (text == NULL) {
		printf("Coming up\n");
		return;
	}
	printf("%s\t height : %d \t key : %d\n", (char *)text, text->height, text->key);
	printf("Going left\n");
	preorder(text->left);
	printf("Going right\n");
	preorder(text->right);
	printf("Coming up\n");
}

int length_text(text_node *text){
	
	return text == NULL ? 0 : text->key;
}



/* This function appends new line at the end*/
void append_line(text_node *text, char *new_line){
	text_node *tmp_node = text;
	int traversal_complete  = 0;
	if (text->left == NULL){
		text->left = (text_node *)create_object(new_line);//ask barve
		
		text->left->left = NULL;
		text->left->right = NULL;
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
//		printf("In append line %s\n", next_leaf->left);
//		printf("In append line char %s\n", (char *)next_leaf->left);
		next_leaf->left->left = NULL;
		next_leaf->left->right = NULL;
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
				//printf("Going on a preorder traversal before rotation\n\n**********************************\n");
				//preorder(text);

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
/* This function performs the inorder traversal of a given tree */
void inorder(text_node *text){
	if (text->right == NULL)	{
/*			printf("************************\n");
			printf("The key is %d\n", txt->key);
			printf("The height is %d\n", txt->height);*/
			printf("%s\n", (char *)text->left);
			//printf("The parent address is %d\n", txt->parent);
			

		return;
	}
	else if (text != NULL){

		inorder(text->left);
		
			
		inorder(text->right);
	}
}

text_node *get_to_line(text_node *text, int index) {
	text_node *tmp_node = text;
	if (text->key < index || index == 0 || text->key == 0) {
		return NULL;
	}
	else
	{
		while (tmp_node->right != NULL) {
			if (index <= tmp_node->left->key) {
				tmp_node = tmp_node->left;
			}
			else {
				index -= tmp_node->left->key;
				tmp_node = tmp_node->right;
			}
		}

		return (tmp_node);
	}
}
/* This function gets the line of number index, if such a line exists, and returns NULL else */
char *get_line(text_node *text, int index){
	text_node* tmp_node = get_to_line(text, index);
	return tmp_node == NULL ? NULL : (char *)(tmp_node->left);
}

/* This function sets the line of number index, if such a line exists, to new line, and returns a pointer to the previous line of that number. If no line of that number exists, it does not change the structure and returns NULL */
char *set_line(text_node *text, int index, char * new_line){
	text_node *tmp_node = get_to_line(text, index);
	text_node *old_node, *new_node;
	if (tmp_node == NULL) {
		return NULL;
	}
	new_node = (text_node *)create_object(new_line);
	old_node = tmp_node->left;
	tmp_node->left = new_node;

	return old_node;
}

 
void insert_line(text_node *text, int index, char * new_line){
	text_node *tmp_node = text;
	int traversal_complete;
	int count = index;
	if (text->left == NULL)
	{
		text->left = (text_node *)create_object(new_line);
		text->left->left = NULL;
		text->left->right = NULL;
		text->key = 1;
		text->right = NULL;
		text->parent = NULL;
		text->height = 0;
	}
	else if (index > (tmp_node->key))
	{
		append_line(text, new_line);
	}
	else
	{
		while (tmp_node->right != NULL)
		{
			(tmp_node->key)++;
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

		text_node *prev_leaf, *new_leaf;
		prev_leaf = get_node();
		new_leaf = get_node();
		prev_leaf->left = tmp_node->left;
		prev_leaf->key = 1;
		prev_leaf->right = NULL;
		prev_leaf->parent = tmp_node;
		prev_leaf->height = 0;

		new_leaf->left = (text_node *)create_object(new_line);
		new_leaf->left->left = NULL;
		new_leaf->left->right = NULL;
		new_leaf->key = 1;
		new_leaf->right = NULL;
		new_leaf->parent = tmp_node;
		new_leaf->height = 0;
		tmp_node->right = prev_leaf;
		tmp_node->left = new_leaf;
		(tmp_node->key)++;
		tmp_node->height = 0;

		traversal_complete = 0;
		while (!traversal_complete && tmp_node != NULL)
		{
			int tmp_height, prev_height;
			prev_height = tmp_node->height;
			if (tmp_node->left->height - tmp_node->right->height == 2)
			{
				if (tmp_node->left->left->height - tmp_node->right->height == 1)
				{
					right_rotation(tmp_node);
					tmp_node->right->height = tmp_node->right->left->height + 1;
					tmp_node->height = tmp_node->right->height + 1;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
					tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
				else
				{
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
			else if (tmp_node->left->height - tmp_node->right->height == -2)
			{
				if (tmp_node->right->right->height - tmp_node->left->height == 1)
				{
					left_rotation(tmp_node);
					tmp_node->left->height = tmp_node->left->right->height + 1;
					tmp_node->height = tmp_node->left->height + 1;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
					tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
				else
				{
					right_rotation(tmp_node->right);
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
			else
			{
				if (tmp_node->left->height > tmp_node->right->height)
				{
					tmp_node->height = tmp_node->left->height + 1;
				}
				else
				{
					tmp_node->height = tmp_node->right->height + 1;
				}
			}
			if (tmp_node->height == prev_height)
			{
				traversal_complete = 1;
			}
			tmp_node = tmp_node->parent;
		}
	}

}

int main() {
	text_node *text1;
	printf("Create a new text file\n");
	text1 = create_text_file();
	//printf("\n*******\nLength of text %d\n", length_text(text1));
	append_line(text1, "Hello");
	//printf("\n*******\nLength of text %d\n", length_text(text1));
	append_line(text1, "1234567890123456789");
	append_line(text1, "Today");
	append_line(text1, "It is windy");
	//printf("\n*******\nLength of text %d\n", length_text(text1));
	insert_line(text1, 2, "Line being inserted");
	insert_line(text1, 12, "12th line");

	//printf("Going for preorder \n");

	//preorder(text1);

	printf("Total number of lines");
	printf("\n*******\nLength of text %d\n", length_text(text1));
	
	printf("Fifth line is %s\n",get_line(text1, 5));
	printf("Sixth line is %s\n", get_line(text1, 6));
	printf("Pointer to old line %s\n",set_line(text1, 5, "Test Set Line"));
	printf("Fifth line is %s\n", get_line(text1, 5));
	printf("Sixth line is %s\n", get_line(text1, 6));
	getchar();
	return 0;
}
