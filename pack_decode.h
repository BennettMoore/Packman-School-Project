//
// file: pack_decode.h
//
// @author Bennett Moore bwm7637@rit.edu
// // // // // // // // // // // // // //

#ifndef PACK_DECODE_H
#define PACK_DECODE_H

#include <stdio.h>
#include "packman_utils.h"

/**
 * Assists decode_bits by traversing the huffman tree and writing any symbols it finds
 *
 * @param root The root of a huffman tree
 * @param instructions A string of instructions corresponding to the huffman tree
 * @param num_bits The number of instructions in the string
 * @param output The file to write to
 *
 * @pre root is the root of a valid, non-empty huffman tree
 * @pre instructions is a valid set of instructions that will work on the given huffman tree
 * @pre output is a valid file that can be written to
 *
 */
void decoder_helper(Tree_node root, uchar * instructions, uint num_bits, FILE * output);

/**
 * Decodes the bit_array of a packman-encoded file into a set of instructions
 *
 * @param root The root of a huffman tree
 * @param num_bits The number of bits in the bit_array
 * @param bit_array The array of bits to be decoded, separated into unsigned integers
 * @param output The file to write to
 *
 * @pre root is the root of a valid, non-empty huffman tree
 * @pre output is a valid file that can be written to
 *
 */
void decode_bits(Tree_node root, uint num_bits, uint * bit_array, FILE * output);

/**
 * Parses a packman-encoded file into its components
 *
 * @param input The file being read
 * @param output The file to write to
 *
 * @pre input is a valid packman-encoded file that can be read
 * @pre output is a valid file that can be written to
 *
 */
void parse_file(FILE * input, FILE * output);


#endif //PACK_DECODE_H
