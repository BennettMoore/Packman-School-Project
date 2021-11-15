//
//file: pack_encode.c
//
//@author Bennett Moore bwm7637@rit.edu
//
// // // // // // // // // // // // // //

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
 * @brief Turns a collection of Tree_nodes into a Huffman tree
 */
Tree_node make_huffman_tree(Tree_node * nodes){
	Heap tree_heap = hdt_create(MAXSYM, compare_syms, print_sym);
	size_t node_num = sizeof(nodes)/sizeof(Tree_node*);
	
	for(size_t i = 0; i < node_num; i++){
		hdt_insert_item(tree_heap, nodes[i]);
	}
	return NULL; //TODO turn heap into actual Huffman Tree
}

int main(void){ //Temporary main statement
	return 0;
}
