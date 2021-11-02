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

#define MAGIC_NUM_BYTES 2

//TODO: Shorten all lines to < 80 characters

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
		report_error("packman.c:main:CF1", __LINE__, "argv[1]", " Fatal error: File 1 could not be found");
		return EXIT_FAILURE;
	}
	if(fread(&buffer, MAGIC_NUM_BYTES, 1, file1) != 1){ //File 1 is empty
		errno = EINVAL;
		report_error("packman.c:main:CF1", __LINE__, "argv[1]", " File 1 is empty");
		
		if(fclose(file1) != 0){ //Unable to close file 1
								//FATAL ERROR
			errno = EIO;
			report_error("packman.c:main:CF1", __LINE__, "argv[1]", " Fatal error: File 1 could not be closed properly");
		}

		return EXIT_FAILURE;
	}
	
	//File 1 is encoded
	if(buffer == magic){
		is_file1_code = true;
	}
	
//[CF2]	//Check File 2
	FILE * file2 = NULL;
	if(strcmp(argv[2], "-") == 0){
		if(!is_file1_code){ //Incompatible file type
			errno = EPERM;
			report_error("packman.c:main:CF2", __LINE__, "argv[2]", " Could not decode File 1 to stdout because File 1 is not encoded");
			
			if(fclose(file1) != 0){ //Unable to close file 1
								//FATAL ERROR
				errno = EIO;
				report_error("packman.c:main:CF2", __LINE__, "argv[1]", " Fatal error: File 1 could not be closed properly");	
			}

			return EXIT_FAILURE;
		}
		else {
			//Output to stdout
			file2 = stdout; 
		}
	}
	else{
		file2 = fopen(argv[2], "wb");
		if(file2 == NULL){	//File 2 not found	//FATAL ERROR
			errno = EPERM;
			report_error("packman.c:main:CF2", __LINE__, "argv[2]", " Fatal error: File 2 could not be found or created");
			
			if(fclose(file1) != 0){ //Unable to close file 1
								//FATAL ERROR
				errno = EIO;
				report_error("packman.c:main:CF2", __LINE__, "argv[1]", " Fatal error: File 1 could not be closed properly");
			}

			return EXIT_FAILURE;
		}

	}

//[PIC]	//Process Input Commands
	
	//TODO: Send files to be encoded/decoded
	
	//Temporary close statements
	fclose(file1);
	fclose(file2);

	return EXIT_SUCCESS;
}
