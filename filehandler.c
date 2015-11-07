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


#include "utils.h"
#include "memory.h"
#include "debug.h"
#include "filehandler.h"


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
	cod_struct.blocks_array = (int *)MALLOC(array_size*sizeof(int));


	if (cod_struct.cod_type == 2)
	{		
    	load_cod_file_to_struct(cod_struct.blocks_array, array_size, file);
	}
	else 
	{
		if (cod_struct.cod_type == 5)
		{
			//load_matrix_to_struct_p5(pgm_struct.matrix_ptr, pgm_struct.lines, pgm_struct.columns, pgm_file, pgm_struct.max_gray_value);
		}
	}  	

    	//dealloc_matrix(pgm_struct.matrix_ptr, pgm_struct.columns);
               

	fclose (file);

	return cod_struct;
}


void read_cod_header(cod_t *cod_struct, FILE *file, char *filename)
{
	char version[3];
	fgets(version, sizeof(version), file);
	//DEBUG("Version: %s", version);
	char *fname_aux = NULL;

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
    	
    	
    	/*DEBUG("FILENAME: %s" , cod_struct->filename);
    	DEBUG("COLUNAS: %d" , cod_struct->columns);
    	DEBUG("LINHAS: %d" , cod_struct->lines);
    	DEBUG("MAX DICT BLOCK INDEX: %d" , cod_struct->max_pattern_index);
    	DEBUG("DICT BLOCK WIDTH: %d" , cod_struct->block_width);
    	DEBUG("DICT BLOCK HEIGHT: %d" , cod_struct->block_height);*/
	}
	else
	{
		ERROR(2, "Wrong File Type: %s", version);
	}
}

dict_t read_dictionary (char *dictionary_fname)
{
	FILE *dict_file;
	dict_file = fopen(dictionary_fname, "r");
	dict_t dict_struct;


	if (dict_file == NULL)
	{
		ERROR(1, "Can't open dictionary file %s", dictionary_fname);			
	}

	read_header_dict(&dict_struct, dict_file, dictionary_fname);
	dict_struct.blocks_ptr = alocate_dict_blocks(dict_struct.width_block, dict_struct.height_block, dict_struct.num_blocks);
	load_blocks_to_struct(dict_struct.blocks_ptr, dict_struct.height_block, dict_struct.width_block, dict_file, dict_struct.num_blocks);

	fclose (dict_file);

	return dict_struct;

}

void read_header_dict(dict_t *dict_struct, FILE *dict_file, char *filename)
{
    	fscanf(dict_file, "%d", &dict_struct->num_blocks);
    	fscanf(dict_file, "%d", &dict_struct->height_block);
    	fscanf(dict_file, "%d", &dict_struct->width_block); 

    	/*DEBUG("QUANT BLOCOS: %d" , dict_struct->num_blocks);
    	DEBUG("LARGURA DO BLOCO: %d" , dict_struct->width_block);
    	DEBUG("ALTURA DO BLOCO: %d" , dict_struct->height_block);*/
}


void load_cod_file_to_struct(int *cod_array, int array_size, FILE *file)
{
	int i;
	//DEBUG("ARRAY SIZE: %d", array_size);
	for (i=0 ; i<array_size ; i++)
	{
		skip_comments_and_spaces(file);
		fscanf(file, "%d", &cod_array[i]);

		//printf("%d\n", cod_array[i]);
	}
}

int **alocate_dict_blocks(int width_block, int height_block, int num_blocks)
{
	int **blocks = NULL;
	int i;

	blocks = (int **)MALLOC(num_blocks * sizeof(int*));
	if (blocks == NULL){
		ERROR(11,"CAN'T ALLOCATE BLOCKS (lines)");
	}
	for (i = 0; i < num_blocks; ++i)
	{
		blocks[i] = (int *)MALLOC((width_block*height_block) * sizeof(int));
		if (blocks[i] == NULL) {
			ERROR(12,"CAN'T ALLOCATE BLOCKS (COLUMNS)");
		}
	}
	DEBUG ("ALLOCATED %d BLOCKS FOR DICT", num_blocks);
	return blocks;
}

void load_blocks_to_struct(int **blocks_ptr, int height_block, int width_block, FILE *file, int num_blocks)
{
	int i;
	int n;
	for (i = 0; i < num_blocks; i++)
	{
		for (n = 0; n < height_block + width_block; n++)
		{
			skip_comments_and_spaces(file);
			fscanf(file, "%d", &blocks_ptr[i][n]);
			//printf("%d ", blocks_ptr[i][n]);
		}
		//printf("\n");
		
	}
}

/*************************************************************************************************************/








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



void dealloc_matrix(pixel_t **matrix, int lines)
{
	int i;

	for (i = 0; i < lines; ++i)
	{
		free(matrix[i]);
	}
	free(matrix);
}

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

void read_header(pgm_t *pgm_struct, FILE *file, char *filename)
{
	char version[3];
	fgets(version, sizeof(version), file);
	//DEBUG("Version: %s", version);

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