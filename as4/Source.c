/* COP 3502C Assignment 4
This program is written by: Judah Libera */
//VS 2019 seems to have an issue with access violation, but runs fine everywhere else i've tested
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

FILE* ifp; //global file pointers
FILE* ofp;

#define MAXLEN 30//given max length for a string

typedef struct itemNode //given structs
{
	char name[MAXLEN];
	int count;
	struct itemNode* left, * right;
}item;

typedef struct treeNameNode
{
	char treeName[MAXLEN];
	struct treeNameNode* left, * right;
	item* tree;
}treename;

treename* searchtreename(treename* current_ptr, char treeName[MAXLEN]);
treename* replacetreenode(treename* current_ptr, treename* newnode);

void freeitems(item* root) //functions for freeing. similair except the root tree free also has call to free tree, which is this function
{
	if (root == NULL)
		return;
	freeitems(root->left);
	freeitems(root->right);
	free(root);

}

void freestuff(treename* root)
{
	if (root == NULL)
		return;
	freestuff(root->left);
	freeitems(root->tree);
	freestuff(root->right);
	free(root);

}

treename* treeinsert(treename* root, treename* element) //inserts a new treename into the bin tree
{
	if (root == NULL)
		root = element;
	else if (strcmp(element->treeName, root->treeName) > 0)
	{
		if (root->right != NULL)
			root->right = treeinsert(root->right, element);
		else
			root->right = element;
	}
	else
	{
		if (root->left != NULL)
			root->left = treeinsert(root->left, element);
		else
			root->left = element;
	}
	return root;
}

item* iteminsert(item* root, item* element) // inserts a new item into the item sub tree
{
	if (root == NULL)
		root = element;
	
	else if (strcmp(element->name, root->name) > 0)
	{
		if (root->right != NULL)
			root->right = iteminsert(root->right, element);
		else
			root->right = element;
	}
	else
	{
		if (root->left != NULL)
			root->left = iteminsert(root->left, element);
		else
			root->left = element;
	}
	return root;
}

treename* createtreenamenode(char name[]) //mallocs space and sets parameters, returning a node to be placed
{
	treename* node = (treename*)malloc(sizeof(treename));
	strcpy(node->treeName, name);
	node->left = NULL;
	node->right = NULL;
	return node;
}

item* createitemnode(char name[], int count)//mallocs space and sets parameters, returning a node to be placed
{
	item* node = (item*)malloc(sizeof(item));
	strcpy(node->name, name);
	node->count = count;
	node->left = NULL;
	node->right = NULL;
	return node;
}

treename* buildtreename(int numoftrees)//over arching funciton to coordnate creating and placing new nodes, returning the root of the tree to be used
{
	char name[MAXLEN];
	treename* node;
	treename* root = NULL;
	for (int i = 0; i < numoftrees; i++)
	{
		fscanf(ifp, "%s", name);
		node = createtreenamenode(name);
		root = treeinsert(root, node);
	}
	return root;
}

treename* buildtreeitems(int numoftrees, int numofitems, treename* root)//over arching funciton to coordnate creating and placing new nodes, returning the root of the tree to be placed into the parent tree
{
	char treeName[MAXLEN];
	char name[MAXLEN];
	int count;
	treename* treenode;
	item* node;
	for (int i = 0; i < numofitems; i++)
	{
		fscanf(ifp, "%s %s %d", treeName, name, &count);
		node = createitemnode(name, count);
		treenode = searchtreename(root, treeName);
		treenode->tree = iteminsert(treenode->tree, node);
		root = replacetreenode(root, treenode);
	}
	return root;
}

void displaytree(treename* root) //used for first display
{
	if (root != NULL)
	{
		displaytree(root->left);
		printf("%s ", root->treeName);
		fprintf(ofp, "%s ", root->treeName);
		displaytree(root->right);
	}
}

void displayitems(item* root) // displaying the sub tree
{
	if (root != NULL)
	{
		displayitems(root->left);
		printf("%s ", root->name);
		fprintf(ofp, "%s ", root->name);
		displayitems(root->right);
	}
}

void displayfull(treename* root) // controlling dispaly function
{
	if (root != NULL)
	{
		displayfull(root->left);
		printf("\n===%s===\n", root->treeName);
		fprintf(ofp, "\n===%s===\n", root->treeName);
		displayitems(root->tree);
		displayfull(root->right);
	}
}

treename* searchtreename(treename* current_ptr, char treeName[MAXLEN]) //recursively gets closer to searchecd for name
{
	if (current_ptr != NULL)
	{
		if (strcmp(current_ptr->treeName, treeName) == 0)
			return current_ptr;
		if (strcmp(current_ptr->treeName, treeName) > 0)
			return searchtreename(current_ptr->left, treeName);
		else
			return searchtreename(current_ptr->right, treeName);
	}
	else
	{
		return NULL;
	}
}

item* searchitem(item* current_ptr, char Name[MAXLEN])//recursively gets closer to searchecd for item
{

	if (current_ptr != NULL)
	{
		if (strcmp(current_ptr->name, Name) == 0)
			return current_ptr;
		if (strcmp(current_ptr->name, Name) > 0)
			return searchitem(current_ptr->left, Name);
		else
			return searchitem(current_ptr->right, Name);
	}
	else
	{
		return NULL;
	}
}

int itembefore(item* root, char Name[MAXLEN]) //recursively goes through entire tree assing one to counter if the item is before
{
	if (root != NULL)
	{

		if (strcmp(root->name, Name) < 0)
			return 1 + itembefore(root->right, Name) + itembefore(root->left, Name);
		else
			return itembefore(root->right, Name) + itembefore(root->left, Name);
	}
	else
		return 0;
}

treename* replacetreenode(treename* root, treename* newnode) // used whe nnodes are altered, replaces old node with altered version
{
	if (root != NULL)
	{
		if (strcmp(root->treeName, newnode->treeName) == 0)
		{
			root = newnode;
		}
		if (strcmp(root->treeName, newnode->treeName) > 0)
			root->left = replacetreenode(root->left, newnode);
		else
			root->right = replacetreenode(root->right, newnode);
	}
	return root;
}

int heights(item* node) // recursivley goes throiugh all the nodes then adds them on its way back, ultamatly returning whichever is higher
{
	int lh;
	int rh;

	if (node == NULL)
		return 0;

	lh = heights(node->left);
	rh = heights(node->right);

	if (lh > rh)
		return lh + 1;
	else
		return rh + 1;
}

int count(item* root) //adds count with recusive calls to go through each node
{
	if (root == NULL)
		return 0;
	return (root->count + count(root->left) + count(root->right));
}

treename* parent(treename* root, treename* node) // returns the parent of a given node - modified from given bst.c example
{

	// Take care of NULL cases.
	if (root == NULL || root == node)
		return NULL;

	// The root is the direct parent of node.
	if (root->left == node || root->right == node)
		return root;

	// Look for node's parent in the left side of the tree.
	if (strcmp(node->treeName, root->treeName) < 0)
		return parent(root->left, node);

	// Look for node's parent in the right side of the tree.
	else if (strcmp(node->treeName, root->treeName) > 0)
		return parent(root->right, node);

	return NULL; // Catch any other extraneous cases.

}

item* parentitem(item* root, item* node) // returns the parent of a given node - modified from given bst.c example
{

	// Take care of NULL cases.
	if (root == NULL || root == node)
		return NULL;

	// The root is the direct parent of node.
	if (root->left == node || root->right == node)
		return root;

	// Look for node's parent in the left side of the tree.
	if (strcmp(node->name, root->name) < 0)
		return parentitem(root->left, node);

	// Look for node's parent in the right side of the tree.
	else if (strcmp(node->name, root->name) > 0)
		return parentitem(root->right, node);

	return NULL; // Catch any other extraneous cases.

}

int isLeaf(treename* node)
{

	return (node->left == NULL && node->right == NULL);
}

treename* minVal(treename* root) // gets the minimum vlaue by returning the leftmost node - modified from given bst.c example
{

	// Root stores the minimal value.
	if (root->left == NULL)
		return root;

	// The left subtree of the root stores the minimal value.
	else
		return minVal(root->left);
}

item* minValitem(item* root)// gets the minimum vlaue by returning the leftmost node - modified from given bst.c example
{

	// Root stores the minimal value.
	if (root->left == NULL)
		return root;

	// The left subtree of the root stores the minimal value.
	else
		return minValitem(root->left);
}

treename* delete(treename* root, char name[MAXLEN]) // deletes a parent tree node - modified from given bst.c example
{

	treename* delnode, * new_del_node, * save_node;
	treename* par;
	char save_val[MAXLEN];

	delnode = searchtreename(root, name); // Get a pointer to the node to delete.

	par = parent(root, delnode); // Get the parent of this node.

	if (delnode == NULL)
		return NULL;

	// Take care of the case where the node to delete is a leaf node.
	if (delnode->left == NULL && delnode->right == NULL) //case 1
	{

		// Deleting the only node in the tree.
		if (par == NULL) {
			freeitems(root->tree);
			free(root); // free the memory for the node.
			return NULL;
		}

		// Deletes the node if it's a left child.
		if (strcmp(name, par->treeName) < 0) {
			freeitems(par->left->tree);
			free(par->left); // Free the memory for the node.
			par->left = NULL;
		}

		// Deletes the node if it's a right child.
		else {
			freeitems(par->right->tree);
			free(par->right); // Free the memory for the node.
			par->right = NULL;
		}

		return root; // Return the root of the new tree.
	}

	// Take care of the case where the node to be deleted only has a left
	// child.
	if (delnode->left != NULL && delnode->right == NULL) {

		// Deleting the root node of the tree.
		if (par == NULL) {
			save_node = delnode->left;
			freeitems(delnode->tree);
			free(delnode); // Free the node to delete.
			return save_node; // Return the new root node of the resulting tree.
		}

		// Deletes the node if it's a left child.
		if (strcmp(name, par->treeName) < 0) {
			save_node = par->left; // Save the node to delete.
			par->left = par->left->left; // Readjust the parent pointer.
			freeitems(save_node->tree);
			free(save_node); // Free the memory for the deleted node.
		}

		// Deletes the node if it's a right child.
		else {
			save_node = par->right; // Save the node to delete.
			par->right = par->right->left; // Readjust the parent pointer.
			freeitems(save_node->tree);
			free(save_node); // Free the memory for the deleted node.
		}

		return root; // Return the root of the tree after the deletion.
	}

	// Takes care of the case where the deleted node only has a right child.
	if (delnode->left == NULL && delnode->right != NULL) {

		// Node to delete is the root node.
		if (par == NULL) {
			save_node = delnode->right;
			freeitems(delnode->tree);
			free(delnode);
			return save_node;
		}

		// Delete's the node if it is a left child.
		if (strcmp(name, par->treeName) < 0) {
			save_node = par->left;
			par->left = par->left->right;
			freeitems(save_node->tree);
			free(save_node);
		}

		// Delete's the node if it is a right child.
		else {
			save_node = par->right;
			par->right = par->right->right;
			freeitems(save_node->tree);
			free(save_node);
		}
		return root;
	}
	//if your code reaches hear it means delnode has two children
	  // Find the new physical node to delete.
	new_del_node = minVal(delnode->right);
	strcpy(save_val, new_del_node->treeName);

	delete(root, save_val);  // Now, delete the proper value.

	// Restore the data to the original node to be deleted.
	strcpy(delnode->treeName, save_val);

	return root;
}

item* deleteitem(item* root, char name[MAXLEN]) // deletes an item from the sub tree - modified from given bst.c example
{

	item* delnode, * new_del_node, * save_node;
	item* par;
	char save_val[MAXLEN];

	delnode = searchitem(root, name); // Get a pointer to the node to delete.

	par = parentitem(root, delnode); // Get the parent of this node.

	if (delnode == NULL)
		return NULL;

	// Take care of the case where the node to delete is a leaf node.
	if (delnode->left == NULL && delnode->right == NULL) //case 1
	{

		// Deleting the only node in the tree.
		if (par == NULL) {
			free(root); // free the memory for the node.
			return NULL;
		}

		// Deletes the node if it's a left child.
		if (strcmp(name, par->name) < 0) {
			free(par->left); // Free the memory for the node.
			par->left = NULL;
		}

		// Deletes the node if it's a right child.
		else {
			free(par->right); // Free the memory for the node.
			par->right = NULL;
		}

		return root; // Return the root of the new tree.
	}

	// Take care of the case where the node to be deleted only has a left
	// child.
	if (delnode->left != NULL && delnode->right == NULL) {

		// Deleting the root node of the tree.
		if (par == NULL) {
			save_node = delnode->left;
			free(delnode); // Free the node to delete.
			return save_node; // Return the new root node of the resulting tree.
		}

		// Deletes the node if it's a left child.
		if (strcmp(name, par->name) < 0) {
			save_node = par->left; // Save the node to delete.
			par->left = par->left->left; // Readjust the parent pointer.
			free(save_node); // Free the memory for the deleted node.
		}

		// Deletes the node if it's a right child.
		else {
			save_node = par->right; // Save the node to delete.
			par->right = par->right->left; // Readjust the parent pointer.
			free(save_node); // Free the memory for the deleted node.
		}

		return root; // Return the root of the tree after the deletion.
	}

	// Takes care of the case where the deleted node only has a right child.
	if (delnode->left == NULL && delnode->right != NULL) {

		// Node to delete is the root node.
		if (par == NULL) {
			save_node = delnode->right;
			free(delnode);
			return save_node;
		}

		// Delete's the node if it is a left child.
		if (strcmp(name, par->name) < 0) {
			save_node = par->left;
			par->left = par->left->right;
			free(save_node);
		}

		// Delete's the node if it is a right child.
		else {
			save_node = par->right;
			par->right = par->right->right;
			free(save_node);
		}
		return root;
	}
	//if your code reaches hear it means delnode has two children
	  // Find the new physical node to delete.
	new_del_node = minValitem(delnode->right);
	strcpy(save_val, new_del_node->name);
	int savecount = new_del_node->count;

	deleteitem(root, save_val);  // Now, delete the proper value.

	// Restore the data to the original node to be deleted.
	strcpy(delnode->name, save_val);
	delnode->count = savecount;

	return root;
}

int main()
{
	ifp = fopen("in.txt", "r"); // in and out pointers
	ofp = fopen("out.txt", "w");
	int numoftrees, numofitems, numofquerie;

	fscanf(ifp, "%d %d %d", &numoftrees, &numofitems, &numofquerie);

	treename* trees = buildtreename(numoftrees); //builds trees adn stores them in the trees root node
	trees = buildtreeitems(numoftrees, numofitems, trees);

	displaytree(trees);
	displayfull(trees);
	printf("\n");
	fprintf(ofp, "\n");

	char command[MAXLEN]; //temp variables to run operations
	char treen[MAXLEN];
	char itemn[MAXLEN];
	int amount;
	treename* temptreenode = NULL;
	item* tempitemnode = NULL;
	for (int i = 0; i < numofquerie; i++) //all commands have nested cathes for if asked for nodes don't exist, the ncall functions to perform given operations
	{
		fscanf(ifp, "%s ", command);
		if (strcmp(command, "search") == 0)
		{
			fscanf(ifp, "%s %s", treen, itemn);

			temptreenode = searchtreename(trees, treen);
			if (temptreenode != NULL)
			{
				tempitemnode = searchitem(temptreenode->tree, itemn);
				if (tempitemnode == NULL)
				{
					printf("%s not found in %s\n", itemn, treen);
					fprintf(ofp, "%s not found in %s\n", itemn, treen);
				}
				else
				{
					printf("%d %s found in %s\n", tempitemnode->count, itemn, treen);
					fprintf(ofp, "%d %s found in %s\n", tempitemnode->count, itemn, treen);
				}
			}
			else
			{
				printf("%s does not exist\n", treen);
				fprintf(ofp, "%s does not exist\n", treen);
			}
		}
		else if (strcmp(command, "item_before") == 0)
		{
			fscanf(ifp, "%s %s", treen, itemn);

			int itbef;
			temptreenode = searchtreename(trees, treen);
			if (temptreenode == NULL)
			{
				printf("%s does not exist\n", treen);
				fprintf(ofp, "%s does not exist\n", treen);
			}
			else
			{
				tempitemnode = searchitem(temptreenode->tree, itemn);
				if (tempitemnode == NULL)
				{
					printf("%s not found in %s\n", itemn, treen);
					fprintf(ofp, "%s not found in %s\n", itemn, treen);
				}
				itbef = itembefore(temptreenode->tree, itemn);
				printf("item before %s: %d\n", itemn, itbef);
				fprintf(ofp, "item before %s: %d\n", itemn, itbef);
			}
		}
		else if (strcmp(command, "height_balance") == 0)
		{
			fscanf(ifp, "%s", treen);

			int lh, rh, diff;
			temptreenode = searchtreename(trees, treen);
			if (temptreenode != NULL)
			{
				if (tempitemnode == NULL)
				{
					lh = -1;
					rh = -1;
				}
				else
				{
					lh = heights(temptreenode->tree->left);
					rh = heights(temptreenode->tree->right);
				}
				diff = lh - rh;
				printf("%s: left height %d, right height %d, difference %d, ", treen, lh - 1, rh - 1, abs(diff));
				fprintf(ofp, "%s: left height %d, right height %d, difference %d, ", treen, lh - 1, rh - 1, abs(diff));
				if (abs(diff) > 1)
				{
					printf("not balanced\n");
					fprintf(ofp, "not balanced\n");
				}
				else
				{
					printf("balanced\n");
					fprintf(ofp, "balanced\n");
				}
			}
			else
			{
				printf("%s does not exist\n", treen);
				fprintf(ofp, "%s does not exist\n", treen);
			}
		}
		else if (strcmp(command, "count") == 0)
		{
			fscanf(ifp, "%s", treen);

			int total;
			temptreenode = searchtreename(trees, treen);
			if (temptreenode == NULL)
			{
				printf("%s does not exist\n", treen);
				fprintf(ofp, "%s does not exist\n", treen);
			}
			else
			{
				total = count(temptreenode->tree);
				printf("%s count %d\n", treen, total);
				fprintf(ofp, "%s count %d\n", treen, total);
			}
		}
		else if (strcmp(command, "reduce") == 0)
		{
			fscanf(ifp, "%s %s %d", treen, itemn, &amount);

			temptreenode = searchtreename(trees, treen);
			if (temptreenode != NULL)
			{
				tempitemnode = searchitem(temptreenode->tree, itemn);
				if (tempitemnode != NULL)
				{
					tempitemnode->count = tempitemnode->count - amount;
					if (tempitemnode->count <= 0)
					{
						tempitemnode = deleteitem(temptreenode->tree, itemn);
						temptreenode->tree = tempitemnode;
						replacetreenode(trees, temptreenode);
						printf("%s reduced\n", itemn);
						fprintf(ofp, "%s reduced\n", itemn);
					}
					else
					{
						temptreenode->tree = tempitemnode;
						replacetreenode(trees, temptreenode);
						printf("%s reduced\n", itemn);
						fprintf(ofp, "%s reduced\n", itemn);
					}
				}
				else
				{
					printf("%s not found in %s\n", itemn, treen);
					fprintf(ofp, "%s not found in %s\n", itemn, treen);
				}
			}
			else
			{
				printf("%s does not exist\n", treen);
				fprintf(ofp, "%s does not exist\n", treen);
			}
		}
		else if (strcmp(command, "delete") == 0)
		{
			fscanf(ifp, "%s %s", treen, itemn);

			temptreenode = searchtreename(trees, treen);
			if (temptreenode == NULL)
			{
				printf("%s does not exist\n", treen);
				fprintf(ofp, "%s does not exist\n", treen);
			}
			else
			{
				tempitemnode = deleteitem(temptreenode->tree, itemn);
				if (tempitemnode == NULL)
				{
					printf("%s does not exist in %s\n", itemn, treen);
					fprintf(ofp, "%s does not exist in %s\n", itemn, treen);
				}
				else
				{
					temptreenode->tree = tempitemnode;
					replacetreenode(trees, temptreenode);
					printf("%s deleted from %s\n", itemn, treen);
					fprintf(ofp, "%s deleted from %s\n", itemn, treen);
				}
			}
		}
		else if (strcmp(command, "delete_name") == 0)
		{
			fscanf(ifp, "%s", treen);

			temptreenode = searchtreename(trees, treen);
			if (temptreenode == NULL)
			{
				printf("%s does not exist\n", treen);
				fprintf(ofp, "%s does not exist\n", treen);
			}
			else
			{
				trees = delete(trees, treen);
				printf("%s deleted\n", treen);
				fprintf(ofp, "%s deleted\n", treen);
			}
		}
		else
		{
			printf("\n\nQuerie error.\n\n");
			fprintf(ofp, "\n\nQuerie error.\n\n");
		}
	}

	freestuff(trees);

	fclose(ifp);
	fclose(ofp);

	return 0;
}