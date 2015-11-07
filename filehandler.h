/**
 * @file utils.c
 * @brief Description - this files holds the functions prototypes of the filehandler.c file
 * @author Paulo Penicheiro nº 2130628 Ruben Miguel nº 2130664
 * @date Novembro - 2015
 * @version 1 
 */
#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

#define MAX_FNAME (256)

typedef short pixel_t;

/**Struct to store info about the matrix */
typedef struct pgm
{
	int pgm_type; 	/**< 2 (p2-ASCII) or 5 (p5-raw) */
	int columns; 	/**< width of image */
	int lines; 		/**< height of image */
	int max_gray_value;	/**< max value of pixels */
	char filename[MAX_FNAME]; 	/**< name of the file */

	pixel_t **matrix_ptr;
}pgm_t;

typedef struct cod
{
	char filename[MAX_FNAME];
	int cod_type; 	/**< 2 (z2-ASCII) or 5 (z5-raw) */
	int lines; 	/**< number of colums */
	int columns; 		/**< number of lines */
	int max_pattern_index;	/**< max pattern index */
	int block_width;
	int block_height;

	short *blocks_array;
}cod_t;

typedef	struct dictionary {
	char filename[MAX_FNAME]; /** hold the name of dict file */
	int num_blocks;				/** number of blocks in dict */
	int width_block;			/** width of an individual block */
	int height_block;			/** height of an individual block */

	pixel_t **blocks_ptr;			/** vector holding the blocks of dict*/	
}dict_t;

/**
 * This function allows skip comments and spaces
 *
 * @param file file to be comments and spaces skipped
 * @return void
 */
void skip_comments_and_spaces(FILE *file);
/**
 * This function allows read .cod file
 *
 * @param filename name of the file to be processed
 * @return cod_struct
 */
cod_t read_cod_file(char *filename);
/**
 * This function allows read .cod file header
 *
 * @param cod_struct structure with the data file 
 * @param file file to be processed
 * @param filename name of the file to be processed
 * @return void
 */
void read_cod_header(cod_t *cod_struct, FILE *file, char *filename);
/**
 * This function allows read dictionary file 
 *
 * @param dictionary_fname name of the dictionary file
 * @return dict_struct
 */
dict_t read_dictionary (char *dictionary_fname);
/**
 * This function allows read dictionary file header 
 *
 * @param dict_struct structure with the dictionary data
 * @param dict_file dictionary file to be processed
 * @param filename dictionary filename
 * @return void
 */
void read_header_dict(dict_t *dict_struct, FILE *dict_file, char *filename);
/**
 * This function allows load cod file to structure 
 *
 * @param cod_array array to store dict blocks 
 * @param array_size size of dict array
 * @param file file to be processed
 * @return void
 */
void load_cod_file_to_struct(short *cod_array, int array_size, FILE *file);
/**
 * This function allows allocate memory for blocks matrix
 *
 * @param width_block width of the block
 * @param height_block height of the block
 * @param num_blocks number of blocks
 * @return blocks
 */
pixel_t **alocate_dict_blocks(int width_block, int height_block, int num_blocks);
/**
 * This function allows to load dictionary file to structure 
 *
 * @param blocks_ptr pointer to matrix blocks
 * @param height_block height of blocks
 * @param width_block width of blocks
 * @param file file to be processed
 * @param num_blocks number of blocks
 * @return void
 */
void load_blocks_to_struct(pixel_t **matrix_ptr, int height_block, int width_block, FILE *file, int num_blocks);
/**
 * This function allows to deallocate dict blocks matrix  
 *
 * @param blocks_ptr pointer to matrix blocks
 * @param lines number of matrix lines
 * @return void
 */
void dealloc_dict_blocks(pixel_t **blocks_ptr, int lines);
/**
 * This function allows read .pgm file
 *
 * @param filename name of the file to be processed
 * @return pgm_struct
 */
pgm_t read_file(char *filename);
/**
 * This function allows allocate memory for pgm matrix
 *
 * @param cols number of matrix columns
 * @param lines number of matrix lines
 * @return matrix
 */
pixel_t **allocate_matrix(int cols, int lines);
/**
 * This function allows read pgm file header 
 *
 * @param pgm_struct structure with the pgm file data
 * @param file pgm file to be processed
 * @param filename pgm filename
 * @return void
 */
void read_header(pgm_t *pgm_struct, FILE *file, char *filename);
/**
 * This function allows to load pgm P2 (ASCII) file to structure 
 *
 * @param matrix pgm matrix
 * @param lines number of matrix lines
 * @param cols number of matrix columns
 * @param file file to be processed
 * @return void
 */
void load_matrix_to_struct_p2(pixel_t **matrix, int lines, int cols, FILE *file);
/**
 * This function allows to load pgm P5 (binary) file to structure 
 *
 * @param matrix pgm matrix
 * @param lines number of matrix lines
 * @param cols number of matrix columns
 * @param file file to be processed
 * @param max_gray_value maximum gray intensity
 * @return void
 */
void load_matrix_to_struct_p5(pixel_t **matrix, int lines, int cols, FILE *file, int max_gray_value);
/**
 * This function allows to write pgm data structure into file
 *
 * @param pgm_struct structure with the pgm file data
 * @param filename pgm filename to be written
 * @return void
 */
void write_pgm_file(pgm_t pgm_struct, const char *filename);
/**
 * This function allows to deallocate pgm matrix  
 *
 * @param matrix pgm matrix
 * @param lines number of matrix lines
 * @return void
 */
void dealloc_matrix(pixel_t **matrix, int lines);

#endif