//
//file: pack_encode.h
//
//@author Bennett Moore bwm7637@rit.edu
//
// // // // // // // // // // // // // //

#ifndef PACK_ENCODE_H
#define PACK_ENCODE_H

#include <stdio.h>
#include "packman_utils.h"


/**
 * Compares two Tree_nodes and returns whether or not the left is less than the right
 *
 * @param lhs One of the Tree_nodes being compared
 * @param rhs The other Tree_node being compared
 *
 * @return 1 if lhs < rhs, 0 if lhs >= rhs
 *
 * @pre lhs and rhs are valid, non-empty Tree_nodes
 *
 */
int compare_syms(const void * lhs, const void * rhs);

/**
 * Prints out a Tree_node's information to a given stream
 *
 * @param item The Tree_node being printed
 * @param outfp The output stream being printed to
 *
 * @pre item is a valid, non-empty Tree_node
 *
 */
void print_sym(const void * item, FILE * outfp);

/**
 * Finds and catalogues all unique symbols in a given file and turns them into an array of Tree_nodes
 *
 * @param input A pointer to the file being scanned for symbols
 *
 * @returns An array of Tree_nodes corresponding to the symbols in input
 *
 * @pre input is a valid file
 *
 */
Tree_node * find_syms(FILE * input);

/**
 * Assists make_huffman_tree with construction by making all interior nodes necessary
 *
 * @param lhs The lefthand child of the new interior node
 * @param rhs The righthand child of the new interior node
 *
 * @return An interior node connected to its two child nodes
 *
 * @pre lhs and rhs are valid, non-empty Tree_nodes
 *
 */
Tree_node huffman_helper(Tree_node lhs, Tree_node rhs);

/**
 * Constructs a huffman tree from an array of Tree_nodes
 *
 * @param nodes An array of Tree_nodes
 *
 * @return The root of a huffman tree
 *
 * @pre The contents of nodes are all valid, non-empty Tree_nodes
 * @post nodes is freed after use
 *
 */
Tree_node make_huffman_tree(Tree_node * nodes);

/**
 * Assists create_lut by performing a depth-first search across the entire huffman tree
 *
 * @param root The current root of the tree being scanned
 * @param path The previous path taken to reach this point
 * @param lut The lookup table to write to
 *
 */
void lut_helper(Tree_node root, char * path, char ** lut);

/**
 * Creates a lookup table from a huffman tree
 *
 * @param root The root of a huffman tree
 *
 * @return A string array of the path to each and every possible symbol
 *
 * @pre root is the root of a valid, non-empty huffman tree
 *
 */
char ** create_lut(Tree_node root);

/**
 * Creates a bit array from an input file and its corresponding lookup table
 *
 * @param lut A lookup table with paths to each character
 * @param input The file which the lookup table describes
 *
 * @return An array of unsigned integers which contains the number of bits used at first, 
 * and then shows the contents of the bit storage array
 *
 * @pre lut is a valid, non-empty lookup table that corresponds to the contents of input
 * @post lut is freed after use
 *
 */
uint * make_bit_array(char ** lut, FILE * input);


#endif //PACK_ENCODE_H
