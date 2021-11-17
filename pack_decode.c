//
// file: pack_decode.c
//
// @author Bennett Moore bwm7637@rit.edu
// // // // // // // // // // // // // //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "packman_utils.h"
#include "pack_decode.h"



/**
 * @brief Decodes bit array and writes decoded character to output file
 */
void decode_bits(Tree_node root, uint num_bits, uint * bit_array, FILE * output){
	(void) root; //TODO Remove temporary statement
	(void) output; //TODO Remove temporary statement
	uint bit_index = MAX_BIT_INDEX;
	uint array_index = 0;
	uchar * instructions = calloc(num_bits, sizeof(uchar));

	for(size_t i = 0; i < num_bits; i++){ //Scan every bit in bit_array
		if(bit_array[array_index] & 1<<bit_index){ //Bit is a 1
			instructions[i] = '1';
		}
		else{ //Bit is a 0
			instructions[i] = '0';
		}

		if(bit_index > 0){
			bit_index--;
		}
		else{
			bit_index = MAX_BIT_INDEX;
			array_index++;
		}
	}
	//TODO Make helper function to parse instructions via the tree
}

/**
 * @brief Parses encoded file info for later use
 */
void parse_file(FILE * input, FILE * output){

	//Read Tree
	Tree_node root = read_tree(input);

	if(root == NULL){ //read_tree failed
		errno = 157; //Internal Error
		report_error("pack_decode.c:parse_file", __LINE__, "read_tree", "Huffman tree could not be created from encoded file");
		return;
	}
	
	//Read num_bits
	uint num_bits = 0;

	if(fread(&num_bits, sizeof(uint), 1, input) != 1){ //fread failed
		errno = EINTR;
		report_error("pack_decode.c:parse_file", __LINE__, "fread", "Number of bits used in the encoding not found");
		free_tree_node(root);
		return;
	}
	size_t array_size = (num_bits/BITS_IN_INT)+1;

	//Read bit storage array	
	uint * bit_array = (uint *)calloc(array_size, sizeof(uint));
	size_t words_read = fread(bit_array, sizeof(uint), array_size, input); 
	
	if(words_read != array_size){ //fread failed
		fprintf(stderr, "pack_decode.c:parse_file:%i: Read in %lu words of bit_array, supposed to read in %lu words.\n", __LINE__, words_read, array_size);
	}
	
	decode_bits(root, num_bits, bit_array, output);

	free(bit_array);
	free_tree_node(root);
}

int main(void){ //Temporary main statement
	return 0;
}
