#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include "utils.h"
#include "memory.h"
#include "debug.h"
#include "filehandler.h"




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



void skip_comments(FILE *fp)
{
    int ch;
    char line[100];
 
    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        ;
    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        skip_comments(fp);
    } else
        fseek(fp, -1, SEEK_CUR);
}


void read_file(char *filename){
	FILE *pgm_file;
	pgm_file = fopen(filename, "r");
	pgm_t pgm_struct;
	char version[3];
	
	if (pgm_file == NULL)
	{
		ERROR(2, "Can't open file %s", filename);			
	}

	fgets(version, sizeof(version), pgm_file);
	if (!strcmp(version, "P5") && !strcmp(version, "P2")) {
        ERROR(3, "Wrong File Type %s", version);
    }
    else {
    	strcpy(pgm_struct.filename, filename);
    	pgm_struct.pgm_type = atoi(version+1);

    	skip_comments(pgm_file);
    	fscanf(pgm_file, "%d", &pgm_struct.columns);    	

    	skip_comments(pgm_file);
    	fscanf(pgm_file, "%d", &pgm_struct.lines);

    	skip_comments(pgm_file);
    	fscanf(pgm_file, "%d", &pgm_struct.max_gray_value);



    	DEBUG("COLUNAS: %d" , pgm_struct.columns);
    	DEBUG("LINHAS: %d" , pgm_struct.lines);
    	DEBUG("MAX GRAY VALUE: %d" , pgm_struct.max_gray_value);

    	pgm_struct.matrix_ptr = allocate_matrix(pgm_struct.columns, pgm_struct.lines);

    	dealloc_matrix(pgm_struct.matrix_ptr, pgm_struct.columns);
    }    

	fclose (pgm_file);
}

pixel_t **allocate_matrix(int cols, int lines)
{
	pixel_t **matrix = NULL;
	int i;

	matrix = (pixel_t **)MALLOC(lines * sizeof(pixel_t*));
	if (matrix == NULL){
		ERROR(10,"CAN'T ALLOCATE MATRIX (lines)");
	}

	for (i = 0; i < lines; ++i)
	{
		matrix[i] = (pixel_t *)MALLOC(cols * sizeof(pixel_t));
		if (matrix[i] == NULL) {
			ERROR(11,"CAN'T ALLOCATE MATRIX (COLUMNS)");
		}
		
		DEBUG("TAMANHO DA LINHA %d: %d", i, sizeof(matrix[i]) * sizeof(pixel_t));
	}
		
	return matrix;
}

void dealloc_matrix(pixel_t **matrix, int lines)
{
	int i;
 
    for (i = 0; i < lines; ++i)
    {
        free(matrix[i]);
    }
    free(matrix);
}