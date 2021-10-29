//
// File: packman_utils.h
// Description: 
//    Utility declarations and definitions for Packman, the Encoder-Decoder
//    packman is an encoder/decoder program using huffman encoding.
//
// @author ben k steele, bks@cs.rit.edu
//
// // // // // // // // // // // // // // // // // // // // // // // //

#ifndef PACKMAN_UTILS_H
#define PACKMAN_UTILS_H

// include files needed for class declaration
#include <stdio.h>    // FILE
#include <stdint.h>   // uint8_t uint16_t
#include <stdbool.h>  // bool

// === typedefs

/// uchar is an alias for the unsigned char type, uint8_t.
typedef uint8_t uchar;

/// ushort is an alias for the unsigned short type, uint16_t.
typedef uint16_t ushort;

/// uint is an alias for the unsigned int type, uint32_t.
typedef uint32_t uint;

// === definitions

/// buffer size is for file I/O processing.
#define BUFSIZE  256

/// maximum number of symbols is 256; 2^8 where 8 is number of bits in a byte.
#define MAXSYM  2 << 8

/// NUL is the null character byte for string termination.
#define NUL  '\0'

/// BITS_PER_LEAF is number of bits per leaf node in a 'tree file' object.
#define BITS_PER_LEAF  8

/// BITS_PER_BYTE is used for encoding/decoding

#define BITS_PER_BYTE  8

/// BITS_IN_INT is used for encoding/decoding

#define BITS_IN_INT    ( sizeof(uint) * BITS_PER_BYTE )

/// MAX_BIT_INDEX is used for encoding/decoding

#define MAX_BIT_INDEX  ( BITS_IN_INT - 1 )

// === magic function

/// get_magic returns the 'magic number' for binary packman files.
/// Each packman-encoded binary file begins with this value.
/// @return unsigned short representing the magic value.

unsigned short get_magic( void ) ;

// === treenode structure for huffman tree encoding

/// Tree_node_s structure stores sym, the symbol name, and its frequency.

struct Tree_node_s {
    int freq;                 ///< frequency
    uchar sym;                ///< symbol is NUL if node is an interior node
    struct Tree_node_s * left;  ///< left child
    struct Tree_node_s * right; ///< right child
};

/// Tree_node is a pointer to a Tree_node structure.
typedef struct Tree_node_s * Tree_node;

/// create_tree_node allocates space for a Tree_node and stores the
/// symbol and its frequency.
/// @param sym the unsigned character symbol
/// @param freq the frequency of the symbol's occurrence
/// @return pointer to Tree_node allocated on the heap or NULL on failure

Tree_node create_tree_node( uchar sym, int freq) ;

/// free_tree deallocates the tree.
/// @param tree a pointer to the dynamic storage for the tree node.
/// @pre tree must be a pointer to dynamic storage.

void free_tree_node( Tree_node tree) ;

// === 'tree file' functions

/// write_tree writes an encoded version of tree to the output file pointer.
/// The encoded version of tree inside is called a 'tree file' object.
/// @param ofp the open file pointer to which to write
/// @param tree a pointer to the tree structure to write
/// @return 0 on success and -1 on error

int write_tree( FILE * ofp, Tree_node tree) ;

/// print_tree
/// @param tree a pointer to the tree structure to print

void print_tree( Tree_node tree ) ;

/// read_tree reads an encoded 'tree file' object from the input file pointer.
/// @param fp the open file pointer from which to read.
/// @return Tree_node, a dynamic pointer to the tree, or NULL on failure.
/// @post Tree_node memory is owned by the caller.

Tree_node read_tree( FILE * fp ) ;

/// report_error is a general purpose error print routine.
/// <br>
/// report_error output pattern:
/// <pre>File_name|:|Line_number|:|File_or_function_causing_error|Error_msg|</pre>
/// <br>
/// @param codefile the program source file name of the error
/// @param line the line number of the error
/// @param fname the file/function name of the source of the error
/// @param msg the descriptive content for the error
/// @pre if errno is set, it is from the error

void report_error( char * codefile, int line, char * fname, char * msg ) ;


// // // // // // // // // // // // // // // // // // // // // // // //
// Revisions:
// See 'git log'

#endif // PACKMAN_UTILS_H

