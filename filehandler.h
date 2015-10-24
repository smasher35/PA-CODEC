#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

typedef short pixel_t;

/**Struct to store info about the matrix */
typedef struct pgm
{
	int pgm_type; 	/**< 2 (p2-ASCII) or 5 (p5-raw) */
	int columns; 	/**< width of image */
	int lines; 		/**< height of image */
	int max_gray_value;	/**< max value of pixels */
	char filename[256]; 	/**< name of the file */

	pixel_t **matrix_ptr;
}pgm_t;


void read_file(char *filename);
void read_header(pgm_t *pgm_struct, FILE *file, char *filename);
void skip_comments_and_spaces(FILE *file);
void dealloc_matrix(pixel_t **matrix, int lines);

pixel_t **allocate_matrix_p2(int cols, int lines);
void load_matrix_to_struct_p2(pixel_t **matrix, int lines, int cols, FILE *file);

pixel_t **allocate_matrix_p5(int cols, int lines);
#endif