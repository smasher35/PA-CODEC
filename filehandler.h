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

	int *blocks_array;
}cod_t;

typedef	struct dictionary {
	char filename[MAX_FNAME]; /** hold the name of dict file */
	int num_blocks;				/** number of blocks in dict */
	int width_block;			/** width of an individual block */
	int height_block;			/** height of an individual block */

	int **blocks_ptr;			/** vector holding the blocks of dict*/	
}dict_t;


cod_t read_cod_file(char *filename);
void read_cod_header(cod_t *cod_struct, FILE *file, char *filename);
void skip_comments_and_spaces(FILE *file);
int *allocate_cod_blocks_array(int cols, int lines, int dict_block_size);
void dealloc_matrix(pixel_t **matrix, int lines);

void load_cod_file_to_struct(int *cod_array, int array_size, FILE *file);

dict_t read_dictionary (char *dictionary_fname);
void read_header_dict(dict_t *dict_struct, FILE *dict_file, char *filename);

pgm_t read_file(char *filename);
void read_header(pgm_t *pgm_struct, FILE *file, char *filename);
void skip_comments_and_spaces(FILE *file);
void dealloc_matrix(pixel_t **matrix, int lines);

pixel_t **allocate_matrix(int cols, int lines);
void load_matrix_to_struct_p2(pixel_t **matrix, int lines, int cols, FILE *file);
void load_matrix_to_struct_p5(pixel_t **matrix, int lines, int cols, FILE *file, int max_gray_value);

#endif