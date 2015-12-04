#ifndef __PGM_COD_H__
#define __PGM_COD_H__

#define MAX_FNAME 255

typedef struct
{
	char filename[MAX_FNAME];	/**< COD filename */
	char pgm_type[3];	/**< COD file type in string format */
	int columns;				/**< original PGM width */
	int rows;					/**< original PGM height */
	int max_value;				/**< COD max value */
	int block_width;			/**< used DIC file block width */
	int block_height;			/**< used DIC file block height */
	int *indexVector_ptr;		/**< array to hold COD file indexes */
	int num_blocks;				/**< number of blocks in COD file */

} pgmCod_t;

#endif