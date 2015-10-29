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

    	//dealloc_matrix(pgm_struct.matrix_ptr, pgm_struct.columns);
               

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
			printf("%hd", matrix[i][n]);
			//DEBUG("VALOR DA MATRIZ NA POSICAO %d,%d: %hd", i, n, pgm_struct.matrix_ptr[i][n]);
		}
		printf("\n");
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
				fread(&matrix[i][n], 1, 1, file);
				printf("%hd", matrix[i][n]);
				//DEBUG("VALOR DA MATRIZ NA POSICAO %d,%d: %hd", i, n, pgm_struct.matrix_ptr[i][n]);
			}
			printf("\n");
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
					printf("%hd", matrix[i][n]);
					//DEBUG("VALOR DA MATRIZ NA POSICAO %d,%d: %hd", i, n, pgm_struct.matrix_ptr[i][n]);
				}
				printf("\n");
			}
	    }
    }
    
}

void read_header(pgm_t *pgm_struct, FILE *file, char *filename)
{
	char version[3];
	fgets(version, sizeof(version), file);
	DEBUG("Version: %s", version);

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



    	DEBUG("COLUNAS: %d" , pgm_struct->columns);
    	DEBUG("LINHAS: %d" , pgm_struct->lines);
    	DEBUG("MAX GRAY VALUE: %d" , pgm_struct->max_gray_value);
	}
	else
	{
		ERROR(2, "Wrong File Type: %s", version);
	}
}