/**
 * @file utils.c
 * @brief Description - this files holds the features to handle the files and alocate memmory to it'matrix
 * @author Paulo Penicheiro nº 2130628 Ruben Miguel nº 2130664
 * @date Novembro - 2015
 * @version 1 
 */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>
#include <limits.h>


#include "utils.h"
#include "memory.h"
#include "debug.h"
#include "filehandler.h"

/**
 * This function allows skip comments and spaces
 *
 * @param file file to be comments and spaces skipped
 * @return void
 */
void skip_comments_and_spaces(FILE *file)
{
    int ch;
    char line[100];
 
    while ((ch = fgetc(file)) != EOF && isspace(ch))
        ;
    if (ch == '#') {
        fgets(line, sizeof(line), file);
        skip_comments_and_spaces(file);
    } else
        fseek(file, -1, SEEK_CUR);
}

/**
 * This function allows read .cod file
 *
 * @param filename name of the file to be processed
 * @return cod_struct
 */
cod_t read_cod_file(char *filename){
	FILE *file;
	file = fopen(filename, "r");
	cod_t cod_struct;

	if (file == NULL)
	{
		ERROR(1, "Can't open file %s", filename);			
	}	
	
	read_cod_header(&cod_struct, file, filename);
	
	int dict_block_size;
	int array_size;

	dict_block_size = cod_struct.block_width*cod_struct.block_height;	
	array_size = (cod_struct.columns * cod_struct.lines) / dict_block_size;
	cod_struct.blocks_array = (short *)MALLOC(array_size*sizeof(short));


	if (cod_struct.cod_type == 2)
	{		
    	load_cod_file_to_struct(cod_struct.blocks_array, array_size, file);
	}
	else 
	{
		if (cod_struct.cod_type == 5)
		{
			ERROR(1,"FEATURE NOT IMPLEMENTED FOR P5 FILES");
		}
	}  	               

	fclose (file);

	return cod_struct;
}

/**
 * This function allows read .cod file header
 *
 * @param cod_struct structure with the data file 
 * @param file file to be processed
 * @param filename name of the file to be processed
 * @return void
 */
void read_cod_header(cod_t *cod_struct, FILE *file, char *filename)
{
	char version[3];
	fgets(version, sizeof(version), file);
	//DEBUG("Version: %s", version);
	//char *fname_aux = NULL;

	if (strcmp(version, "Z2") == 0 || strcmp(version, "Z5") == 0)
	{

    	cod_struct->cod_type = atoi(version+1);

    	skip_comments_and_spaces(file);
    	fscanf(file, "%d", &cod_struct->columns);    	

    	skip_comments_and_spaces(file);
    	fscanf(file, "%d", &cod_struct->lines);

    	skip_comments_and_spaces(file);
    	fscanf(file, "%d", &cod_struct->max_pattern_index);

    	skip_comments_and_spaces(file);
    	fscanf(file, "%d", &cod_struct->block_width);

    	skip_comments_and_spaces(file);
    	fscanf(file, "%d", &cod_struct->block_height);

    	strcpy(cod_struct->filename, basename(filename));
	}
	else
	{
		ERROR(2, "Wrong File Type: %s", version);
	}
}
/**
 * This function allows read dictionary file 
 *
 * @param dictionary_fname name of the dictionary file
 * @return dict_struct
 */
dict_t read_dictionary (char *dictionary_fname)
{
	FILE *dict_file;
	dict_file = fopen(dictionary_fname, "r");
	dict_t dict_struct;


	if (dict_file == NULL)
	{
		ERROR(1, "Can't open dictionary file %s", dictionary_fname);			
	}

	read_header_dict(&dict_struct, dict_file);
	dict_struct.blocks_ptr = alocate_dict_blocks(dict_struct.width_block, dict_struct.height_block, dict_struct.num_blocks);
	load_blocks_to_struct(dict_struct.blocks_ptr, dict_struct.height_block, dict_struct.width_block, dict_file, dict_struct.num_blocks);

	fclose (dict_file);

	return dict_struct;

}
/**
 * This function allows read dictionary file header 
 *
 * @param dict_struct structure with the dictionary data
 * @param dict_file dictionary file to be processed
 * @param filename dictionary filename
 * @return void
 */
void read_header_dict(dict_t *dict_struct, FILE *dict_file)
{
    	fscanf(dict_file, "%d", &dict_struct->num_blocks);
    	fscanf(dict_file, "%d", &dict_struct->height_block);
    	fscanf(dict_file, "%d", &dict_struct->width_block); 

}

/**
 * This function allows load cod file to structure 
 *
 * @param cod_array array to store dict blocks 
 * @param array_size size of dict array
 * @param file file to be processed
 * @return void
 */
void load_cod_file_to_struct(short *cod_array, int array_size, FILE *file)
{
	int i;
	for (i=0 ; i<array_size ; i++)
	{
		skip_comments_and_spaces(file);
		fscanf(file, "%hd", &cod_array[i]);

	}
}

/**
 * This function allows allocate memory for blocks matrix
 *
 * @param width_block width of the block
 * @param height_block height of the block
 * @param num_blocks number of blocks
 * @return blocks
 */
pixel_t **alocate_dict_blocks(int width_block, int height_block, int num_blocks)
{
	pixel_t **blocks = NULL;
	int i;

	DEBUG("NUM BLOCKS TO ALLOCATE: %d", num_blocks);
	blocks = (pixel_t **)MALLOC(num_blocks * sizeof(pixel_t*));
	if (blocks == NULL){
		ERROR(11,"CAN'T ALLOCATE BLOCKS (lines)");
	}
	for (i = 0; i < num_blocks; ++i)
	{
		blocks[i] = (pixel_t *)MALLOC((width_block*height_block) * sizeof(pixel_t));
		if (blocks[i] == NULL) {
			ERROR(12,"CAN'T ALLOCATE BLOCKS (COLUMNS)");
		}
	}
	DEBUG ("ALLOCATED %d BLOCKS FOR DICT", num_blocks);
	return blocks;
}
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
void load_blocks_to_struct(pixel_t **blocks_ptr, int height_block, int width_block, FILE *file, int num_blocks)
{
	int i;
	int n;
	for (i = 0; i < num_blocks; i++)
	{
		for (n = 0; n < height_block + width_block; n++)
		{
			skip_comments_and_spaces(file);
			fscanf(file, "%hd", &blocks_ptr[i][n]);
		}
		
	}
}
/**
 * This function allows to deallocate dict blocks matrix  
 *
 * @param blocks_ptr pointer to matrix blocks
 * @param lines number of matrix lines
 * @return void
 */
void dealloc_dict_blocks(pixel_t **blocks_ptr, int lines)
{
	int i;

	for (i = 0; i < lines; ++i)
	{
		free(blocks_ptr[i]);
	}
	free(blocks_ptr);
}

/*************************************************************************************************************/
/**
 * This function allows read .pgm file
 *
 * @param filename name of the file to be processed
 * @return pgm_struct
 */
pgm_t read_file(char *filename){
	FILE *pgm_file;
	pgm_file = fopen(filename, "r");
	pgm_t pgm_struct;


	if (pgm_file == NULL)
	{
		ERROR(1, "Can't open file %s", filename);			
	}	

	read_header(&pgm_struct, pgm_file, filename);
	pgm_struct.matrix_ptr = allocate_matrix(pgm_struct.columns, pgm_struct.lines);

	if (pgm_struct.pgm_type == 2)
	{		
		load_matrix_to_struct_p2(pgm_struct.matrix_ptr, pgm_struct.lines, pgm_struct.columns, pgm_file);
	}
	else 
	{
		if (pgm_struct.pgm_type == 5)
		{
			load_matrix_to_struct_p5(pgm_struct.matrix_ptr, pgm_struct.lines, pgm_struct.columns, pgm_file, pgm_struct.max_gray_value);
		}
	}  	

	fclose (pgm_file);

	return pgm_struct;
}
/**
 * This function allows allocate memory for pgm matrix
 *
 * @param cols number of matrix columns
 * @param lines number of matrix lines
 * @return matrix
 */
pixel_t **allocate_matrix(int cols, int lines)
{
	pixel_t **matrix = NULL;
	int i;

	matrix = (pixel_t **)MALLOC(lines * sizeof(pixel_t*));
	if (matrix == NULL){
		ERROR(11,"CAN'T ALLOCATE MATRIX (lines)");
	}

	for (i = 0; i < lines; ++i)
	{
		matrix[i] = (pixel_t *)MALLOC(cols * sizeof(pixel_t));
		if (matrix[i] == NULL) {
			ERROR(12,"CAN'T ALLOCATE MATRIX (COLUMNS)");
		}
	}

	return matrix;
}
/**
 * This function allows to deallocate pgm matrix  
 *
 * @param matrix pgm matrix
 * @param lines number of matrix lines
 * @return void
 */
void dealloc_matrix(pixel_t **matrix, int lines)
{
	int i;

	for (i = 0; i < lines; ++i)
	{
		free(matrix[i]);
	}
	free(matrix);
}
/**
 * This function allows to load pgm P2 (ASCII) file to structure 
 *
 * @param matrix pgm matrix
 * @param lines number of matrix lines
 * @param cols number of matrix columns
 * @param file file to be processed
 * @return void
 */
void load_matrix_to_struct_p2(pixel_t **matrix, int lines, int cols, FILE *file)
{
	int i;
	int n;
	for (i = 0; i < lines; i++)
	{
		for (n = 0; n < cols; n++)
		{
			skip_comments_and_spaces(file);
			fscanf(file, "%hd", &matrix[i][n]);
		}
	}
}
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
void load_matrix_to_struct_p5(pixel_t **matrix, int lines, int cols, FILE *file, int max_gray_value){
	int i;
	int n;

	if (max_gray_value<255)
	{
		for (i = 0; i < lines; i++)
		{
			for (n = 0; n < cols; n++)
			{
				fread(&matrix[i][n], 1, 1, file);;
			}
		}
	}

	else 
	{
		if (max_gray_value>=255 && max_gray_value<65535)    
		{
			for (i = 0; i < lines; i++)
			{
				for (n = 0; n < cols; n++)
				{			
					fread(&matrix[i][n], 2, 1, file);
				}
			}
		}
	}

}
/**
 * This function allows read pgm file header 
 *
 * @param pgm_struct structure with the pgm file data
 * @param file pgm file to be processed
 * @param filename pgm filename
 * @return void
 */
void read_header(pgm_t *pgm_struct, FILE *file, char *filename)
{
	char version[3];
	fgets(version, sizeof(version), file);

	if (strcmp(version, "P2") == 0 || strcmp(version, "P5") == 0)
	{
		strcpy(pgm_struct->filename, filename);
		pgm_struct->pgm_type = atoi(version+1);

		skip_comments_and_spaces(file);
		fscanf(file, "%d", &pgm_struct->columns);    	

		skip_comments_and_spaces(file);
		fscanf(file, "%d", &pgm_struct->lines);

		skip_comments_and_spaces(file);
		fscanf(file, "%d", &pgm_struct->max_gray_value);



		//DEBUG("COLUNAS: %d" , pgm_struct->columns);
		//DEBUG("LINHAS: %d" , pgm_struct->lines);
		//DEBUG("MAX GRAY VALUE: %d" , pgm_struct->max_gray_value);
	}
	else
	{
		ERROR(2, "Wrong File Type: %s", version);
	}
}
/**
 * This function allows to write pgm data structure into file
 *
 * @param pgm_struct structure with the pgm file data
 * @param filename pgm filename to be written
 * @return void
 */

void write_pgm_file(pgm_t pgm_struct, const char *filename)
{
	
	char *bname = filename;
	char *token;
	char *path;

	DEBUG("BNAME: %s" , bname);

	
	if(validate_extension(bname, ".cod") == 1)
	{
		token = strtok(bname, ".");
		bname = token;
		path = bname;
		strcat(path, ".pgm");
	}

	FILE *file;

	file = fopen(path, "w");
	if (file == NULL)
	{
		ERROR(1, "Can't open file %s", path);			
	}
	
	fprintf(file, "P%d\n", pgm_struct.pgm_type);
	fprintf(file, "%d %d\n", pgm_struct.columns, pgm_struct.lines);
	fprintf(file, "%d\n", pgm_struct.max_gray_value);

	int i, j;
	for (i=0; i<pgm_struct.lines; i++)
	{
		for (j = 0; j < pgm_struct.columns; j++)
		{
			fprintf(file, "%d ", pgm_struct.matrix_ptr[i][j]);
		}
		fprintf(file, "\n");
	}



	fclose(file);

}