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
#include <errno.h>
#include "HeapDT.h"
#include "packman_utils.h"
#include "pack_encode.h"

#define BIT_STORAGE_BLOCK 5

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
	if(item_t == NULL){
		fprintf(outfp, "NULL\n");
	}
	else if(item_t->sym != NUL){
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
	size_t read_size = 0;
	uint * syms = (uint *)calloc(MAXSYM, sizeof(uint));
	uchar * buffer = (uchar*)calloc(BUFSIZE, sizeof(uchar));

	//Find symbols and frequencies in text file
	for(size_t i = 0; i < MAXSYM; i++){
		syms[i] = 0;
	}

	while(still_reading){
		read_size = fread(buffer, sizeof(uchar), BUFSIZE, input);
	
		if(read_size != BUFSIZE){ //Read everything	
			still_reading = false;
		}
		for(size_t i = 0; i < read_size; i++){
			syms[buffer[i]]++;
			buffer[i] = ' ';
		}
	}

	//Turn each unique symbol/frequency pair into a Tree_node and put it in an array
	Tree_node * nodes = (Tree_node *)calloc(MAXSYM, sizeof(Tree_node));

	for(int i = 0; i < MAXSYM; i++){
		nodes[i] = create_tree_node(i, syms[i]);
	}

	//Housekeeeping
	free(buffer);
	free(syms);

	return nodes;
}

/**
 * @brief Constructs interior nodes for the Huffman Tree
 */
Tree_node huffman_helper(Tree_node rhs, Tree_node lhs){
	uint tot_freq = 0;
	
	if(rhs != NULL && lhs != NULL){
		tot_freq = lhs->freq + rhs->freq;
	}
	else{
		errno = 158; //Bad parameters
		report_error("pack_encode:huffman_helper", __LINE__, "Parameters", "One or more treenodes passed to huffman_helper were NULL");
		return NULL;
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
	
	//Sort nodes using a min-heap
	for(size_t i = 0; i < MAXSYM; i++){
		if(nodes[i]->freq > 0){
			hdt_insert_item(tree_heap, nodes[i]);
		}
		else{
			free(nodes[i]);
		}
	}
	
	Tree_node root = NULL;
	while(hdt_size(tree_heap) > 2){
		//Creates an interior node from the two lowest-frequency nodes in the heap, and inserts an interior node with the previous nodes attached
		hdt_insert_item(tree_heap, huffman_helper(hdt_remove_top(tree_heap), hdt_remove_top(tree_heap)));
	}
	//Catch odd-numbered heaps
	if(hdt_size(tree_heap) == 2){
		root = huffman_helper(hdt_remove_top(tree_heap), hdt_remove_top(tree_heap));
	}
	else{
		root = hdt_remove_top(tree_heap);
	}
	
	//Housekeeping
	hdt_destroy(tree_heap);
	free(nodes);

	return root;
}

/**
 * @brief Recursively scouts out tree and records path to each symbol in lut
 */
void lut_helper(Tree_node root, char * path, char ** lut){
	
	if(root == NULL){ //NULL Checking
		fprintf(stderr,"pack_encode.c:lut_helper:%i: Helper encountered a NULL node. Encountered at: %s\n", __LINE__, path);
	}
	else if(root->sym != NUL){ //Helper found something
		lut[root->sym] = strdup(path);
	}
	else{ //Helper found interior node
		char * left_path = strdup(path);
		char * right_path = strdup(path);
		
		left_path = realloc(left_path, sizeof(left_path) + sizeof(uchar)*2);
		right_path = realloc(right_path, sizeof(right_path) + sizeof(uchar)*2);

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
	char * path = (char *)calloc(1, sizeof(char));
	
	lut_helper(root, path, lut);

	return lut;
}

/**
 * @brief generates a bit array from a file and its corresponding lut
 */
uint * make_bit_array(char ** lut, FILE * input){
	uint * bit_array = (uint *)calloc(BIT_STORAGE_BLOCK, sizeof(uint));
	uint bits_used = 0;
	uint bit_index = MAX_BIT_INDEX;
	uint array_size = BIT_STORAGE_BLOCK;
	uint array_index = 1;
	uchar buffer;
	
	//Rewind file to beginning
	if(fseek(input, 0, SEEK_SET) != 0){
		errno = ESPIPE;
		report_error("pack_encode.c:make_bit_array", __LINE__, "Input file", "File could not be rewound");
		free(bit_array);
		return NULL;
	}
	
	//While there are still characters in the file to be read
	while(fread(&buffer, sizeof(uchar), 1, input) == 1){
		if(!lut[buffer]){ //Encountered invalid character
			errno = 157; //Internal Error
			fprintf(stderr, "pack_encode.c:make_bit_array:%i: '%c' does not exist in the generated lookup table\n", __LINE__, buffer);
			free(bit_array);
			return NULL;
		}
		
		//For all characters in the specific codeword
		for(size_t i = 0; i < strlen(lut[buffer]); i++){
			
			if(lut[buffer][i] == '1'){ //1 should be placed in the uint
				bit_array[array_index] = bit_array[array_index] | 1 << bit_index;
			} //No need to place 0's since the uint is initialized to all 0

			if(bit_index > 0){ //Still bits in the uint
				bit_index--;
			}
			else{
				bit_index = MAX_BIT_INDEX;
				array_index++;
				
				//Need to allocate more space
				if(array_index >= array_size){
					array_size += BIT_STORAGE_BLOCK;
					bit_array = realloc(bit_array, array_size * sizeof(uint));
					
					if(bit_array == NULL){ //Realloc failed
						errno = 151; //Dynamic Allocation Error
						report_error("pack_encode.c:make_bit_array", __LINE__, "bit_array realloc", "Fatal error: Reallocation of bit_array failed");
						return NULL;
					}

					for(size_t j = array_index; j < array_size; j++){ //Set new allocated space to 0
						bit_array[j] = 0;
					}
				}
			}

			bits_used++;
		}
	}

	bit_array[0] = bits_used;

	//Housekeeping
	for(int i = 0; i < MAXSYM; i++){
		free(lut[i]);
	}
	free(lut);

	return bit_array;
}
