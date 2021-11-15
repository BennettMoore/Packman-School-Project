//
//file: pack_encode.c
//
//@author Bennett Moore bwm7637@rit.edu
//
// // // // // // // // // // // // // //

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "HeapDT.h"
#include "packman_utils.h"
#include "pack_encode.h"


/**
 * @brief Compares two Tree_nodes and returns the less frequent of the two
 */
int compare_syms(const void * lhs, const void * rhs){
	Tree_node lhs_t = (Tree_node) lhs;
	Tree_node rhs_t = (Tree_node) rhs;
	return lhs_t->freq < rhs_t->freq;
}

/**
 * @brief Prints a Tree_node to a specific stream 
 */
void print_sym(const void * item, FILE * outfp){
	Tree_node item_t = (Tree_node) item;
	if(item_t->sym != NUL){
		fprintf(outfp, "%u:%c\n",item_t->freq, item_t->sym);
	}
	else{
		fprintf(outfp, "%u:*\n", item_t->freq);
	}
}

/**
 * @brief Find all symbols in the provided file
 */
Tree_node * find_syms(FILE * input){
	bool still_reading = true;
	size_t nmemb = BUFSIZE/BITS_PER_BYTE;
	size_t size = BITS_PER_BYTE;
	size_t read_size = 0;
	uint * syms = (uint *)calloc(MAXSYM, sizeof(uint));
	uchar * buffer = (uchar*)calloc(BUFSIZE, sizeof(uchar));

	//Find symbols and frequencies in text file
	for(size_t i = 0; i < MAXSYM; i++){
		syms[i] = 0;
	}

	while(still_reading){
		read_size = fread(buffer, size, nmemb, input);
	
		if(read_size != nmemb){ //Read everything	
			still_reading = false;
		}
		for(size_t i = 0; i < read_size; i++){
			syms[buffer[i]]++;
			buffer[i] = ' ';
		}
	}

	//Turn each unique symbol/frequency pair into a Tree_node and put it in an array
	Tree_node * nodes = (Tree_node *)calloc(MAXSYM, sizeof(Tree_node));
	size_t new_size = 0;


	for(int i = 0; i < MAXSYM; i++){
		if(syms[i] != 0){
			nodes[new_size] = create_tree_node(i, syms[i]);
			new_size++;
		}
	}

	//Housekeeeping
	nodes = realloc(nodes, new_size*sizeof(Tree_node));
	free(buffer);
	free(syms);

	return nodes;
}

/**
 * @brief Constructs interior nodes for the Huffman Tree
 */
Tree_node huffman_helper(Tree_node lhs, Tree_node rhs){
	uint tot_freq = 0;
	
	if(rhs != NULL){
		tot_freq = lhs->freq + rhs->freq;
	}
	else{
		tot_freq = lhs->freq;
	}
	
	//Creating the interior node
	Tree_node temp = create_tree_node(NUL, tot_freq);
	temp->left = lhs;
	temp->right = rhs;

	return temp;
}

/**
 * @brief Turns a collection of Tree_nodes into a Huffman tree
 */
Tree_node make_huffman_tree(Tree_node * nodes){
	Heap tree_heap = hdt_create(MAXSYM, compare_syms, print_sym);
	size_t node_num = sizeof(nodes)/sizeof(Tree_node*);
	
	//Sort nodes using a min-heap
	for(size_t i = 0; i < node_num; i++){
		hdt_insert_item(tree_heap, nodes[i]);
	}
	
	Tree_node root = NULL;
	while(hdt_size(tree_heap) > 2){
		//Creates an interior node from the two lowest-frequency nodes in the heap, and inserts an interior node with the previous nodes attached
		hdt_insert_item(tree_heap, huffman_helper(hdt_remove_top(tree_heap), hdt_remove_top(tree_heap)));
	}
	//Catch odd-numbered heaps
	if(hdt_size(tree_heap) == 2){
		root = huffman_helper(hdt_remove_top(tree_heap), NULL);
	}
	else{
		root = hdt_remove_top(tree_heap);
	}

	hdt_destroy(tree_heap);

	return root;
}

/**
 * @brief Recursively scouts out tree and records path to each symbol in lut
 */
void lut_helper(Tree_node root, char * path, char ** lut){
	
	if(root == NULL){} //NULL Checking
	else if(root->sym != NUL){ //Helper found something
		strcpy(lut[root->sym], path);
	}
	else{ //Helper found interior node
		char * left_path = strdup(path);
		char * right_path = strdup(path);
		strcat(left_path, "0");
		strcat(right_path, "1");

		lut_helper(root->left, left_path, lut);
		lut_helper(root->right, right_path, lut);
	}

	free(path);
	return;
}

/**
 * @brief generates a lookup table from a Huffman Tree
 */
char ** create_lut(Tree_node root){
	char ** lut = (char **)calloc(MAXSYM, sizeof(char *));
	char * path = (char *)calloc(BUFSIZE, sizeof(char));
	

	lut_helper(root, path, lut);
	
	free(path);
	return lut;
}

int main(void){ //Temporary main statement
	return 0;
}
