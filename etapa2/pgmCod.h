/**
 * @file pgmCod.h
 * @brief Description - this files holds the structure of the enconded file
 * @date Dezembro - 2015
 * @version 1 
 */
#ifndef __PGM_COD_H__
#define __PGM_COD_H__

#define MAX_FNAME 255



typedef struct
{
	char filename[MAX_FNAME];	/**< COD filename */
	int pgm_type;				/**< COD file type in string format */
	int columns;				/**< original PGM width */
	int rows;					/**< original PGM height */
	int max_value;				/**< COD max value */
	int block_width;			/**< used DIC file block width */
	int block_height;			/**< used DIC file block height */
	int *indexVector_ptr;		/**< array to hold COD file indexes */
	int num_blocks;				/**< number of blocks in COD file */

} pgmCod_t;

#endif