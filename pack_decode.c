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
 * @brief Helps decode_bits traverse through the huffmantree
 */
void decoder_helper(Tree_node root, uchar * instructions, uint num_bits, FILE * output){
	uint instruction_index = 0;
	Tree_node curr_node = root;

	//While there are still instructions
	while(instruction_index < num_bits){
		//While searching for a symbol
		while(curr_node->sym == NUL){
			if(instructions[instruction_index] == '0'){
				curr_node = curr_node->left;
			}
			else if(instructions[instruction_index] == '1'){
				curr_node = curr_node->right;
			}
			else{ //Illegal symbol
				errno = EINVAL;
				report_error("pack_decode:decoder_helper", __LINE__, "instructions", "Encountered instruction that was not '0' or '1'");
				return;
			}
			instruction_index++;
		}

		if(curr_node->sym == NUL){ //Found a NUL character
			errno = 157; //Internal Error
			report_error("pack_decode:decoder_helper", __LINE__, "curr_node", "Rogue NUL character discovered");
			return;
		}
		
		//Write the symbol found
		if(fwrite(&curr_node->sym, sizeof(uchar), 1, output) != 1){ //fwrite failed
			errno = EINTR;
			report_error("pack_decode:decoder_helper", __LINE__, "fwrite", "Could not write symbol to file");
			return;
		}
		else{ //fwrite succeeds
			curr_node = root;
		}
	}
	if(curr_node != root){ //Fragmented codeword(s) found
		errno = EILSEQ;
		report_error("pack_decode:decoder_helper", __LINE__, "root", "Instructions did not correctly line up to the provided huffman tree");
	}
}

/**
 * @brief Decodes bit array and writes decoded character to output file
 */
void decode_bits(Tree_node root, uint num_bits, uint * bit_array, FILE * output){
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
	
	//Decode from tree and write to file
	decoder_helper(root, instructions, num_bits, output);

	//Housekeeping
	free(instructions);
}

/**
 * @brief Parses encoded file info for later use
 */
void parse_file(FILE * input, FILE * output){

	rewind(input);

	//Read Tree
	Tree_node root = read_tree(input);

	if(root == NULL){ //read_tree failed
		errno = 157; //Internal Error
		report_error("pack_decode.c:parse_file", __LINE__, "read_tree", "Huffman tree could not be created from encoded file");
		return;
	}

	print_tree(root); //TODO Remove Temporary Print Statement
	
	//Read num_bits
	uint num_bits = 0;

	if(fread(&num_bits, sizeof(uint), 1, input) != 1){ //fread failed
		errno = EINTR;
		report_error("pack_decode.c:parse_file", __LINE__, "fread", "Number of bits used in the encoding not found");
		free_tree_node(root);
		return;
	}
	size_t array_size = (num_bits/BITS_IN_INT);//+1;

	//Read bit storage array	
	uint bit_array[array_size];//(uint *)calloc(array_size, sizeof(uint));
	size_t words_read = fread(bit_array, sizeof(uint), array_size, input); 
	
	if(words_read != array_size){ //fread failed
		fprintf(stderr, "pack_decode.c:parse_file:%i: Read in %lu words of bit_array, supposed to read in %lu words.\n", __LINE__, words_read, array_size);
	}
	else{
		decode_bits(root, num_bits, bit_array, output);
	}

	//free(bit_array);
	free_tree_node(root);
}
