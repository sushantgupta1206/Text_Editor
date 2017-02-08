void insert_line(text_node *text, int index, char * new_line) 
{
	text_node *tmp_node = text;
	int traversal_complete;
	int count = index;
	if (text->left == NULL)
	{
		text->left = (text_node *) create_object(new_line); 
      		text->key  = 1;
      		text->right  = NULL;
		text->parent = NULL;
		text->height = 0;
	}
	else if (index > (tmp_node->key))
	{
		append_line(text, new_line);
	}	
	else
	{
		while(tmp_node->right != NULL)
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
        	prev_leaf->right  = NULL;
        	prev_leaf->parent = tmp_node;
        	prev_leaf->height = 0;

        	new_leaf->left = (text_node *) create_object(new_line);
        	new_leaf->key = 1;
        	new_leaf->right  = NULL;
        	new_leaf->parent = tmp_node;
        	new_leaf->height = 0;
        	tmp_node->right = prev_leaf;
        	tmp_node->left = new_leaf;
		(tmp_node->key)++; 
		tmp_node->height = 0;
		  
		traversal_complete = 0;
		while(!traversal_complete && tmp_node!=NULL)
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
				if( tmp_node->right->right->height - tmp_node->left->height == 1)
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
				if(tmp_node->left->height > tmp_node->right->height)
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
