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
 * @brief Find all symbols in the provided file
 */
uint * find_syms(FILE * input){
	bool still_reading = true;
	size_t nmemb = BUFSIZE/BITS_PER_BYTE;
	size_t size = BITS_PER_BYTE;
	size_t read_size = 0;
	uint * syms = (uint *)calloc(BUFSIZE, sizeof(uint));
	uchar * buffer = (uchar*)calloc(nmemb, sizeof(uchar));

	for(size_t i = 0; i < BUFSIZE; i++){
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

	free(buffer);
	return syms;
}


int main(void){ //Temporary main statement
	return 0;
}
