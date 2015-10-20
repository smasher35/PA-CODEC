#ifndef __UTILS_C__
#define __UTILS_C__

/**
 * @file utils.c
 * @brief Description - this files holds common functions related to the first project delivery, in program paCodec
 * @author Paulo Penicheiro nº 2130628 Ruben Miguel nº 2130664
 * @date Novembro - 2015
 * @version 1 
 */

#include <stdio.h>
#include "utils.h"

/**Struct to store info about the matrix */
typedef short pixel_t;
typedef struct pgm
{
	int pgm_type; 	/**< 2 (p2-ASCII) or 5 (p5-raw) */
	int columns; 	/**< width of image */
	int lines; 		/**< height of image */
	int max_value;	/**< max value of pixels */
	char filename[256]; 	/**< name of the file */

	pixel_t **matrix_ptr;
}pgm_t;



 
/** This function proveides information about the programmers of this application */
void about(void){
	printf("==================== paCodec Authors ====================\n");
	printf("* Paulo Penicheiro - 2130628                            *\n");
	printf("* Ruben Miguel - 2130664                                *\n");
	printf("* ---------------------------                           *\n");
	printf("* UC - Programacao Avancada                             *\n");
	printf("* 2015 - 2016                                           *\n");
	printf("=========================================================\n");
}

void read_file(char *filename){
	FILE *file;
	file = fopen(filename, "r");

	if (file == NULL)
	{
			ERROR(2, "Can't open file %s", filename);			
	}

	



}






#endif