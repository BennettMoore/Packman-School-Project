// 
// file: packman.c
//
// @author Bennett Moore bwm7637@rit.edu
// // // // // // // // // // // // // // //

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "HeapDT.h"
#include "packman_utils.h"
#include "pack_encode.h"

#define MAGIC_NUM_BYTES 2

//TODO: Shorten all lines to < 80 characters


/**
 * Closes a file, or else returns an error message and crashes the program
 *
 * @param exit The file to be closed
 * @param name The name of the file
 * @param msg The location and information where this function is being called
 * @param line The line number of the call
 *
 * @pre exit is a valid, open file
 *
 */
void close_file(char * msg, int line, char * name, FILE * exit){
	
	if(fclose(exit)){ //Unable to close file		//FATAL ERROR
		errno = EIO;
		report_error(msg, line, name, " Fatal error: File could not be closed properly");
		assert(NULL);
	}
}

/**
 * The main method
 *
 * @param argc number of file arguments
 * @param argv[] array of file arguments
 *
 * @return whether the program ran successfully or not
 */
int main(int argc, char * argv[]){
	
	unsigned short buffer;
	bool is_file1_code = false;

//[IEC]	//Initial error checking
	if(argc < 3){ 			//Too few arguments
		errno = EINVAL;
		report_error("packman.c:main:IEC", __LINE__, "argc", " Not enough arguments");
		return EXIT_FAILURE;
	}
	else if (argc > 3){ 		//Too many arguments
		errno = E2BIG;
		report_error("packman.c:main:IEC", __LINE__, "argc", " Too many arguments");
		return EXIT_FAILURE;
	}
	
	//Get magic number for header checking
	unsigned short magic = get_magic();
	
//[CF1]	//Check File 1
	FILE * file1 = fopen(argv[1], "rb");
	if(file1 == NULL){		//File 1 not found	//FATAL ERROR
		close_file("packman.c:main:CF1", __LINE__, argv[1], file1);
		return EXIT_FAILURE;
	}
	if(fread(&buffer, MAGIC_NUM_BYTES, 1, file1) != 1){ //File 1 is empty
		errno = EINVAL;
		report_error("packman.c:main:CF1", __LINE__, argv[1], " File 1 is empty");
		
		close_file("packman.c:main:CF1", __LINE__, argv[1], file1);

		return EXIT_FAILURE;
	}
	
	//File 1 is encoded
	if(buffer == magic){
		is_file1_code = true;
	}
	
//[CF2]	//Check File 2
	FILE * file2 = NULL;

	//Decode to STDOUT
	if(strcmp(argv[2], "-") == 0){
		if(!is_file1_code){ //Incompatible file type
			errno = EPERM;
			report_error("packman.c:main:CF2", __LINE__, argv[2], " Could not decode File 1 to stdout because File 1 is not encoded");
			
			close_file("packman.c:main:CF2", __LINE__, argv[1], file1);	
			
			return EXIT_FAILURE;
		}
		else {
			//Output to stdout
			file2 = stdout; 
		}
	}
	//Decode to file
	else{
		file2 = fopen(argv[2], "wb");
		if(file2 == NULL){	//File 2 not found	//FATAL ERROR
			errno = EPERM;
			report_error("packman.c:main:CF2", __LINE__, argv[2], " Fatal error: File 2 could not be found or created");
			
			close_file("packman.c:main:CF2", __LINE__, argv[1], file1);

			return EXIT_FAILURE;
		}

	}

//[ETF]	//Encode Text File

	if(!is_file1_code){ //Encode file
		if(fseek(file1, 0, SEEK_SET)){ //Could not rewind
			errno = ESPIPE;
			report_error("packman.c:main:ETF", __LINE__, argv[1], " Could not rewind File 1");
			
			close_file("packman.c:main:ETF", __LINE__, argv[1], file1);
			close_file("packman.c:main:ETF", __LINE__, argv[2], file2);

			return EXIT_FAILURE;
		}
		
		//Find all symbols and use them to make a huffman tree
		Tree_node root = make_huffman_tree(find_syms(file1));

		print_tree(root); //TODO remove test print

		//Construct a lookup table and then use it to make a bit storage array
		uint * bit_st_array = make_bit_array(create_lut(root), file1);
		
		//Write header
		if(fwrite(&magic, sizeof(unsigned short), 1, file2) != 1){
			errno = EINTR;
			report_error("packman.c:main:ETF", __LINE__, "fwrite", " Could not write header to file");
				
			close_file("packman.c:main:ETF", __LINE__, argv[1], file1);
			close_file("packman.c:main:ETF", __LINE__, argv[2], file2);
			
			return EXIT_FAILURE;
		}
		
		//Write tree
		if(write_tree(file2, root)){
			errno = EINTR;
			report_error("packman.c:main:ETF", __LINE__, "write_tree", " Could not write huffman tree to file");

			close_file("packman.c:main:ETF", __LINE__, argv[1], file1);
			close_file("packman.c:main:ETF", __LINE__, argv[2], file2);
			
			return EXIT_FAILURE;
		}
		
		//Write bit storage array length + bit storage array
		if(fwrite(bit_st_array, sizeof(uint), sizeof(bit_st_array)/sizeof(uint*), file2) != sizeof(bit_st_array)/sizeof(uint*)){
			errno = EINTR;
			report_error("packman.c:main:ETF", __LINE__, "fwrite", " Could not write bit storage array");
			
			close_file("packman.c:main:ETF", __LINE__, argv[1], file1);
			close_file("packman.c:main:ETF", __LINE__, argv[2], file2);
			
			return EXIT_FAILURE;
		}
		
		//Housekeeping
		free(bit_st_array);
		free_tree_node(root);
	}
	

//[DBF] //Decode Binary File

	//TODO: Send files to be decoded
	
	//Temporary close statements
	fclose(file1);
	fclose(file2);

	return EXIT_SUCCESS;
}
